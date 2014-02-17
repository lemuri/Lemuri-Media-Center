#include "MediaPlayer.h"

#include "MediaCenter.h"

#include <QDirIterator>
#include <QDebug>

MediaPlayer::MediaPlayer(QObject *parent) :
    QStandardItemModel(parent),
    m_playlist(new QMediaPlaylist(this)),
    m_player(new QMediaPlayer(this))
{
    connect(m_player, &QMediaPlayer::videoAvailableChanged, this, &MediaPlayer::hasVideoChanged);
    m_player->setVolume(0);
}

bool MediaPlayer::hasVideo() const
{
    return m_player->isVideoAvailable();
}

QMediaPlayer *MediaPlayer::mediaObject() const
{
    return m_player;
}

void MediaPlayer::stop()
{
    m_player->stop();
}

void MediaPlayer::nextClip()
{
    if (m_player->state() == QMediaPlayer::StoppedState) {
        m_playlist->next();
        m_player->setVolume(1);
        m_player->setVolume(0);
        //    qDebug() << m_playlist->currentMedia().canonicalResource().;
        QFile *file = new QFile(m_playlist->currentMedia().canonicalUrl().toLocalFile());
        file->open(QFile::ReadOnly);
        m_player->setMedia(m_playlist->currentMedia(), file);
        m_player->play();
    }
}

void MediaPlayer::addMedia(const QString &filename)
{
    m_playlist->addMedia(QUrl::fromLocalFile(filename));
}
