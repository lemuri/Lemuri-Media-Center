#include "CoverProvider.h"

#include "MediaCenter.h"

#include <QDebug>

CoverProvider::CoverProvider() :
    QQuickImageProvider(Image),
    m_fallback(QLatin1String(":/images/cd.png"))
{
}

QImage CoverProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QImage ret(MediaCenter::coverPath(id));
    if (ret.isNull()) {
        ret = m_fallback;
    }

    if (!ret.isNull() && requestedSize.isValid()) {
        // Load the requested image, scalling to the
        // requested size
        QSize scaleSize = requestedSize;
        if (scaleSize.height() <= 0) {
            scaleSize.setHeight(scaleSize.width());
        } else if (scaleSize.width() <= 0) {
            scaleSize.setWidth(scaleSize.height());
        }

        ret = ret.scaled(scaleSize, Qt::KeepAspectRatio);
    }

    if (size) {
        *size = requestedSize;
    }

    return ret;
}
