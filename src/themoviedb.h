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

    void getCover(const QFileInfo &fileInfo, QStringList &titles);

    void setNetworkAccessManager(QNetworkAccessManager *nam);

private:
    void searchFinished();
    void downloadFinished();
    void tryAgain();

    QNetworkAccessManager *m_nam = 0;
    QString m_title;
    QStringList m_titles;
    QFileInfo m_fileInfo;
};

#endif // THEMOVIEDB_H
