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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>

#include <QDate>
#include <QLineEdit>
#include <QSettings>
#include <QString>
#include <QTime>
#include <QTextStream>

#include "analogmeterwidget.h"
#include "horizontalvolumebarwidget.h"
#include "settingsdialog.h"
#include "volumelinechartwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    logFile = NULL;
    logIndex = 0;
    process = NULL;
    triggerCount = 0;
    volumeMeterWidget = NULL;

    QCoreApplication::setOrganizationName("VU Meter");
    QCoreApplication::setOrganizationDomain("https://garage.maemo.org/projects/vumeter");
    QCoreApplication::setApplicationName("VU Meter");

    QSettings settings;
    meter = settings.value(SettingsDialog::METER,
                           SettingsDialog::METER_DEFAULT).toInt();
    interpolationLevel = settings.value(SettingsDialog::INTERPOLATION_LEVEL,
                                        SettingsDialog::INTERPOLATION_LEVEL_DEFAULT).toInt();

    ui->setupUi(this);

    adapter = new PulseAudioAdapter();

    changeMeter();
    setDbScale();

    ui->currentVolume->setNum(0);
    ui->maximumVolume->setNum(0);

#if defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    sources = new QStringList();
    sources->append("Microphone");
    sources->append("Bluetooth");
    sources->append("Monitor");
#else
    sources = adapter->getAvailableSources();
#endif
    ui->sourcesComboBox->addItems(*sources);

    /*
#ifdef Q_WS_MAEMO_5
    ui->sourcesComboBox->setEditable(true);
    ui->sourcesComboBox->lineEdit()->setAlignment(Qt::AlignHCenter);
    ui->sourcesComboBox->lineEdit()->setReadOnly(true);
#endif
    */

    connect(ui->sourcesComboBox, SIGNAL(activated(int)), this, SLOT(sourceChanged(int)));

#if defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    adapter->connectAudioStream("source.hw0");
    //connect_to_vu_meter_stream("source.hw0", this, pow(2, interpolationLevel));
#else
    adapter->connectAudioStream(sources->at(0));
//    connect_to_vu_meter_stream(sources->at(0).toLocal8Bit().constData(), this, pow(2, interpolationLevel));
#endif

    connect(adapter, SIGNAL(audioDataReceived(float)), this, SLOT(updateValues(float)), Qt::QueuedConnection);
    connect(ui->maxLabel, SIGNAL(linkActivated(QString)), this, SLOT(resetMaxValue()));

    connect(ui->radioButtonDb, SIGNAL(clicked()), this, SLOT(setDbScale()));
    connect(ui->radioButtonLinear, SIGNAL(clicked()), this, SLOT(setLinearScale()));

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settingsDialog()));
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue(SettingsDialog::METER, meter);
    settings.sync();

    delete ui;

    if(process != NULL){
        process->terminate();
        delete process;
    }

    if(sources != NULL){
        delete sources;
    }

    delete volumeMeterWidget;
    delete adapter;

    closeFile(logFile);
}

void MainWindow::applySettings(){
    QSettings settings;
    triggerValue = settings.value(SettingsDialog::TRIGGER_VALUE,
                                  SettingsDialog::TRIGGER_VALUE_DEFAULT).toInt();
    triggerCommand = settings.value(SettingsDialog::TRIGGER_COMMAND,
                                    SettingsDialog::TRIGGER_COMMAND_DEFAULT).toString();
    triggerCountSetting = settings.value(SettingsDialog::TRIGGER_COUNT,
                                         SettingsDialog::TRIGGER_COUNT_DEFAULT).toInt();

#ifdef Q_WS_MAEMO_5
    setAttribute(Qt::WA_Maemo5AutoOrientation,
                 settings.value(SettingsDialog::AUTO_ROTATE,
                                SettingsDialog::AUTO_ROTATE_DEFAULT).toBool());
#endif

    volumeMeterWidget->setTriggerValue(triggerValue);
    volumeMeterWidget->setUseColoredScale(settings.value(SettingsDialog::COLOR_SCALE, false).toBool());
    volumeMeterWidget->setColors(settings.value(SettingsDialog::COLOR_LOWER_COLOR,
                                                SettingsDialog::COLOR_LOWER_COLOR_DEFAULT).value<QColor>(),
                                 settings.value(SettingsDialog::COLOR_MID_COLOR,
                                                SettingsDialog::COLOR_MID_COLOR_DEFAULT).value<QColor>(),
                                 settings.value(SettingsDialog::COLOR_UPPER_COLOR,
                                                SettingsDialog::COLOR_UPPER_COLOR_DEFAULT).value<QColor>());
    volumeMeterWidget->setColorThresholds(settings.value(SettingsDialog::COLOR_LOWER_THRESHOLD,
                                                         SettingsDialog::COLOR_LOWER_THRESHOLD_DEFAULT).toInt(),
                                          settings.value(SettingsDialog::COLOR_UPPER_THRESHOLD,
                                                         SettingsDialog::COLOR_UPPER_THRESHOLD_DEFAULT).toInt());

    loggingEnabled = settings.value(SettingsDialog::LOG_LOGGING_ENABLED,
                                    SettingsDialog::LOG_LOGGING_ENABLED_DEFAULT).toBool();
    logFileName = settings.value(SettingsDialog::LOG_FILE_NAME,
                                 SettingsDialog::LOG_FILE_NAME_DEFAULT).toString();

    if(loggingEnabled){
        if(logFile != NULL && logFile->fileName().compare(logFileName) != 0){
            closeFile(logFile);
        }

        if(logFile == NULL && logFileName.compare(SettingsDialog::LOG_FILE_NAME_DEFAULT) != 0){
            logIndex = 0;
            logFile = new QFile(logFileName);
            logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        }
    }

    int ipolLevel = settings.value(SettingsDialog::INTERPOLATION_LEVEL,
                                   SettingsDialog::INTERPOLATION_LEVEL_DEFAULT).toInt();
    if(interpolationLevel != ipolLevel){
        interpolationLevel = ipolLevel;
        sourceChanged(ui->sourcesComboBox->currentIndex());
    }
}

void MainWindow::closeFile(QFile *file){
    if(file != NULL && file->isOpen()){
        qDebug("Closing file %s.", file->fileName().toLocal8Bit().data());
        file->flush();
        file->close();
        delete file;
        file = NULL;
        qDebug("File closed.");
    }
}

void MainWindow::changeMeter(){
    if(volumeMeterWidget != NULL){
        disconnect(volumeMeterWidget, SIGNAL(clicked()), this, SLOT(changeMeter()));
        delete volumeMeterWidget;
        meter = ++meter % 3;
    }

    switch(meter){
    case 0:
        volumeMeterWidget = new AnalogMeterWidget(this);
        break;
    case 1:
        volumeMeterWidget = new HorizontalVolumeBarWidget(this);
        break;
    case 2:
        volumeMeterWidget = new VolumeLineChartWidget(this);
    }

    volumeMeterWidget->setDbUnit(ui->radioButtonDb->isChecked());
    ui->volumeMeterLayout->addWidget(volumeMeterWidget);
    applySettings();
    connect(volumeMeterWidget, SIGNAL(clicked()), this, SLOT(changeMeter()));
}

bool MainWindow::getInterpolationEnabled(){
    return interpolationLevel != 0;
}

void MainWindow::processFinished(){
    if(process != NULL){
        process->deleteLater();
        process = NULL;
    }
}

void MainWindow::resetMaxValue(){
    maxVol = 0;
    ui->maximumVolume->setNum(maxVol);
}

void MainWindow::settingsDialog(){
    SettingsDialog dialog(this);

    if(dialog.exec()){
        applySettings();
    }
}

void MainWindow::setLinearScale(){
    volumeMeterWidget->setDbUnit(false);
    resetMaxValue();
}

void MainWindow::setDbScale(){
    volumeMeterWidget->setDbUnit(true);
    resetMaxValue();
}

void MainWindow::sourceChanged(int index){
    adapter->disconnectAudioStream();

    resetMaxValue();
    emit updateVuMeterSignal(0.);

#if defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    switch(index){
    case 0:
        adapter->connectAudioStream("source.hw0");
//        connect_to_vu_meter_stream("source.hw0", this, pow(2, interpolationLevel));
        break;
    case 1:
        adapter->connectAudioStream("source.hw1");
//        connect_to_vu_meter_stream("source.hw1", this, pow(2, interpolationLevel));
        break;
    case 2:
        adapter->connectAudioStream("sink.hw0.monitor");
//        connect_to_vu_meter_stream("sink.hw0.monitor", this, pow(2, interpolationLevel));
        break;
    default:
        qDebug("Unknown source!");
    }
#else
    adapter->connectAudioStream(sources->at(index));
//    connect_to_vu_meter_stream(sources->at(index).toLocal8Bit().constData(), this, pow(2, interpolationLevel));
#endif
}

void MainWindow::updateValues(float value){
    int vol = 0;

    if(ui->radioButtonDb->isChecked()){
        vol = 100 + adapter->linearVolToDb(value);
    }else if(ui->radioButtonLinear->isChecked()){
        vol = 100 * value;
    }

    if(vol < 0){
        vol = 0;
    }

    if(loggingEnabled){
        QTextStream out(logFile);
        out << logIndex << ";"
                << QDate::currentDate().toString("yyyy-MM-dd") << ";"
                << QTime::currentTime().toString("hh:mm:ss.zzz") << ";"
                << vol << ";"
                << ((ui->radioButtonDb->isChecked()) ? "dB" : "Linear") << "\n";
        logIndex++;
    }

    if(triggerValue > 0){
        if(vol > triggerValue && process == NULL){
            if(triggerCount >= triggerCountSetting){
                process = new QProcess();
                connect(process, SIGNAL(finished(int)), this, SLOT(processFinished()));
                process->start(triggerCommand);
                triggerCount = 0;
            }else{
                triggerCount++;
            }
        }else{
            triggerCount = 0;
        }
    }

    volumeMeterWidget->updateValue(vol);
    ui->currentVolume->setNum(vol);
    if(maxVol < vol){
        maxVol = vol;
        ui->maximumVolume->setNum(vol);
    }
}

void MainWindow::updateVuMeter(const float *data){
    float vol = *data;

    emit updateVuMeterSignal(vol);
}


