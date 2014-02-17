#ifndef SCREENSAVER_H
#define SCREENSAVER_H

#include <QObject>

class OrgFreedesktopScreenSaverInterface;
class ScreenSaver : public QObject
{
    Q_OBJECT
public:
    explicit ScreenSaver(QObject *parent = 0);

private:
    OrgFreedesktopScreenSaverInterface *m_interface;
    quint32 m_cookie;
};

#endif // SCREENSAVER_H
