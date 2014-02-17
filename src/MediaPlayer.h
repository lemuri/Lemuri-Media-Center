#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QStandardItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class MediaPlayer : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasVideo READ hasVideo NOTIFY hasVideoChanged)
    Q_PROPERTY(QObject* mediaObject READ mediaObject NOTIFY mediaObjectChanged)
public:
    explicit MediaPlayer(QObject *parent = 0);

    bool hasVideo() const;
    QMediaPlayer* mediaObject() const;
    void loadClips();

    Q_INVOKABLE void stop();
    Q_INVOKABLE void nextClip();
    Q_INVOKABLE void addMedia(const QString &filename);

signals:
    void hasVideoChanged(bool videoAvailable);
    void mediaObjectChanged();
    
private:
    QMediaPlaylist *m_playlist;
    QMediaPlayer *m_player;
};

#endif // MEDIAPLAYER_H
