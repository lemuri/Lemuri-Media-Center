#ifndef PLAYER_AND_MODEL_H
#define PLAYER_AND_MODEL_H

#include <QAbstractItemModel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFile>

class PlayerAndModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(bool playing READ playing WRITE setPlaying NOTIFY playingChanged)
    Q_PROPERTY(bool hasVideo READ hasVideo NOTIFY hasVideoChanged)
    Q_PROPERTY(QObject* mediaObject READ mediaObject NOTIFY mediaObjectChanged)
public:
    enum Column
    {
        Title = 0,
        ColumnCount
    };
    explicit PlayerAndModel(QObject *parent = 0);

    void setPlaybackMode(QMediaPlaylist::PlaybackMode mode);
    bool playing() const;
    void setPlaying(bool playing);
    bool hasVideo() const;
    QObject* mediaObject() const;
    void setAutoplay(bool autoplay);
    void setUseCrypt(bool use);
    void shuffle();
    void clear();

public slots:
    void setVolume(int volume);

    void start();
    void stop();
    void next();
    bool addMedia(const QString &filename);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;

    QMediaPlaylist *playlist() const;
    void setPlaylist(QMediaPlaylist *playlist);

signals:
    void countChanged();
    void notification(const QString &message);
    void hasVideoChanged(bool videoAvailable);
    void mediaObjectChanged();
    void playingChanged();

private slots:
    void stateChanged(QMediaPlayer::State newState);
    void error(QMediaPlayer::Error error);
    void beginInsertItems(int start, int end);
    void endInsertItems();
    void beginRemoveItems(int start, int end);
    void endRemoveItems();
    void changeItems(int start, int end);
    void savePlaylist();
    void loadPlaylist();

private:
    void removeCurrent();

    QMediaPlaylist *m_playlist;
    QMediaPlayer *m_player;
    QHash<int, QByteArray> m_roleNames;
    QFile *m_currentFile;
    bool m_autoplay;
    bool m_useCrypt;
    bool m_stop;
    int m_volume;
};

#endif // PLAYER_AND_MODEL_H
