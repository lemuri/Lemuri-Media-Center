#include "MediaCenter.h"

#include "config.h"
#include "MusicImporter.h"
#include "BackgroundModel.h"
#include "PlayerAndModel.h"
#include "MediaPlayer.h"
#include "CoverProvider.h"
#include "KeyManager.h"
#include "IconProvider.h"
#include "DBusInterface.h"
#include "ScreenSaver.h"
#include "GenreModel.h"
#include "AlbumModel.h"
#include "TracksModel.h"

#include <QtQml/QQmlContext>
#include <QtQml/QQmlTypeInfo>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlComponent>
#include <QtQuick/QQuickView>

#include <QStyleFactory>
#include <QStringBuilder>
#include <QDateTime>
#include <QThread>
#include <QSettings>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

#define CORE_GENRE_FILTERS "core/GenreFilters"
#define CORE_MONEY_ACCUMULATED "core/MoneyAccumulated"
#define CORE_CREDITS_MULTIPLIER "core/CreditsMultiplier"
#define GUI_SHOW_FULLSCREEN "gui/ShowFullScreen"
#define GUI_VOLUME "gui/Volume"
#define GUI_COVER_SIZE "gui/CoverSize"
#define GUI_RENT_TEXT "gui/RentText"

MediaCenter::MediaCenter() :
    m_importerThread(new QThread(this)),
    m_genreFiltersChanged(false),
    m_indexReady(false),
    m_showVideo(true),
    m_configuring(false),
    m_keyManager(new KeyManager(this))
{
    m_volume = config(GUI_VOLUME, 100);
    m_coverSize = config(GUI_COVER_SIZE, 240);
    m_showFullScreen = config(GUI_SHOW_FULLSCREEN, true);
    QSettings settings;
    setGenreFilters(settings.value(QLatin1String(CORE_GENRE_FILTERS)).toStringList());

    m_showVideoTimer = new QTimer(this);
    m_showVideoTimer->setSingleShot(true);
    m_showVideoTimer->setInterval(5000);
    connect(m_showVideoTimer, &QTimer::timeout, this, &MediaCenter::showVideoTimeout);
    connect(this, SIGNAL(configuringChanged()), m_showVideoTimer, SLOT(start()));

    QTime timer;
    timer.start();

    createMediaFolders();

    m_importer = new MusicImporter;
    connect(m_importer, &MusicImporter::mediaCountChanged, this, &MediaCenter::updateMediaCount);
    connect(m_importer, &MusicImporter::changed, this, &MediaCenter::indexChanged);
    connect(m_importer, &MusicImporter::finished, this, &MediaCenter::indexFinished);
    connect(m_importer, &MusicImporter::errorMessage, this, &MediaCenter::notification);
    connect(m_importerThread, &QThread::started, m_importer, &MusicImporter::init);
    m_importer->moveToThread(m_importerThread);

    m_view = new QQuickView;
    m_view->setWidth(1024);
    m_view->setHeight(768);
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
//    m_view->setCursor(Qt::BlankCursor);
    m_view->installEventFilter(m_keyManager);

    m_playerModel = new PlayerAndModel(this);
    m_playerModel->setPlaybackMode(QMediaPlaylist::Sequential);
    m_playerModel->setUseCrypt(true);
    m_playerModel->setAutoplay(true);
    m_playerModel->setVolume(volume());
//    connect(m_playerModel, &PlayerAndModel::hasVideoChanged, this, &MediaCenter::paidMediaChanged);
//    connect(m_playerModel, &PlayerAndModel::playingChanged, this, &MediaCenter::paidMediaChanged);
    connect(m_playerModel, &PlayerAndModel::notification, this, &MediaCenter::notification);
    connect(this, SIGNAL(volumeChanged(int)), m_playerModel, SLOT(setVolume(int)));

    qmlRegisterType<GenreModel>("MediaCenter", 1, 0, "GenreModel");
    qmlRegisterType<AlbumModel>("MediaCenter", 1, 0, "AlbumModel");
    qmlRegisterType<TracksModel>("MediaCenter", 1, 0, "TracksModel");
    qmlRegisterType<PlayerAndModel>("MediaCenter", 1, 0, "PlayerAndModel");

    m_view->rootContext()->setContextProperty(QLatin1String("Backend"), this);
    m_view->rootContext()->setContextProperty(QLatin1String("KeyManager"), m_keyManager);
    m_view->rootContext()->setContextProperty(QLatin1String("BackgroundModel"), new BackgroundModel(this));
    m_view->rootContext()->setContextProperty(QLatin1String("Player"),  m_playerModel);

    m_view->engine()->addImageProvider(QLatin1String("icon"), new IconProvider);
    m_view->engine()->addImageProvider(QLatin1String("cover"), new CoverProvider);

    connect(m_view->engine(), SIGNAL(quit()), QGuiApplication::instance(), SLOT(quit()));

#ifdef QML_RESOURCE
    m_view->setSource(QUrl(QLatin1String("qrc:/qml/main.qml")));
#else
    m_view->setSource(QUrl::fromLocalFile("/home/daniel/code/MediaCenter/src/qml/main.qml"));
#endif // QML_RESOURCE
}

MediaCenter::~MediaCenter()
{
    if (m_importerThread) {
        m_importerThread->quit();
        m_importerThread->wait();
    }
}

bool MediaCenter::registerDBusService()
{
    DBusInterface *interface = new DBusInterface(this);
    connect(interface, &DBusInterface::updateIndex,
            m_importer, &MusicImporter::init, Qt::QueuedConnection);

    if (interface->registerService()) {
        m_importerThread->start();
        (void) new ScreenSaver(this);

        return true;
    }
    return false;
}

void MediaCenter::show()
{
//    MainWidgetView *view = new MainWidgetView(this);

    if (m_showFullScreen) {
        m_view->showFullScreen();
    } else {
        m_view->showNormal();
    }
}

bool MediaCenter::indexReady() const
{
    return m_indexReady;
}

int MediaCenter::volume() const
{
    return m_volume;
}

int MediaCenter::coverSize() const
{
    return m_coverSize;
}

void MediaCenter::setCoverSize(int coverSize)
{
    if (m_coverSize != coverSize) {
        m_coverSize = coverSize;
        setConfig(GUI_COVER_SIZE, m_coverSize);
        emit coverSizeChanged(m_coverSize);
    }
}

bool MediaCenter::showVideo() const
{
    return m_showVideo;
}

void MediaCenter::showVideoStart()
{
    m_showVideoTimer->start();
}

bool MediaCenter::configuring() const
{
    return m_configuring;
}

void MediaCenter::setConfiguring(bool configuring)
{
    if (m_configuring != configuring) {
        m_configuring = configuring;
        if (configuring) {
            m_view->setCursor(Qt::ArrowCursor);
        } else {
            m_view->setCursor(Qt::BlankCursor);
            if (m_genreFiltersChanged) {
                emit genreFiltersChanged();
            }
        }
        emit configuringChanged();
    }
}

bool MediaCenter::showFullScreen() const
{
    return m_showFullScreen;
}

void MediaCenter::setShowFullScreen(bool showFullScreen)
{
    if (m_showFullScreen != showFullScreen) {
        m_showFullScreen = showFullScreen;
        setConfig(GUI_SHOW_FULLSCREEN, showFullScreen);
        show();
        emit showFullScreenChanged();
    }
}

KeyManager *MediaCenter::keyManager() const
{
    return m_keyManager;
}

void MediaCenter::increaseVolume()
{
    int volume = m_volume + 5;
    if (volume > 100) {
        volume = 100;
    }

    if (m_volume != volume) {
        m_volume = volume;
        setConfig("gui/Volume", m_volume);
        emit volumeChanged(m_volume);
    }
}

void MediaCenter::decreaseVolume()
{
    int volume = m_volume - 5;
    if (volume < 0) {
        volume = 0;
    }

    if (m_volume != volume) {
        m_volume = volume;
        setConfig(GUI_VOLUME, m_volume);
        emit volumeChanged(m_volume);
    }
}

void MediaCenter::hideVideo()
{
    if (m_showVideo) {
        m_showVideo = false;
        emit showVideoChanged();
    }
    m_showVideoTimer->start();
}

void MediaCenter::clearPlayingList()
{
    m_playerModel->clear();
}

QStringList MediaCenter::genreFilters() const
{
    return m_genreFilters;
}

void MediaCenter::setGenreFilters(const QStringList &genreFilters)
{
    QStringList genresFixed = genreFilters;
    genresFixed.sort();
    genresFixed.removeDuplicates();
    genresFixed.removeAll(QString());
    genresFixed.removeAll(QString(""));
    if (m_genreFilters != genresFixed) {
        QSettings settings;
        settings.setValue(QLatin1String(CORE_GENRE_FILTERS), genresFixed);
        m_genreFilters = genresFixed;
        if (m_configuring) {
            m_genreFiltersChanged = true;
        } else {
            m_genreFiltersChanged = false;
            emit genreFiltersChanged();
        }
    }
}

QString MediaCenter::version() const
{
    return QCoreApplication::applicationVersion();
}

bool MediaCenter::playMedia(const QString &filename, const QString &title, const QString &album, const QString &recorder, const QString &isrc)
{
    return m_playerModel->addMedia(filename);
}

QString MediaCenter::config(const QString &name, const QString &defaultValue)
{
    QSettings settings;
    return settings.value(name, defaultValue).toString();
}

QStringList MediaCenter::config(const QString &name, const QStringList &defaultValue)
{
    QSettings settings;
    return settings.value(name, defaultValue).toStringList();
}

int MediaCenter::config(const QString &name, int defaultValue)
{
    return config(name, QString::number(defaultValue)).toInt();
}

void MediaCenter::setConfig(const QString &name, const QString &value)
{
    QSettings settings;
    settings.setValue(name, value);
}

void MediaCenter::setConfig(const QString &name, int value)
{
    setConfig(name, QString::number(value));
}

QString MediaCenter::dataDir()
{
    return QDir::homePath() % QLatin1String("/.lemuri/") % QCoreApplication::applicationName();
}

QString MediaCenter::coverPath(const QString &absolutePath)
{
    return absolutePath % QLatin1String("/cover.jpg");
}

QString MediaCenter::coverThumbPath(const QString &absolutePath)
{
    return absolutePath % QLatin1String("/cover-thumb.jpg");
}

QString MediaCenter::pathMedia()
{
    // TODO ue xdg dirs
    return QDir::homePath() % "/Vídeos";
}

QString MediaCenter::pathPlaylist()
{
    return dataDir() % QLatin1String("/playlist.m3u");
}

QString MediaCenter::xapianDBPath()
{
    return dataDir() % QLatin1String("/index");
}

void MediaCenter::updateMediaCount(int mediaCount)
{
    emit mediaCountChanged();
}

void MediaCenter::indexFinished()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_indexReady) {
        m_indexReady = true;
        emit indexReadyChanged();
    }
}

void MediaCenter::showVideoTimeout()
{
    if (!m_showVideo && !m_configuring) {
        m_showVideo = true;
        emit showVideoChanged();
    }
}

void MediaCenter::close()
{
    m_view->close();
    QCoreApplication::quit();
}

void MediaCenter::nextMedia()
{
    m_playerModel->next();
}

void MediaCenter::createMediaFolders()
{
    QDir dataDirectory = dataDir();
    if (!dataDirectory.exists() && !dataDirectory.mkpath(dataDirectory.absolutePath())) {
        qWarning() << "Falha ao criar diretorio:" << dataDirectory.absolutePath();
    }
}
