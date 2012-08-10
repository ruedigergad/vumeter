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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QFileDialog>
#include <QPalette>
#include <QSettings>

const QString SettingsDialog::AUTO_ROTATE = "auto_rotate";

const QString SettingsDialog::COLOR_SCALE = "color_scale";
const QString SettingsDialog::COLOR_LOWER_THRESHOLD = "color/lower_threshold";
const QString SettingsDialog::COLOR_UPPER_THRESHOLD = "color/upper_threshold";
const QString SettingsDialog::COLOR_LOWER_COLOR = "color/lower_color";
const QString SettingsDialog::COLOR_MID_COLOR = "color/mid_color";
const QString SettingsDialog::COLOR_UPPER_COLOR = "color/upper_color";

const QString SettingsDialog::INTERPOLATION_LEVEL = "interpolation_level";

const QString SettingsDialog::LOG_FILE_NAME = "log/file_name";
const QString SettingsDialog::LOG_LOGGING_ENABLED = "log/enabled";

const QString SettingsDialog::METER = "meter";

const QString SettingsDialog::TRIGGER = "trigger";
const QString SettingsDialog::TRIGGER_COMMAND = SettingsDialog::TRIGGER + "/command";
const QString SettingsDialog::TRIGGER_COUNT = SettingsDialog::TRIGGER + "/count";
const QString SettingsDialog::TRIGGER_VALUE = SettingsDialog::TRIGGER + "/value";


const QColor SettingsDialog::COLOR_LOWER_COLOR_DEFAULT = QColor(Qt::green);
const QColor SettingsDialog::COLOR_MID_COLOR_DEFAULT = QColor(Qt::yellow);
const QColor SettingsDialog::COLOR_UPPER_COLOR_DEFAULT = QColor(Qt::red);

const QString SettingsDialog::LOG_FILE_NAME_DEFAULT = QObject::tr("Select File");

const QString SettingsDialog::TRIGGER_COMMAND_DEFAULT = "";


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings settings;

    ui->triggerValueEdit->setValue(settings.value(SettingsDialog::TRIGGER_VALUE,
                                                  SettingsDialog::TRIGGER_VALUE_DEFAULT).toInt());
    ui->triggerCommandEdit->setText(settings.value(SettingsDialog::TRIGGER_COMMAND,
                                                   SettingsDialog::TRIGGER_COMMAND_DEFAULT).toString());
    ui->triggerCountEdit->setValue(settings.value(SettingsDialog::TRIGGER_COUNT,
                                                  SettingsDialog::TRIGGER_COUNT_DEFAULT).toInt());

#ifdef Q_WS_MAEMO_5
    ui->autoRotateCheckBox->setChecked(settings.value(SettingsDialog::AUTO_ROTATE,
                                                      SettingsDialog::AUTO_ROTATE_DEFAULT).toBool());
#else
    ui->autoRotateCheckBox->close();
    ui->autoRotateLabel->close();
#endif

    ui->colorScaleSettingCheckBox->setChecked(settings.value(SettingsDialog::COLOR_SCALE,
                                                             SettingsDialog::COLOR_SCALE_DEFAULT).toBool());

    ui->lowerThresholdSpinBox->setValue(settings.value(SettingsDialog::COLOR_LOWER_THRESHOLD,
                                                       SettingsDialog::COLOR_LOWER_THRESHOLD_DEFAULT).toInt());
    ui->upperThresholdSpinBox->setValue(settings.value(SettingsDialog::COLOR_UPPER_THRESHOLD,
                                                       SettingsDialog::COLOR_UPPER_THRESHOLD_DEFAULT).toInt());

    ui->lowerColorPushButton->setColor(settings.value(SettingsDialog::COLOR_LOWER_COLOR,
                                                      SettingsDialog::COLOR_LOWER_COLOR_DEFAULT).value<QColor>());
    ui->midColorPushButton->setColor(settings.value(SettingsDialog::COLOR_MID_COLOR,
                                                    SettingsDialog::COLOR_MID_COLOR_DEFAULT).value<QColor>());
    ui->upperColorPushButton->setColor(settings.value(SettingsDialog::COLOR_UPPER_COLOR,
                                                      SettingsDialog::COLOR_UPPER_COLOR_DEFAULT).value<QColor>());

    ui->logDataSettingCheckBox->setChecked(settings.value(SettingsDialog::LOG_LOGGING_ENABLED,
                                                          SettingsDialog::LOG_LOGGING_ENABLED_DEFAULT).toBool());
    ui->logDataFileNameLineEdit->setText(settings.value(SettingsDialog::LOG_FILE_NAME,
                                                        SettingsDialog::LOG_FILE_NAME_DEFAULT).toString());
    connect(ui->logDataFileChooserPushButton, SIGNAL(clicked()), this, SLOT(openLogFileSelectionDialog()));

    ui->interpolationSpinBox->setValue(settings.value(SettingsDialog::INTERPOLATION_LEVEL,
                                                      SettingsDialog::INTERPOLATION_LEVEL_DEFAULT).toInt());

    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), SIGNAL(clicked()), this, SLOT(restoreDefaults()));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept(){
    QSettings settings;

    settings.setValue(SettingsDialog::TRIGGER_VALUE, ui->triggerValueEdit->value());
    settings.setValue(SettingsDialog::TRIGGER_COMMAND, ui->triggerCommandEdit->text());
    settings.setValue(SettingsDialog::TRIGGER_COUNT, ui->triggerCountEdit->value());

#ifdef Q_WS_MAEMO_5
    settings.setValue(SettingsDialog::AUTO_ROTATE, ui->autoRotateCheckBox->checkState());
#endif

    settings.setValue(SettingsDialog::COLOR_SCALE, ui->colorScaleSettingCheckBox->checkState());

    settings.setValue(SettingsDialog::COLOR_LOWER_THRESHOLD, ui->lowerThresholdSpinBox->value());
    settings.setValue(SettingsDialog::COLOR_UPPER_THRESHOLD, ui->upperThresholdSpinBox->value());

    settings.setValue(SettingsDialog::COLOR_LOWER_COLOR, ui->lowerColorPushButton->getColor());
    settings.setValue(SettingsDialog::COLOR_MID_COLOR, ui->midColorPushButton->getColor());
    settings.setValue(SettingsDialog::COLOR_UPPER_COLOR, ui->upperColorPushButton->getColor());

    settings.setValue(SettingsDialog::LOG_LOGGING_ENABLED, ui->logDataSettingCheckBox->isChecked());
    settings.setValue(SettingsDialog::LOG_FILE_NAME, ui->logDataFileNameLineEdit->text());

    settings.setValue(SettingsDialog::INTERPOLATION_LEVEL, ui->interpolationSpinBox->value());

    settings.sync();

    done(1);
}

void SettingsDialog::openLogFileSelectionDialog(){
    QString currentFile;

    if(ui->logDataFileNameLineEdit == NULL
       || ui->logDataFileNameLineEdit->text().compare(tr("Select File")) == 0){
#ifdef Q_WS_MAEMO_5
        currentFile = QDir::home().absolutePath() + "/MyDocs";
#else
        currentFile = QDir::home().absolutePath();
#endif
    }else{
        currentFile = ui->logDataFileNameLineEdit->text();
    }

    QString newFile = QFileDialog::getSaveFileName(this, "Select log file.", currentFile);
    if(newFile != NULL){
        ui->logDataFileNameLineEdit->setText(newFile);
    }
}

void SettingsDialog::restoreDefaults(){
    ui->triggerValueEdit->setValue(SettingsDialog::TRIGGER_VALUE_DEFAULT);
    ui->triggerCommandEdit->setText(SettingsDialog::TRIGGER_COMMAND_DEFAULT);
    ui->triggerCountEdit->setValue(SettingsDialog::TRIGGER_COUNT_DEFAULT);

#ifdef Q_WS_MAEMO_5
    ui->autoRotateCheckBox->setChecked(SettingsDialog::AUTO_ROTATE_DEFAULT);
#endif

    ui->colorScaleSettingCheckBox->setChecked(SettingsDialog::COLOR_SCALE_DEFAULT);

    ui->lowerThresholdSpinBox->setValue(SettingsDialog::COLOR_LOWER_THRESHOLD_DEFAULT);
    ui->upperThresholdSpinBox->setValue(SettingsDialog::COLOR_UPPER_THRESHOLD_DEFAULT);

    ui->lowerColorPushButton->setColor(SettingsDialog::COLOR_LOWER_COLOR_DEFAULT);
    ui->midColorPushButton->setColor(SettingsDialog::COLOR_MID_COLOR_DEFAULT);
    ui->upperColorPushButton->setColor(SettingsDialog::COLOR_UPPER_COLOR_DEFAULT);

    ui->logDataSettingCheckBox->setChecked(SettingsDialog::LOG_LOGGING_ENABLED_DEFAULT);
    ui->logDataFileNameLineEdit->setText(SettingsDialog::LOG_FILE_NAME_DEFAULT);

    ui->interpolationSpinBox->setValue(SettingsDialog::INTERPOLATION_LEVEL_DEFAULT);
}

