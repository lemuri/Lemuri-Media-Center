#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include <QObject>
#include <QDBusContext>

class DBusInterface : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.lemuri.MediaCenter")
public:
    explicit DBusInterface(QObject *parent = 0);

    bool registerService();

signals:
    void updateIndex();

public:
    void UpdateIndex();
    void UpdateLicense();
};

#endif // DBUSINTERFACE_H
