#include "themoviedb.h"

#include "MediaCenter.h"

#include <QStringBuilder>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>

#define LEMURI_MEDIA_CENTER_KEY "3e4b22d9919ae1fcaada321737e19dba"

TheMovieDB::TheMovieDB(QObject *parent) :
    QObject(parent)
{
}

void TheMovieDB::getCover(const QFileInfo &fileInfo, QStringList &titles)
{
    m_title = titles.takeFirst().replace(QRegularExpression("[^\\w]"), " ");
    m_titles = titles;
    m_fileInfo = fileInfo;

    QUrl url("https://api.themoviedb.org/3/search/movie");
    QUrlQuery query;
    query.addQueryItem(QLatin1String("api_key"), QLatin1String(LEMURI_MEDIA_CENTER_KEY));
    query.addQueryItem(QLatin1String("query"), m_title);
    query.addQueryItem(QLatin1String("language"), QLatin1String("pt_BR"));
    url.setQuery(query);

    QNetworkReply *reply = m_nam->get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished,
            this, &TheMovieDB::searchFinished);
}

void TheMovieDB::setNetworkAccessManager(QNetworkAccessManager *nam)
{
    m_nam = nam;
}

void TheMovieDB::searchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (document.isNull()) {
        qDebug() << "Null document for" << m_title;
        tryAgain();
        return;
    }

    QJsonArray array = document.object()["results"].toArray();
    qDebug() << array;
    if (array.isEmpty()) {
        qDebug() << "Null array for" << m_title;
        tryAgain();
        return;
    }

    QString posterPath = array.first().toObject()["poster_path"].toString();
    if (!posterPath.isEmpty()) {
        QUrl url(QLatin1String("http://image.tmdb.org/t/p/w500/") % posterPath);
        QNetworkReply *replyImg = m_nam->get(QNetworkRequest(url));
        connect(replyImg, &QNetworkReply::finished,
                this, &TheMovieDB::downloadFinished);
    } else {
        qDebug() << "Null results for" << m_title;
    }
}

void TheMovieDB::downloadFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (reply->error()) {
        qDebug() << reply->errorString();
        tryAgain();
        return;
    }

    const QString &cover = MediaCenter::coverPath(m_fileInfo.absoluteFilePath());
    QFile imgFile(cover);
    if (imgFile.open(QFile::ReadWrite)) {
        imgFile.write(reply->readAll());
    } else {
        qDebug() << "Failed to open" << imgFile.fileName();
    }

    deleteLater();
}

void TheMovieDB::tryAgain()
{
    if (m_titles.isEmpty()) {
        deleteLater();
        return;
    }

    getCover(m_fileInfo, m_titles);
}
