#ifndef THEMOVIEDB_H
#define THEMOVIEDB_H

#include <QObject>
#include <QFileInfo>
#include <QNetworkAccessManager>

class TheMovieDB : public QObject
{
    Q_OBJECT
public:
    explicit TheMovieDB(QObject *parent = 0);

    void getCover(const QFileInfo &fileInfo, const QString &title);

    void setNetworkAccessManager(QNetworkAccessManager *nam);

private:
    void searchFinished();
    void downloadFinished();

    QNetworkAccessManager *m_nam = 0;
    QString m_title;
    QFileInfo m_fileInfo;
};

#endif // THEMOVIEDB_H
