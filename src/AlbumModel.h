#ifndef ALBUMMODEL_H
#define ALBUMMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

#include "AlbumInfo.h"

class IndexManager;
class AlbumModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Role {
        RolePath,
        RoleArtist,
        RoleTitle,
        RoleDvd,
        RoleCover
    };

    explicit AlbumModel(QObject *parent = 0);

    Q_PROPERTY(QStringList genreFilters READ genreFilters WRITE setGenreFilters NOTIFY genreFiltersChanged)
    QStringList genreFilters() const;
    void setGenreFilters(const QStringList &genreFilters);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;

public Q_SLOTS:
    void update();
    void closeIndex();
    int indexForLetter(const QString &letter);

Q_SIGNALS:
    void genreFiltersChanged();

private:
    void updateFinished();

    AlbumInfoList m_albums;
    QHash<int, QByteArray> m_roleNames;
    QStringList m_genreFilters;
    IndexManager *m_manager;
};

#endif // ALBUMMODEL_H
