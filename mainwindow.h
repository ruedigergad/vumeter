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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>
#include <QProcess>
#include <QStringList>

#include "pulseaudioadapter.h"
#include "volumemeterwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool getInterpolationEnabled();
    void updateVuMeter(const float *data);

signals:
    void updateVuMeterSignal(float);

private:
    Ui::MainWindow *ui;

    PulseAudioAdapter *adapter;

    int interpolationLevel;

    bool        loggingEnabled;
    QString     logFileName;
    QFile       *logFile;
    long        logIndex;

    int maxVol;
    int meter;

    QProcess *process;

    QStringList *sources;

    int     triggerCount;
    int     triggerCountSetting;
    int     triggerValue;
    QString triggerCommand;

    VolumeMeterWidget *volumeMeterWidget;


    void applySettings();
    void closeFile(QFile *file);
    void openLogFile();

private slots:
    void changeMeter();
    void processFinished();
    void resetMaxValue();
    void settingsDialog();
    void setLinearScale();
    void setDbScale();
    void sourceChanged(int source);
    void updateValues(float value);
};

#endif // MAINWINDOW_H
