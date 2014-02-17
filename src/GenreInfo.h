#ifndef GENREINFO_H
#define GENREINFO_H

#include <QString>

class GenreInfo
{
public:
    QString name;
    quint64 mediaCount;
};

typedef QList<GenreInfo> GenreInfoList;

#endif // GENREINFO_H
