#ifndef ALBUMINFO_H
#define ALBUMINFO_H

#include <QString>

class AlbumInfo
{
public:
    QString path;
    QString artist;
    QString title;
    bool isDvd;
};

typedef QList<AlbumInfo> AlbumInfoList;

#endif // ALBUMINFO_H
