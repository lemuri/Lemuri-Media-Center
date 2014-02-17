#include "IndexQuery.h"

IndexQuery::IndexQuery()
{
}

GenreInfoList IndexQuery::genres() const
{
    return m_genres;
}

QStringList IndexQuery::facets() const
{
    return m_facets;
}

TrackInfoList IndexQuery::tracks() const
{
    return m_tracks;
}

AlbumInfoList IndexQuery::albums() const
{
    return m_albums;
}

QString IndexQuery::error() const
{
    return m_error;
}

void IndexQuery::setFacets(const QStringList &facets)
{
    m_facets = facets;
    emit finished();
}

void IndexQuery::setGenres(const GenreInfoList &genres)
{
    m_genres = genres;
    emit finished();
}

void IndexQuery::setTracks(const TrackInfoList &tracks)
{
    m_tracks = tracks;
    emit finished();
}

void IndexQuery::setAlbums(const AlbumInfoList &albums)
{
    m_albums = albums;
    emit finished();
}

void IndexQuery::setError(const QString &error)
{
    m_error = error;
    emit finished();
}
