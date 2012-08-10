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

#ifndef VOLUMELINECHARTWIDGET_H
#define VOLUMELINECHARTWIDGET_H

#include <QList>
#include <QPainter>
#include <QPaintEvent>

#include "volumemeterwidget.h"

/**
 * Widget for displaying the volume in a line chart.
 */
class VolumeLineChartWidget : public VolumeMeterWidget
{

public:
    VolumeLineChartWidget(QWidget *parent);
    ~VolumeLineChartWidget();

    void setDbUnit(bool b);

protected:
    void draw(QPainter *painter);
    void drawTriggerIndicator(QPainter *painter);
    void resizeEvent(QResizeEvent *event);

private:
    void prepareBackground();
    /**
     * Initialize QList for volume values.
     * @see VolumeLineChartWidget#volumeValues
     */
    void initValues();

    QList<float> *volumeValues;
};

#endif // VOLUMELINECHARTWIDGET_H
