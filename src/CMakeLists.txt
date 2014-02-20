set(CMAKE_AUTOMOC ON)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB QML_SRC qml/*.qml qml/*.js)

set(lmc_SRCS
    ${QML_SRC}
    CoverProvider.cpp
    IconProvider.cpp
    AlbumInfo.h
    AlbumModel.cpp
    GenreInfo.h
    GenreModel.cpp
    TrackInfo.h
    TracksModel.cpp
    PlayerAndModel.cpp
    MusicImporter.cpp
    IndexQuery.cpp
    IndexWorker.cpp
    IndexManager.cpp
    BackgroundModel.cpp
    KeyManager.cpp
    ScreenSaver.cpp
    DBusInterface.cpp
    MediaCenter.cpp
    themoviedb.cpp
    mimefileresolver.cpp
    main.cpp
)

qt5_add_resources(lmc_SRCS
    application.qrc
    OPTIONS -compress 3 -threshold 3
)

qt5_add_dbus_adaptor(lmc_SRCS
    org.lemuri.MediaCenter.xml
    DBusInterface.h
    DBusInterface
)

set_property(SOURCE org.freedesktop.ScreenSaver.xml PROPERTY NO_NAMESPACE true)
qt5_add_dbus_interface(lmc_SRCS org.freedesktop.ScreenSaver.xml ScreenSaverInterface)

add_executable(lemuri-media-center
    ${lmc_SRCS}
)

qt5_use_modules(lemuri-media-center Multimedia Quick DBus Widgets)
target_link_libraries(lemuri-media-center
    ${XAPIAN_LIBRARIES}
    ${TAGLIB_LIBRARIES}
)

install(TARGETS lemuri-media-center DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
