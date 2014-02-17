#include "DBusInterface.h"
#include "mediacenteradaptor.h"

#include <QDebug>

DBusInterface::DBusInterface(QObject *parent) :
    QObject(parent)
{
    (void) new MediaCenterAdaptor(this);
}

bool DBusInterface::registerService()
{
    if (!QDBusConnection::sessionBus().registerService(QLatin1String("org.lemuri.MediaCenter"))) {
        qWarning() << "unable to register service to dbus";
        return false;
    } else {
        if (!QDBusConnection::sessionBus().registerObject(QLatin1String("/"), this)) {
            qWarning() << "unable to register object to dbus";
            return false;
        }
    }
    return true;
}

void DBusInterface::UpdateIndex()
{
    emit updateIndex();
}
