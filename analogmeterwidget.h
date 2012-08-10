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

#ifndef ANALOGMETERWIDGET_H
#define ANALOGMETERWIDGET_H

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>

#include <volumemeterwidget.h>

/**
 * Analog display for volume values.
 *
 * @see VolumeMeterWidget
 */
class AnalogMeterWidget : public VolumeMeterWidget
{

public:
    AnalogMeterWidget(QWidget *parent);

protected:

    void draw(QPainter *painter);
    void drawBackground(QPainter *painter);
    void drawTriggerIndicator(QPainter *painter);
    void initPainter(QPainter *painter);

private:
    void prepareBackground();
};

#endif // ANALOGMETERWIDGET_H
