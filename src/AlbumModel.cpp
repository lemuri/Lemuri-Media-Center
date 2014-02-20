#include "AlbumModel.h"

#include "IndexQuery.h"
#include "IndexManager.h"
#include "MediaCenter.h"

#include <QTime>
#include <QFile>
#include <QStringBuilder>
#include <QDebug>

AlbumModel::AlbumModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_manager = new IndexManager(this);

    m_roleNames[RolePath]   = "rolePath";
    m_roleNames[RoleTitle]  = "roleTitle";
    m_roleNames[RoleArtist] = "roleArtist";
    m_roleNames[RoleDvd]    = "roleDvd";
    m_roleNames[RoleCover]  = "roleCover";
}

QStringList AlbumModel::genreFilters() const
{
    return m_genreFilters;
}

void AlbumModel::setGenreFilters(const QStringList &genreFilters)
{
    if (m_genreFilters != genreFilters) {
        m_genreFilters = genreFilters;
        emit genreFiltersChanged();
        update();
    }
}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_albums.size();
    }
}

int AlbumModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }
}

QModelIndex AlbumModel::index(int row, int column, const QModelIndex &parent) const
{
    return !parent.isValid()
            && row >= 0 && row < m_albums.size()
            && column >= 0 && column < m_roleNames.size()
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex AlbumModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() < m_albums.size()) {
        switch (role) {
        case RolePath:
            return m_albums.at(index.row()).path;
            break;
        case RoleTitle:
            return m_albums.at(index.row()).title;
            break;
        case RoleArtist:
            return m_albums.at(index.row()).artist;
            break;
        case RoleDvd:
            return m_albums.at(index.row()).isDvd;
            break;
        case RoleCover:
        {
            const QString &path = m_albums.at(index.row()).path;

            QString file = MediaCenter::coverThumbPath(path);
            if (QFile::exists(file)) {
                return QString("file://%1").arg(file);
            }

            file = MediaCenter::coverPath(path);
            if (QFile::exists(file)) {
                return QString("file://%1").arg(file);
            }
            return QString();
        }
        }
    }
    return QVariant();
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    return m_roleNames;
}

void AlbumModel::update()
{
    if (m_albums.size()) {
        beginResetModel();
        m_albums.clear();
        endResetModel();
    }

    IndexQuery *query = m_manager->getAlbums(m_genreFilters);
    connect(query, &IndexQuery::finished,
            this, &AlbumModel::updateFinished);
}

void AlbumModel::closeIndex()
{
    m_manager->close();
}

int AlbumModel::indexForLetter(const QString &letter)
{
    QTime timer;
    timer.start();

    if (!letter.isEmpty()) {
        for (int i = 0; i < m_albums.size(); ++i) {
            if (m_albums.at(i).artist.startsWith(letter, Qt::CaseInsensitive)) {
                qWarning() << Q_FUNC_INFO <<  timer.elapsed() << "ms" << i;
                return i;
            }
        }
    }
    qWarning() << Q_FUNC_INFO <<  timer.elapsed() << "ms";

    return -1;
}

void AlbumModel::updateFinished()
{
    beginResetModel();
    IndexQuery *query = qobject_cast<IndexQuery*>(sender());
    m_albums = query->albums();
    endResetModel();

    query->deleteLater();
}
