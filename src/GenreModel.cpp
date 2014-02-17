#include "GenreModel.h"

#include "IndexManager.h"
#include "IndexQuery.h"

#include <QDebug>

GenreModel::GenreModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_manager = new IndexManager(this);

    m_roleNames[RoleName]       = "roleName";
    m_roleNames[RoleEnabled]    = "roleEnabled";
    m_roleNames[RoleMediaCount] = "roleMediaCount";
}

QStringList GenreModel::genreFilters() const
{
    return m_genreFilters;
}

void GenreModel::setGenreFilters(const QStringList &genreFilters)
{
    if (m_genreFilters != genreFilters) {
        m_genreFilters = genreFilters;
        emit genreFiltersChanged();
        update();
    }
}

int GenreModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_genres.size();
    }
}

int GenreModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }
}

QModelIndex GenreModel::index(int row, int column, const QModelIndex &parent) const
{
    return !parent.isValid()
            && row >= 0 && row < m_genres.size()
            && column >= 0 && column < m_roleNames.size()
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex GenreModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant GenreModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() < m_genres.size()) {
        switch (role) {
        case RoleName:
            return m_genres.at(index.row()).name;
            break;
        case RoleEnabled:
            return !m_genreFilters.contains(m_genres.at(index.row()).name);
        case RoleMediaCount:
            return m_genres.at(index.row()).mediaCount;
            break;
        }
    }
    return QVariant();
}

QHash<int, QByteArray> GenreModel::roleNames() const
{
    return m_roleNames;
}

void GenreModel::update()
{
    if (m_genres.size()) {
        beginResetModel();
        m_genres.clear();
        endResetModel();
    }

    IndexQuery *query = m_manager->getGenres();
    connect(query, &IndexQuery::finished,
            this, &GenreModel::updateFinished);
}

void GenreModel::closeIndex()
{
    m_manager->close();
}

QString GenreModel::genreName(int row)
{
    if (row >= 0 && row < m_genres.size()) {
        return m_genres.at(row).name;
    }
    return QString();
}

void GenreModel::updateFinished()
{
    beginResetModel();
    IndexQuery *query = qobject_cast<IndexQuery*>(sender());
    m_genres = query->genres();
    endResetModel();

    QStringList remove;
    foreach (const QString &filtered, m_genreFilters) {
        bool found = false;
        for (int i = 0; i < m_genres.size(); ++i) {
            if (m_genres.at(i).name == filtered) {
                found = true;
                break;
            }
        }

        if (!found) {
            remove << filtered;
        }
    }

    // clear genres not present anymore
    if (!remove.isEmpty()) {
        foreach (const QString &genre, remove) {
            m_genreFilters.removeOne(genre);
            qDebug() << Q_FUNC_INFO << "removing genre:" << genre;
        }
        emit genreFiltersChanged();
    }

    query->deleteLater();
}

void GenreModel::toggleGenre(int row)
{
    if (row < 0 || row >= m_genres.size()) {
        return;
    }

    QString genre = m_genres.at(row).name;
    if (m_genreFilters.contains(genre)) {
        m_genreFilters.removeOne(genre);
    } else {
        m_genreFilters.append(genre);
    }
    emit dataChanged(index(row, 0), index(row, 0));

    emit genreFiltersChanged();
}
