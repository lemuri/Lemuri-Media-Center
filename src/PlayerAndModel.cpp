#include "PlayerAndModel.h"

#include "MediaCenter.h"

#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>
#include <QTimer>
#include <QStringBuilder>
#include <QDebug>

PlayerAndModel::PlayerAndModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_playlist(new QMediaPlaylist(this))
    , m_player(new QMediaPlayer(this, QMediaPlayer::StreamPlayback))
    , m_currentFile(0)
    , m_autoplay(false)
    , m_useCrypt(false)
    , m_stop(false)
    , m_volume(0)
{
    m_roleNames = QAbstractItemModel::roleNames();
    m_roleNames[Title] = "title";

    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &PlayerAndModel::hasVideoChanged);
    connect(m_player, &QMediaPlayer::stateChanged, this, &PlayerAndModel::stateChanged);
    connect(m_player, SIGNAL(error(QMediaPlayer::Error)), SLOT(error(QMediaPlayer::Error)));
    m_player->setVolume(0);

    connect(m_playlist, &QMediaPlaylist::mediaAboutToBeInserted, this, &PlayerAndModel::beginInsertItems);
    connect(m_playlist, &QMediaPlaylist::mediaInserted, this, &PlayerAndModel::endInsertItems);
    connect(m_playlist, &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlayerAndModel::beginRemoveItems);
    connect(m_playlist, &QMediaPlaylist::mediaRemoved, this, &PlayerAndModel::endRemoveItems);
    connect(this, &QAbstractItemModel::rowsInserted, this, &PlayerAndModel::countChanged);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &PlayerAndModel::countChanged);
}

void PlayerAndModel::setPlaybackMode(QMediaPlaylist::PlaybackMode mode)
{
    m_playlist->setPlaybackMode(mode);
    if (mode == QMediaPlaylist::Sequential) {
        m_useCrypt = true;
        loadPlaylist();
    }
}

bool PlayerAndModel::playing() const
{
    if (m_playlist->isEmpty()) {
        return false;
    } else if (m_playlist->playbackMode() != QMediaPlaylist::Sequential) {
        return m_player->state() != QMediaPlayer::StoppedState;
    }
    return true;
}

void PlayerAndModel::setPlaying(bool playing)
{
    if (playing) {
        start();
    } else {
        stop();
    }
}

bool PlayerAndModel::hasVideo() const
{
    return m_player->isVideoAvailable();
}

QObject *PlayerAndModel::mediaObject() const
{
    return m_player;
}

void PlayerAndModel::setAutoplay(bool autoplay)
{
    m_autoplay = autoplay;
}

void PlayerAndModel::setUseCrypt(bool use)
{
    m_useCrypt = use;
    m_player->setPlaylist(0);
}

void PlayerAndModel::shuffle()
{
    m_playlist->shuffle();
}

void PlayerAndModel::clear()
{
    m_playlist->clear();
    savePlaylist();

    stop();
}

void PlayerAndModel::setVolume(int volume)
{
    if (m_volume != volume) {
        m_volume = volume;
        m_player->setVolume(m_volume);
    }
}

void PlayerAndModel::start()
{
    m_stop = false;
    if (m_player->state() == QMediaPlayer::StoppedState) {
        if (m_playlist->isEmpty()) {
            emit playingChanged();
            return;
        }

        if (m_playlist->playbackMode() != QMediaPlaylist::Sequential) {
            m_playlist->next();
        } else {
            m_playlist->setCurrentIndex(0);
        }

        // Hack to make sure we have the right volume on all streams
        m_player->setVolume(1);
        m_player->setVolume(0);
        m_player->setVolume(m_volume);

        if (!m_playlist->currentMedia().isNull()) {
            m_currentFile = new QFile(m_playlist->currentMedia().canonicalUrl().toLocalFile());
            if (m_currentFile->open(QFile::ReadOnly)) {
                m_player->setMedia(m_playlist->currentMedia(), m_currentFile);
                m_player->play();

                // Skip the media remove and another start() call
                return;
            } else {
                QString error = QString("Falha ao abrir arquivo: %1").arg(m_currentFile->errorString());
                qWarning() << error;
                emit notification(error);
            }
        } else {
            // something odd happened as the next item on the
            // playlist was null
            qWarning() << "Media nula";
        }

        // Either an error happened or the current media was
        // null, so remove the media and try again.
        m_playlist->removeMedia(m_playlist->currentIndex());
        QTimer::singleShot(0, this, SLOT(start()));
    }
}

void PlayerAndModel::stop()
{
    m_stop = true;
    m_player->stop();

    emit playingChanged();
}

void PlayerAndModel::next()
{
    if (m_player->state() == QMediaPlayer::PlayingState) {
        m_player->stop();
    } else {
        start();
    }
}

bool PlayerAndModel::addMedia(const QString &filename)
{
    if (QFile::exists(filename)) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            QString title;
            QMediaResource resource(QUrl::fromLocalFile(filename), title);

            m_playlist->addMedia(resource);
            savePlaylist();

            if (m_autoplay) {
                QTimer::singleShot(0, this, SLOT(start()));
            }
            return true;
        }
    }
    return false;
}

int PlayerAndModel::rowCount(const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid() ? m_playlist->mediaCount() : 0;
}

int PlayerAndModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex PlayerAndModel::index(int row, int column, const QModelIndex &parent) const
{
    return m_playlist && !parent.isValid()
            && row >= 0 && row < m_playlist->mediaCount()
            && column >= 0 && column < ColumnCount
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex PlayerAndModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant PlayerAndModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        if (index.column() == Title) {
            return m_playlist->media(index.row()).canonicalResource().mimeType();
        }
    }
    return QVariant();
}

QHash<int, QByteArray> PlayerAndModel::roleNames() const
{
    return m_roleNames;
}

QMediaPlaylist *PlayerAndModel::playlist() const
{
    return m_playlist;
}

void PlayerAndModel::stateChanged(QMediaPlayer::State newState)
{
//    qDebug() << Q_FUNC_INFO << newState << m_playlist->playbackMode();
    if (newState == QMediaPlayer::StoppedState) {
        removeCurrent();

        if (m_stop) {
            // This is needed to clear the output buffer when
            // we stop in the middle of a video
            m_player->setMedia(QMediaContent());
        } else {
            // Queue the start because we were called
            // by the player itself, which means the error
            // signal might not be processed yet.
            QTimer::singleShot(0, this, SLOT(start()));
        }
    } else {
        emit playingChanged();
    }
}

void PlayerAndModel::error(QMediaPlayer::Error error)
{
    qDebug() << Q_FUNC_INFO << m_player->state() << error << m_player->errorString();
    emit notification(m_player->errorString());

    removeCurrent();

    // Depending on the error stateChanged is not emitted, BUG?
    QTimer::singleShot(0, this, SLOT(start()));
}

void PlayerAndModel::beginInsertItems(int start, int end)
{
    beginInsertRows(QModelIndex(), start, end);
}

void PlayerAndModel::endInsertItems()
{
    endInsertRows();
}

void PlayerAndModel::beginRemoveItems(int start, int end)
{
    beginRemoveRows(QModelIndex(), start, end);
}

void PlayerAndModel::endRemoveItems()
{
    endRemoveRows();
}

void PlayerAndModel::changeItems(int start, int end)
{
    emit dataChanged(index(start,0), index(end,ColumnCount));
}

void PlayerAndModel::savePlaylist()
{
    if (m_playlist->playbackMode() != QMediaPlaylist::Sequential) {
        return;
    }

    QFile playlistFile(MediaCenter::pathPlaylist());
    playlistFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&playlistFile);
    for (int i = 0; i < m_playlist->mediaCount(); ++i) {
        out << m_playlist->media(i).canonicalUrl().toString() << endl;
    }
}

void PlayerAndModel::loadPlaylist()
{
    QFile playlistFile(MediaCenter::pathPlaylist());
    playlistFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&playlistFile);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line[0] == QLatin1Char('#') || line.size() > 4096) {
            continue;
        }

        addMedia(QUrl(line).toLocalFile());
    }
}

void PlayerAndModel::removeCurrent()
{
    if (m_playlist->playbackMode() != QMediaPlaylist::Sequential) {
        return;
    }

    if (m_currentFile && m_currentFile == m_player->mediaStream()) {
        delete m_currentFile;
        m_currentFile = 0;
        m_playlist->removeMedia(0);
        savePlaylist();
    }
}
