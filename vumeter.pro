#-------------------------------------------------
#
# Project created by QtCreator 2010-11-16T17:51:48
#
#-------------------------------------------------

CONFIG += link_pkgconfig
PKGCONFIG += libpulse

QT       += core gui opengl

TARGET = vumeter
TEMPLATE = app

FORMS += forms/mainwindow.ui \
    forms/settingsdialog.ui

HEADERS += pa_utils.h \
    pa_context_helper.h \
    pa_streams_helper.h \
    volumemeterwidget.h \
    analogmeterwidget.h \
    horizontalvolumebarwidget.h \
    volumelinechartwidget.h \
    colorchooserbutton.h \
    volumemeterwidgetsqml.h \
    pulseaudioadapter.h

SOURCES += main.cpp \
    pa_utils.cpp \
    pa_context_helper.cpp \
    pa_streams_helper.cpp \
    volumemeterwidget.cpp \
    analogmeterwidget.cpp \
    horizontalvolumebarwidget.cpp \
    volumelinechartwidget.cpp \
    colorchooserbutton.cpp \
    pulseaudioadapter.cpp

OTHER_FILES += debian/changelog \
    debian/compat \
    debian/control \
    debian/copyright \
    debian/postinst \
    debian/prerm \
    debian/rules

linux-g++-maemo5 {
    QT += maemo5

    HEADERS += mainwindow.h \
        settingsdialog.h

    SOURCES += mainwindow.cpp \
        settingsdialog.cpp

    INSTALLS += target desktop icon48
    target.path = /opt/maemo/usr/bin
    desktop.path = /usr/share/applications/hildon
    desktop.files += res/fremantle/usr/share/applications/hildon/vumeter.desktop
    icon48.path = /usr/share/icons/hicolor/48x48/apps
    icon48.files += res/fremantle/usr/share/icons/hicolor/48x48/hildon/vumeter.png
}

# Harmattan specific stuff goes here.
exists($$QMAKE_INCDIR_QT"/../applauncherd/MDeclarativeCache"): {
    MEEGO_VERSION_MAJOR     = 1
    MEEGO_VERSION_MINOR     = 2
    MEEGO_VERSION_PATCH     = 0
    MEEGO_EDITION           = harmattan

    DEFINES += MEEGO_EDITION_HARMATTAN
    QT+= declarative

# As we use QML anyways we do not need any forms.
    FORMS =

# One option is to ship the QML stuff is to embed it in the binary.
    OTHER_FILES += qml/main.qml \
      qml/AboutDialog.qml \
      vumeter_100x100.png
#        qml.qrc
#    RESOURCES += qml.qrc

    target.path = /opt/vumeter/bin
    desktop.path = /usr/share/applications
    desktop.files += res/harmattan/vumeter.desktop
    icon_svg.path = /usr/share/icons/hicolor/scalable/apps
    icon_svg.files += res/svg/vumeter.svg
    icon.path = /usr/share/icons/hicolor/64x64/apps
    icon.files += res/harmattan/vumeter.png
    qml.path = /opt/vumeter/qml
    qml.files += qml/main.qml \
      qml/AboutDialog.qml \
      vumeter_100x100.png

    INSTALLS += target desktop icon_svg icon qml

    # enable booster
    CONFIG += qdeclarative-boostable
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic
}

