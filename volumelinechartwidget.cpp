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

#include "volumelinechartwidget.h"

VolumeLineChartWidget::VolumeLineChartWidget(QWidget *parent) : VolumeMeterWidget(parent)
{
    volumeValues = new QList<float>();
    initValues();
}

VolumeLineChartWidget::~VolumeLineChartWidget(){
    delete volumeValues;
}

void VolumeLineChartWidget::draw(QPainter *painter){
    float vol;

    if(dbUnit){
        vol = ((float) currentValue) / MAX_DB;
    }else{
        vol = ((float) currentValue) / MAX_LINEAR;
    }

    volumeValues->removeFirst();
    volumeValues->append(vol);

    QPen pen(Qt::darkBlue);
    pen.setWidth(2 * h / 100.);
    painter->setPen(pen);

    for(int i = 0; i < (2.69 * h / 2.); i++){
        if(useColoredScale){
            float val = volumeValues->at(i);

            if(dbUnit){
                val *= MAX_DB;

                if(val < lowerThreshold){
                    pen.setColor(lowerColor);
                }else if (val < upperThreshold){
                    pen.setColor(midColor);
                }else{
                    pen.setColor(upperColor);
                }
            }else{
                val *= MAX_LINEAR;

                if(val < lowerThreshold){
                    pen.setColor(lowerColor);
                }else if (val < upperThreshold){
                    pen.setColor(midColor);
                }else{
                    pen.setColor(upperColor);
                }
            }

            painter->setPen(pen);
        }

        painter->drawLine((2 * i) + (0.02 * h),
                          (0.95 * h) - (0.94 * h * volumeValues->at(i)),
                          (2 * i) + (0.03 * h),
                          (0.95 * h) - (0.94 * h * volumeValues->at(i+1)));
    }
}

void VolumeLineChartWidget::drawTriggerIndicator(QPainter *painter){
    float vol;

    if(dbUnit){
        vol = ((float) triggerValue) / MAX_DB;
    }else{
        vol = ((float) triggerValue) / MAX_LINEAR;
    }

    QPen pen(triggerIndicatorColor);
    pen.setWidth(2 * h / 100.);
    painter->setPen(pen);

    painter->drawLine(0.03 * h,
                      (0.95 * h) - (0.94 * h * vol),
                      2.71 * h,
                      (0.95 * h) - (0.94 * h * vol));
}

void VolumeLineChartWidget::prepareBackground(){
    QPainter painter(background);
    initPainter(&painter);

#ifdef Q_WS_MAEMO_5
    QRectF background(0, 0, w, h);
    painter.fillRect(background, QBrush(Qt::white));

    QFont font = painter.font();
    font.setPointSizeF(font.pointSizeF() * 0.6 * h / 100.);
    painter.setFont(font);
#else
    QFont font = painter.font();
    font.setPointSizeF(font.pointSizeF() * h / 100.);
    painter.setFont(font);
#endif

    QPen grayPen(Qt::gray);
    grayPen.setWidth(2 * h / 100.);

    QPen blackPen(Qt::black);
    blackPen.setWidth(2 * h / 100.);

    if(dbUnit){
        for(int i = 0; i < (MAX_DB / 10); i++){
            float y = (0.95 * h) - (0.94 * h / (MAX_DB / 10.) * i);

            if(i % 2 == 0){
                painter.setPen(blackPen);

                if(i > 0){
                    painter.drawLine(0.02 * h, y, 2.72 * h, y);
                }

                painter.drawText(2.75 * h, y + (0.03 * h), QString().setNum(i*10));
            }else{
                painter.setPen(grayPen);
                painter.drawLine(0.02 * h, y, 2.72 * h, y);
            }
        }
    }else{
        for(int i = 0; i <= (MAX_LINEAR / 10); i++){
            float y = (0.95 * h) - (0.94 * h / (MAX_LINEAR / 10.)*i);

            if(i % 5 == 0){
                painter.setPen(blackPen);
                painter.drawLine(0.02 * h, y, 2.72 * h, y);

                if(i < 20){
                    painter.drawText(2.75 * h, y + (0.03 * h), QString().setNum(i*10));
                }
            }else{
                painter.setPen(grayPen);
                painter.drawLine(0.02 * h, y, 2.72 * h, y);
            }
        }
    }

    QRectF chartBackground(0.02 * h, 0.02 * h, 2.7 * h, 0.94 * h);
    painter.drawRect(chartBackground);
}

void VolumeLineChartWidget::initValues(){
    calculateSize();

    for(int i = 0; i < (2.7 * h / 2.); i++){
        volumeValues->append(0.);
    }
}

void VolumeLineChartWidget::resizeEvent(QResizeEvent *event){
    VolumeMeterWidget::resizeEvent(event);

    volumeValues->clear();
    initValues();
}

void VolumeLineChartWidget::setDbUnit(bool b){
    VolumeMeterWidget::setDbUnit(b);

    volumeValues->clear();
    initValues();
}
