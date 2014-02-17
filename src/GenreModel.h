#ifndef GENREMODEL_H
#define GENREMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

#include "GenreInfo.h"

class IndexManager;
class GenreModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Role {
        RoleName,
        RoleEnabled,
        RoleMediaCount
    };

    explicit GenreModel(QObject *parent = 0);

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
    QString genreName(int row);
    void toggleGenre(int row);

Q_SIGNALS:
    void genreFiltersChanged();

private:
    void updateFinished();

    GenreInfoList m_genres;
    QHash<int, QByteArray> m_roleNames;
    QString m_lastPathSearched;
    QStringList m_genreFilters;
    IndexManager *m_manager;
};

#endif // GENREMODEL_H
