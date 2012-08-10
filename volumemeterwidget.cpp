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

#include "volumemeterwidget.h"

#include <QPen>
#include <QRect>

VolumeMeterWidget::VolumeMeterWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    currentValue = 0;
    triggerIndicatorColor = Qt::darkYellow;
    triggerValue = 0;
    background = NULL;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(this, SIGNAL(redraw()), this, SLOT(update()));
}

VolumeMeterWidget::~VolumeMeterWidget(){
    if(background != NULL){
        delete background;
    }
}

void VolumeMeterWidget::calculateSize(){
    h = height();
    w = width();

    if(w > (RATIO * h)){
        w = RATIO * h;
    }else{
        h = w / RATIO;
    }
}

void VolumeMeterWidget::drawBackground(QPainter *painter){
    painter->drawImage(QRect(0, 0, w, h),
                       *background,
                       QRect(0, 0, w, h));
}

void VolumeMeterWidget::drawTriggerIndicator(QPainter */*painter*/){
}

void VolumeMeterWidget::initBackground(){
    if(background != NULL){
        delete background;
        background = NULL;
    }

    background = new QImage(size(), QImage::Format_ARGB32_Premultiplied);
}

void VolumeMeterWidget::initPainter(QPainter *painter){
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setViewport(0, 0, w, h);
    painter->setWindow(0, 0, w, h);
}

void VolumeMeterWidget::mouseReleaseEvent(QMouseEvent */*event*/){
    emit clicked();
}

void VolumeMeterWidget::paintGL(){
    QPainter painter(this);

    calculateSize();

    initPainter(& painter);

    if(background == NULL){
        initBackground();
        prepareBackground();
    }

    drawBackground(& painter);

    if(triggerValue > 0){
        drawTriggerIndicator(& painter);
    }

    draw(& painter);

    painter.end();
}

void VolumeMeterWidget::prepareBackground(){
}

void VolumeMeterWidget::resizeEvent(QResizeEvent */*event*/){
    if(background != NULL){
        delete background;
        background = NULL;
    }
}

void VolumeMeterWidget::setColors(QColor lower, QColor mid, QColor upper){
    lowerColor = lower;
    midColor = mid;
    upperColor = upper;
}

void VolumeMeterWidget::setColorThresholds(int lower, int upper){
    lowerThreshold = lower;
    upperThreshold = upper;
}

void VolumeMeterWidget::setDbUnit(bool b){
    dbUnit = b;
    currentValue = 0;

    if(background != NULL){
        delete background;
        background = NULL;
    }

    emit redraw();
}

void VolumeMeterWidget::setTriggerValue(int value){
    triggerValue = value;
    emit redraw();
}

void VolumeMeterWidget::setUseColoredScale(bool b){
    useColoredScale = b;
    emit redraw();
}

void VolumeMeterWidget::updateValue(int value){
    currentValue = value;
    emit redraw();
}
