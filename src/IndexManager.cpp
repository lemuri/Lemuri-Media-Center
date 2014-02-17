#include "IndexManager.h"

#include "MusicImporter.h"
#include "IndexWorker.h"
#include "IndexQuery.h"

IndexManager::IndexManager(QObject *parent) :
    QObject(parent),
    m_last(0)
{
    qRegisterMetaType<GenreInfoList>("GenreInfoList");
    qRegisterMetaType<TrackInfoList>("TrackInfoList");
    qRegisterMetaType<AlbumInfoList>("AlbumInfoList");

    m_workerThead = new QThread(this);
    m_worker = new IndexWorker;
    m_worker->moveToThread(m_workerThead);
    connect(m_worker, &IndexWorker::facetedSearchFinished,
            this, &IndexManager::facetedSearchFinished);
    connect(m_worker, &IndexWorker::getTracksFinished,
            this, &IndexManager::getTracksFinished);
    connect(m_worker, &IndexWorker::getAlbumsFinished,
            this, &IndexManager::getAlbumsFinished);
    connect(m_worker, &IndexWorker::getGenresFinished,
            this, &IndexManager::getGenresFinished);
    m_workerThead->start();
}

IndexManager::~IndexManager()
{
    m_workerThead->quit();
    m_workerThead->wait();
    delete m_worker;
}

void IndexManager::close()
{
    QMetaObject::invokeMethod(m_worker,
                              "close",
                              Qt::QueuedConnection);
}

IndexQuery *IndexManager::facetedSearch(int slotno, const QStringList &genreFilters)
{
    quint64 id = getId();
    IndexQuery *query = new IndexQuery;
    m_queries[id] = query;
    QMetaObject::invokeMethod(m_worker,
                              "facetedSearch",
                              Qt::QueuedConnection,
                              Q_ARG(quint64, id),
                              Q_ARG(int, slotno),
                              Q_ARG(QStringList, genreFilters));
    return query;
}

IndexQuery *IndexManager::getTracks(const QString &album, const QStringList &genreFilters)
{
    quint64 id = getId();
    IndexQuery *query = new IndexQuery;
    m_queries[id] = query;
    QMetaObject::invokeMethod(m_worker,
                              "getTracks",
                              Qt::QueuedConnection,
                              Q_ARG(quint64, id),
                              Q_ARG(QString, album),
                              Q_ARG(QStringList, genreFilters));
    return query;
}

IndexQuery *IndexManager::getHistory(const QDate &startDate, const QDate &endDate)
{
    quint64 id = getId();
    IndexQuery *query = new IndexQuery;
    m_queries[id] = query;
    QMetaObject::invokeMethod(m_worker,
                              "getHistory",
                              Qt::QueuedConnection,
                              Q_ARG(quint64, id),
                              Q_ARG(QDate, startDate),
                              Q_ARG(QDate, endDate));
    return query;
}

void IndexManager::facetedSearchFinished(quint64 id, const QStringList &facets)
{
    IndexQuery *query = m_queries.value(id);
    if (query) {
        query->setFacets(facets);
    }
}

void IndexManager::getGenresFinished(quint64 id, const GenreInfoList &genres)
{
    IndexQuery *query = m_queries.value(id);
    if (query) {
        query->setGenres(genres);
    }
}

void IndexManager::getAlbumsFinished(quint64 id, const AlbumInfoList &albums)
{
    IndexQuery *query = m_queries.value(id);
    if (query) {
        query->setAlbums(albums);
    }
}

void IndexManager::getTracksFinished(quint64 id, const TrackInfoList &tracks)
{
    IndexQuery *query = m_queries.value(id);
    if (query) {
        query->setTracks(tracks);
    }
}

quint64 IndexManager::getId()
{
    return (++m_last) % UINT_MAX;
}

IndexQuery *IndexManager::getAlbums(const QStringList &genreFilters)
{
    return facetedSearch(MusicImporter::AbsolutePath, genreFilters);
}

IndexQuery *IndexManager::getGenres()
{
    return facetedSearch(MusicImporter::Genre);
}
