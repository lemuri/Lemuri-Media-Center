#ifndef MEDIACENTER_H
#define MEDIACENTER_H

#include <QApplication>
#include <QObject>
#include <QStringList>

class QQuickView;
class PlayerAndModel;
class MusicImporter;
class KeyManager;
class MainWidgetView;
class MediaCenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool indexReady READ indexReady NOTIFY indexReadyChanged)
    Q_PROPERTY(int volume READ volume NOTIFY volumeChanged)
    Q_PROPERTY(int coverSize READ coverSize WRITE setCoverSize NOTIFY coverSizeChanged)
    Q_PROPERTY(bool showVideo READ showVideo NOTIFY showVideoChanged)
    Q_PROPERTY(bool showCursor READ showCursor WRITE setShowCursor NOTIFY showCursorChanged)
    Q_PROPERTY(bool fullScreen READ fullScreen WRITE setFullScreen NOTIFY fullScreenChanged)
    Q_PROPERTY(QStringList genreFilters READ genreFilters WRITE setGenreFilters NOTIFY genreFiltersChanged)
    Q_PROPERTY(QString version READ version)
public:
    enum Roles {
        RoleData = Qt::UserRole + 1,
        RoleKind
    };
    MediaCenter();
    ~MediaCenter();

    bool registerDBusService();
    void show();

    bool indexReady() const;
    int volume() const;
    int coverSize() const;
    void setCoverSize(int coverSize);
    bool showVideo() const;
    void showVideoStart();
    bool showCursor() const;
    void setShowCursor(bool showCursor);
    bool fullScreen() const;
    void setFullScreen(bool fullScreen);
    KeyManager* keyManager() const;

    Q_INVOKABLE void increaseVolume();
    Q_INVOKABLE void decreaseVolume();
    Q_INVOKABLE void hideVideo();

    QStringList genreFilters() const;
    void setGenreFilters(const QStringList &genreFilters);
    QString version() const;

    Q_INVOKABLE bool playMedia(const QString &filename, const QString &title, const QString &album, const QString &recorder, const QString &isrc);

    Q_INVOKABLE QString config(const QString &name, const QString &defaultValue = QString());
    Q_INVOKABLE QStringList config(const QString &name, const QStringList &defaultValue);
    Q_INVOKABLE int config(const QString &name, int defaultValue);
    Q_INVOKABLE void setConfig(const QString &name, const QString &value);
    Q_INVOKABLE void setConfig(const QString &name, int value);

    static QString dataDir();
    static QString coverPath(const QString &absoluteFilePath);
    static QString coverThumbPath(const QString &absolutePath);
    static QString backgroundPath();
    static QString pathMedia();
    static QString pathPlaylist();

    static QString xapianDBPath();

signals:
    void indexReadyChanged();
    void indexChanged();
    void showCursorChanged();
    void fullScreenChanged();
    void volumeChanged(int volume);
    void coverSizeChanged(int size);
    void showVideoChanged();
    void notification(const QString &message);
    void settingsModelChanged();
    void genreFiltersChanged();
    void mediaCountChanged();

private slots:
    void updateMediaCount(int mediaCount);
    void indexFinished();
    void showVideoTimeout();

protected:
    friend class KeyManager;
    void close();

private:
    void createMediaFolders();
    void initKeys();

    QQuickView *m_view;
    MainWidgetView *m_widgetView;
    QThread *m_importerThread;
    MusicImporter *m_importer;
    QTimer *m_showVideoTimer;
    PlayerAndModel *m_playerModel;
    QStringList m_genreFilters;
    bool m_genreFiltersChanged;
    KeyManager *m_keyManager;
    bool m_indexReady;
    bool m_showVideo;
    bool m_showCursor = true;
    bool m_showFullScreen;
    int m_volume;
    int m_coverSize;
};

#endif // MEDIACENTER_H
