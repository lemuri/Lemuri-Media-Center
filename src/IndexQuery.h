#ifndef INDEXQUERY_H
#define INDEXQUERY_H

#include <QObject>
#include <QStringList>

#include "GenreInfo.h"
#include "TrackInfo.h"
#include "AlbumInfo.h"

class IndexQuery : public QObject
{
    Q_OBJECT
public:
    IndexQuery();

    QStringList facets() const;
    GenreInfoList genres() const;
    TrackInfoList tracks() const;
    AlbumInfoList albums() const;
    QString error() const;

Q_SIGNALS:
    void finished();

protected:
    void setFacets(const QStringList &facets);
    void setGenres(const GenreInfoList &genres);
    void setTracks(const TrackInfoList &tracks);
    void setAlbums(const AlbumInfoList &albums);
    void setError(const QString &error);

private:
    friend class IndexManager;
    QStringList m_facets;
    GenreInfoList m_genres;
    AlbumInfoList m_albums;
    TrackInfoList m_tracks;
    QString m_error;
};

#endif // INDEXQUERY_H
