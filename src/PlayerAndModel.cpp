#include "PlayerAndModel.h"

#include "MediaCenter.h"

#include <QFileInfo>
#include <QUrl>
#include <QMediaPlaylist>
#include <QTime>
#include <QTimer>
#include <QStringBuilder>
#include <QDebug>

PlayerAndModel::PlayerAndModel(QObject *parent)
    : QObject(parent)
    , m_player(new QMediaPlayer(this, QMediaPlayer::StreamPlayback))
    , m_currentFile(0)
    , m_volume(0)
    , m_state(Stopped)
{
    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &PlayerAndModel::hasVideoChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayerAndModel::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayerAndModel::durationChanged);
    connect(m_player, &QMediaPlayer::stateChanged, this, &PlayerAndModel::mediaStateChanged);
    connect(m_player, SIGNAL(error(QMediaPlayer::Error)), SLOT(error(QMediaPlayer::Error)));
    m_player->setVolume(0);
}

PlayerAndModel::State PlayerAndModel::state() const
{
    return m_state;
}

bool PlayerAndModel::hasVideo() const
{
    return m_player->isVideoAvailable();
}

qint64 PlayerAndModel::duration() const
{
    return m_player->duration();
}

QString PlayerAndModel::durationString() const
{
    return QTime(0, 0).addSecs(m_player->duration() / 1000).toString(QLatin1String("-hh:mm:ss"));
}

QString PlayerAndModel::remainingString() const
{
    int value = (m_player->duration() - m_player->position()) / 1000;
    return QTime(0, 0).addSecs(value).toString(QLatin1String("-hh:mm:ss"));
}

qint64 PlayerAndModel::position() const
{
    return m_player->position();
}

void PlayerAndModel::setPosition(qint64 position)
{
    m_player->setPosition(position);
}

int PlayerAndModel::volume() const
{
    return m_volume;
}

void PlayerAndModel::setVolume(int volume)
{
    if (m_volume != volume) {
        m_volume = volume;
        m_player->setVolume(m_volume);
        emit volumeChanged();
    }
}

QObject *PlayerAndModel::mediaObject() const
{
    return m_player;
}

void PlayerAndModel::play()
{
    m_player->play();
//    m_stop = false;
//    if (m_player->state() == QMediaPlayer::StoppedState) {

//        // Hack to make sure we have the right volume on all streams
//        m_player->setVolume(1);
//        m_player->setVolume(0);
//        m_player->setVolume(m_volume);

//        if (!m_playlist->currentMedia().isNull()) {
//            m_currentFile = new QFile(m_playlist->currentMedia().canonicalUrl().toLocalFile());
//            if (m_currentFile->open(QFile::ReadOnly)) {
//                m_player->setMedia(m_playlist->currentMedia(), m_currentFile);
//                m_player->play();

//                // Skip the media remove and another start() call
//                return;
//            } else {
//                QString error = QString("Falha ao abrir arquivo: %1").arg(m_currentFile->errorString());
//                qWarning() << error;
//                emit notification(error);
//            }
//        } else {
//            // something odd happened as the next item on the
//            // playlist was null
//            qWarning() << "Media nula";
//        }

//        // Either an error happened or the current media was
//        // null, so remove the media and try again.
//        m_playlist->removeMedia(m_playlist->currentIndex());
//        QTimer::singleShot(0, this, SLOT(start()));
//    }
}

void PlayerAndModel::pause()
{
    m_player->pause();
}

void PlayerAndModel::stop()
{
    m_player->stop();
}

bool PlayerAndModel::addMedia(const QString &filename)
{
    if (QFile::exists(filename)) {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly)) {
            QString title;
            QMediaResource resource(QUrl::fromLocalFile(filename), title);

            m_player->setMedia(resource);
            m_player->play();

            return true;
        }
    }
    return false;
}

void PlayerAndModel::mediaStateChanged(QMediaPlayer::State newState)
{
    qDebug() << "mediaStateChanged" << newState << hasVideo();
    switch (newState) {
    case QMediaPlayer::PlayingState:
        m_state = Playing;
        break;
    case QMediaPlayer::StoppedState:
        m_state = Stopped;
        break;
    case QMediaPlayer::PausedState:
        m_state = Paused;
        break;
    }

    emit stateChanged();
}

void PlayerAndModel::error(QMediaPlayer::Error error)
{
    qDebug() << Q_FUNC_INFO << m_player->state() << error << m_player->errorString();
    emit notification(m_player->errorString());
}
