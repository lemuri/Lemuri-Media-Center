#include "BackgroundModel.h"
#include "MediaCenter.h"

#include <QStringBuilder>
#include <QDirIterator>
#include <QDateTime>
#include <QDebug>

BackgroundModel::BackgroundModel(QObject *parent) :
    QStringListModel(parent)
{
    qsrand(QTime::currentTime().msec());
    // TODO wallpaper dir
    QString path/* = QDir::homePath()*/;
    QStringList images;
    QDirIterator it(path,  QDir::Files);
    while (it.hasNext()) {
        QString image = it.next();
        switch (qrand() % 2) {
        case 0:
            images.append(image);
            break;
        case 1:
            images.prepend(image);
            break;
        default:
            images.insert(images.count() / 2, image);
            break;
        }
    }
    setStringList(images);
}

int BackgroundModel::count() const
{
    return rowCount();
}

QString BackgroundModel::get(int row) const
{
    QModelIndex modelIndex = index(row, 0);
    if (modelIndex.isValid()) {
        return modelIndex.data().toString();
    }
    return QString();
}
