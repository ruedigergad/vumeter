/*
 *  Copyright 2010 Ruediger Gad
 *
 *  This file is part of vumeter.
 *
 *  vumeter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  vumeter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with vumeter.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui/QApplication>

#ifdef MEEGO_EDITION_HARMATTAN
#include <applauncherd/MDeclarativeCache>
#include <QtDeclarative>
#include "pulseaudioadapter.h"
#include "volumemeterwidgetsqml.h"
#endif

#ifdef Q_WS_MAEMO_5
#include "mainwindow.h"
#endif


#ifdef MEEGO_EDITION_HARMATTAN
Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QApplication *app = MDeclarativeCache::qApplication(argc, argv);
    QDeclarativeView *view = MDeclarativeCache::qDeclarativeView();

    qmlRegisterType<PulseAudioAdapter>("vumeter", 1, 0, "PulseAudio");

    qmlRegisterType<VolumeMeter>("vumeter", 1, 0, "VolumeMeter");
    qmlRegisterType<AnalogMeter>("vumeter", 1, 0, "AnalogMeter");
    qmlRegisterType<HorizontalVolumeBar>("vumeter", 1, 0, "HorizontalVolumeBar");
    qmlRegisterType<VolumeLineChart>("vumeter", 1, 0, "VolumeLineChart");

    /*
    One option is to embedd QML files in the binary via resources.
    view.setSource(QUrl("qrc:/qml/main.qml"));
    */

    /*
     * Use external files instead of QML files embedded in the binary (see above).
     * This should significantly speed up the development and testing.
     */
    view->setSource(QUrl("/opt/vumeter/qml/main.qml"));
    view->showFullScreen();
    return app->exec();
}
#else
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
#endif
