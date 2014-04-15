#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

#include "MediaCenter.h"

using namespace std;

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QCoreApplication::setOrganizationName("lemuri");
    QCoreApplication::setOrganizationDomain("lemuri.org");
    QCoreApplication::setApplicationName("lemuri-media-center");
    QCoreApplication::setApplicationVersion("1.0.0");

    QApplication app(argc, argv);

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QCoreApplication::installTranslator(&qtTranslator);
    MediaCenter mediaCenter;

    if (!mediaCenter.registerDBusService()) {
        return 2;
    }

    mediaCenter.show();

    return app.exec();
}
