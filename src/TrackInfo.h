#ifndef TRACKINFO_H
#define TRACKINFO_H

#include <QString>

class TrackInfo
{
public:
    QString album;
    QString artist;
    QString title;
    QString absoluteFilePath;
    bool isVideo;
    int trackNumber;
};

typedef QList<TrackInfo> TrackInfoList;

#endif // TRACKINFO_H
