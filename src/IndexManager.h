#ifndef INDEXMANAGER_H
#define INDEXMANAGER_H

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QHash>

#include "TrackInfo.h"
#include "AlbumInfo.h"
#include "GenreInfo.h"

class IndexQuery;
class IndexWorker;
class IndexManager : public QObject
{
    Q_OBJECT
public:
    explicit IndexManager(QObject *parent = 0);
    ~IndexManager();

    void close();
    IndexQuery *facetedSearch(int slotno, const QStringList &genreFilters = QStringList());
    IndexQuery *getGenres();
    IndexQuery *getAlbums(const QStringList &genreFilters);
    IndexQuery *getTracks(const QString &album, const QStringList &genreFilters);
    IndexQuery *getHistory(const QDate &startDate, const QDate &endDate);

private slots:
    void facetedSearchFinished(quint64 id, const QStringList &facets);
    void getGenresFinished(quint64 id, const GenreInfoList &genres);
    void getAlbumsFinished(quint64 id, const AlbumInfoList &albums);
    void getTracksFinished(quint64 id, const TrackInfoList &tracks);

private:
    quint64 getId();
    IndexWorker *m_worker;
    QThread *m_workerThead;
    QHash<quint64, IndexQuery*> m_queries;
    quint64 m_last;
};

#endif // INDEXMANAGER_H
