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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QColor>
#include <QDialog>
#include <QString>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    /*
     * Constant for storing the settings paths.
     */
    const static QString    AUTO_ROTATE;

    const static QString    COLOR_SCALE;
    const static QString    COLOR_LOWER_THRESHOLD;
    const static QString    COLOR_UPPER_THRESHOLD;
    const static QString    COLOR_LOWER_COLOR;
    const static QString    COLOR_MID_COLOR;
    const static QString    COLOR_UPPER_COLOR;

    const static QString    INTERPOLATION_LEVEL;

    const static QString    LOG_FILE_NAME;
    const static QString    LOG_LOGGING_ENABLED;

    const static QString    METER;

    const static QString    TRIGGER;
    const static QString    TRIGGER_COMMAND;
    const static QString    TRIGGER_COUNT;
    const static QString    TRIGGER_VALUE;


    /*
     * Constants containing the default settings.
     */
    const static bool       AUTO_ROTATE_DEFAULT = false;

    const static bool       COLOR_SCALE_DEFAULT = false;
    const static int        COLOR_LOWER_THRESHOLD_DEFAULT = 60;
    const static int        COLOR_UPPER_THRESHOLD_DEFAULT = 95;
    const static QColor     COLOR_LOWER_COLOR_DEFAULT;
    const static QColor     COLOR_MID_COLOR_DEFAULT;
    const static QColor     COLOR_UPPER_COLOR_DEFAULT;

    const static int        INTERPOLATION_LEVEL_DEFAULT = 0;

    const static QString    LOG_FILE_NAME_DEFAULT;
    const static bool       LOG_LOGGING_ENABLED_DEFAULT = false;

    const static int        METER_DEFAULT = 0;

    const static QString    TRIGGER_COMMAND_DEFAULT;
    const static int        TRIGGER_COUNT_DEFAULT = 0;
    const static int        TRIGGER_VALUE_DEFAULT = 0;


    void accept();

private slots:
    void openLogFileSelectionDialog();
    void restoreDefaults();

private:
    Ui::SettingsDialog *ui;

};

#endif // SETTINGSDIALOG_H
