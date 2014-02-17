#ifndef COVERPROVIDER_H
#define COVERPROVIDER_H

#include <QtQuick/QQuickImageProvider>

class CoverProvider : public QQuickImageProvider
{
public:
    CoverProvider();

    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QImage m_fallback;
};

#endif // COVERPROVIDER_H
