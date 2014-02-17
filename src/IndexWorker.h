#ifndef INDEXWORKER_H
#define INDEXWORKER_H

#include <QObject>
#include <QStringList>

#undef slots
#include <xapian.h>
#define slots

#include "GenreInfo.h"
#include "AlbumInfo.h"
#include "TrackInfo.h"

class IndexWorker : public QObject
{
    Q_OBJECT
public:
    IndexWorker();

signals:
    void failed(quint64 id, const QString &message);
    void facetedSearchFinished(quint64 id, const QStringList &facets);
    void getGenresFinished(quint64 id, const GenreInfoList &genres);
    void getAlbumsFinished(quint64 id, const AlbumInfoList &albuns);
    void getTracksFinished(quint64 id, const TrackInfoList &tracks);

public Q_SLOTS:
    void close();
    void facetedSearch(quint64 id, int slot, const QStringList &genres);
    void getTracks(quint64 id, const QString &albumPath, const QStringList &genreFilters);

private:
    bool open(quint64 id);
    Xapian::Query filter(const QStringList &genres) const;

    Xapian::Database *m_database;
};

#endif // INDEXWORKER_H
