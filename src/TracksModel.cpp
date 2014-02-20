#include "TracksModel.h"

#include "IndexQuery.h"
#include "MediaCenter.h"

#include <QFileInfo>

TracksModel::TracksModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_manager = new IndexManager(this);

    m_roleNames[RoleAlbum]    = "roleAlbum";
    m_roleNames[RoleTitle]    = "roleTitle";
    m_roleNames[RoleArtist]   = "roleArtist";
    m_roleNames[RoleFilePath] = "roleFilePath";
    m_roleNames[RoleIsVideo]  = "roleIsVideo";
    m_roleNames[RoleCover]  = "roleCover";
}

QStringList TracksModel::genreFilters() const
{
    return m_genreFilters;
}

void TracksModel::setGenreFilters(const QStringList &genreFilters)
{
    if (m_genreFilters != genreFilters) {
        m_genreFilters = genreFilters;
        emit genreFiltersChanged();
        if (!m_lastPathSearched.isEmpty()) {
            update(m_lastPathSearched);
        }
    }
}

int TracksModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_tracks.size();
    }
}

int TracksModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }
}

QModelIndex TracksModel::index(int row, int column, const QModelIndex &parent) const
{
    return !parent.isValid()
            && row >= 0 && row < m_tracks.size()
            && column >= 0 && column < m_roleNames.size()
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex TracksModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant TracksModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() < m_tracks.size()) {
        switch (role) {
        case RoleAlbum:
            return m_tracks.at(index.row()).album;
            break;
        case RoleTitle:
            return m_tracks.at(index.row()).title;
            break;
        case RoleArtist:
            return m_tracks.at(index.row()).artist;
            break;
        case RoleFilePath:
            return m_tracks.at(index.row()).absoluteFilePath;
            break;
        case RoleIsVideo:
            return m_tracks.at(index.row()).isVideo;
            break;
        case RoleCover:
        {
            const QString &file = MediaCenter::coverPath(m_tracks.at(index.row()).absoluteFilePath);
            if (QFile::exists(file)) {
                return QString("file://%1").arg(file);
            }
            return QString();
        }
        }
    }
    return QVariant();
}

QHash<int, QByteArray> TracksModel::roleNames() const
{
    return m_roleNames;
}

void TracksModel::update(const QString &path)
{
    if (m_tracks.size()) {
        beginResetModel();
        m_tracks.clear();
        endResetModel();
    }

    m_lastPathSearched = path;
    IndexQuery *query = m_manager->getTracks(path, m_genreFilters);
    connect(query, &IndexQuery::finished,
            this, &TracksModel::updateFinished);
}

void TracksModel::getMovies()
{
    if (m_tracks.size()) {
        beginResetModel();
        m_tracks.clear();
        endResetModel();
    }

    m_lastPathSearched = QString();
    IndexQuery *query = m_manager->getTracks(QString(), m_genreFilters);
    connect(query, &IndexQuery::finished,
            this, &TracksModel::updateFinished);
}

void TracksModel::closeIndex()
{
    m_manager->close();
}

void TracksModel::updateFinished()
{
    beginResetModel();
    IndexQuery *query = qobject_cast<IndexQuery*>(sender());
    m_tracks = query->tracks();
    endResetModel();

    query->deleteLater();
}
