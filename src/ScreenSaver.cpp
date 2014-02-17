#include "ScreenSaver.h"
#include "ScreenSaverInterface.h"

#include <QDebug>

ScreenSaver::ScreenSaver(QObject *parent) :
    QObject(parent),
    m_cookie(0)
{
    m_interface = new OrgFreedesktopScreenSaverInterface(QLatin1String("org.freedesktop.ScreenSaver"),
                                                         QLatin1String("/ScreenSaver"),
                                                         QDBusConnection::sessionBus(),
                                                         this);

    QDBusPendingReply<uint> reply = m_interface->Inhibit(QCoreApplication::applicationName(),
                                                         QLatin1String("Trabalhando"));
    reply.waitForFinished();
    if (reply.isValid()) {
        m_cookie = reply.value();
    } else {
        qWarning() << "Falha ao desabilitar o protetor de tela" << reply.error().message();
    }
}
