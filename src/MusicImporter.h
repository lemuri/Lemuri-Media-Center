#ifndef MUSICIMPORTER_H
#define MUSICIMPORTER_H

#include <QObject>
#include <QStringList>
#include <QFileInfo>
#include <QTimer>
#include <QMimeDatabase>
#include <QNetworkAccessManager>

#undef slots
#include <xapian.h>
#define slots

class MusicImporter : public QObject
{
    Q_OBJECT
public:
    enum XapianRoles {
        AbsoluteFilePath,
        AbsolutePath,
        FileLastModified,
        FileSize,
        Album,
        AlbumDir,
        AlbumYear,
        Artist,
        ArtistDir,
        Title,
        TrackNumber,
        Genre,
        Lenght,
        Rating,
        IsVideo
    };
    MusicImporter();

    QString uniqueId(const QString &filePath) const;

signals:
    void errorMessage(const QString &text);
    void mediaCountChanged(int mediaCount);
    void changed();
    void finished();

public slots:
    void init();
    void markItem(const QString &filename);

private:
    bool open();
    bool validateEntries();
    bool scanPath(const QString &path);
    void commit();
    bool hasDocument(const QString &uniqueId);
    bool addFile(const QFileInfo &fileInfo, const QString &uuid = QString());
    void createCoverFile(const QFileInfo &fileInfo, const QString &title);
    void checkMediaCountChanged(quint64 &mediaCount);
    static QString movieSection(const QString &absolutePath);

    Xapian::WritableDatabase *m_rwXapianDB;
    QNetworkAccessManager m_nam;
    QMimeDatabase m_db;
    QString m_mediaPath;
    quint64 m_mediaCount;
};

#endif // MUSICIMPORTER_H
