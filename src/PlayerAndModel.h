#ifndef PLAYER_AND_MODEL_H
#define PLAYER_AND_MODEL_H

#include <QAbstractItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>

class PlayerAndModel : public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(int state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool hasVideo READ hasVideo NOTIFY hasVideoChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(QObject* mediaObject READ mediaObject NOTIFY mediaObjectChanged)
public:
    enum State
    {
        Playing,
        Paused,
        Stopped
    };
    explicit PlayerAndModel(QObject *parent = 0);

    State state() const;
    bool hasVideo() const;

    qint64 duration() const;

    qint64 position() const;
    void setPosition(qint64 position);

    int volume() const;
    void setVolume(int volume);

    QObject* mediaObject() const;

public slots:

    void play();
    void pause();
    void stop();
    bool addMedia(const QString &filename);

signals:
    void notification(const QString &message);
    void hasVideoChanged(bool videoAvailable);
    void mediaObjectChanged();
    void stateChanged();
    void durationChanged();
    void positionChanged();
    void volumeChanged();

private slots:
    void mediaStateChanged(QMediaPlayer::State newState);
    void error(QMediaPlayer::Error error);

private:
    QMediaPlayer *m_player;
    QFile *m_currentFile;
    int m_volume;
    State m_state;
};

#endif // PLAYER_AND_MODEL_H
