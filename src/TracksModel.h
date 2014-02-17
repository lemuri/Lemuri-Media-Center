#ifndef TRACKSMODEL_H
#define TRACKSMODEL_H

#include <QAbstractItemModel>

#include "IndexManager.h"
#include "TrackInfo.h"

class TracksModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Role {
        RoleAlbum,
        RoleArtist,
        RoleTitle,
        RoleFilePath,
        RoleIsVideo
    };

    explicit TracksModel(QObject *parent = 0);

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
    void update(const QString &path);
    void closeIndex();

Q_SIGNALS:
    void genreFiltersChanged();

private:
    void updateFinished();

    TrackInfoList m_tracks;
    QHash<int, QByteArray> m_roleNames;
    QString m_lastPathSearched;
    QStringList m_genreFilters;
    IndexManager *m_manager;
};

#endif // TRACKSMODEL_H
