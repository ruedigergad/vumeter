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

#include "horizontalvolumebarwidget.h"

HorizontalVolumeBarWidget::HorizontalVolumeBarWidget(QWidget *parent) : VolumeMeterWidget(parent)
{
}

void HorizontalVolumeBarWidget::draw(QPainter *painter){
    float vol;
    QColor color = QColor(Qt::darkBlue);

    if(dbUnit){
        vol = ((float) currentValue) / MAX_DB;

        if(useColoredScale){
            if(currentValue < lowerThreshold){
                color = lowerColor;
            }else if (currentValue < upperThreshold){
                color = midColor;
            }else{
                color = upperColor;
            }
        }
    }else{
        vol = ((float) currentValue) / MAX_LINEAR;

        if(useColoredScale){
            if(currentValue < lowerThreshold){
                color = lowerColor;
            }else if (currentValue < upperThreshold){
                color = midColor;
            }else{
                color = upperColor;
            }
        }
    }

    QRectF scale(0.02 * h, 0.02 * h, 2.94 * h * vol, 0.7 * h);
    painter->fillRect(scale, QBrush(color));
}

void HorizontalVolumeBarWidget::drawTriggerIndicator(QPainter *painter){
    float vol;

    if(dbUnit){
        vol = ((float) triggerValue) / MAX_DB;
    }else{
        vol = ((float) triggerValue) / MAX_LINEAR;
    }

    QPen pen(triggerIndicatorColor);
    pen.setWidth(2 * h / 100.);
    painter->setPen(pen);

    painter->drawLine(2.94 * h * vol, 0.03 * h, 2.94 * h * vol, 0.71 * h);
}

void HorizontalVolumeBarWidget::prepareBackground(){
    QPainter painter(background);
    initPainter(&painter);

#ifdef Q_WS_MAEMO_5
    QRectF background(0, 0, w, h);
    painter.fillRect(background, Qt::white);

    QFont font = painter.font();
    font.setPointSizeF(font.pointSizeF()* 0.6 * h / 100.);
    painter.setFont(font);
#else
    QFont font = painter.font();
    font.setPointSizeF(font.pointSizeF()* h / 100.);
    painter.setFont(font);
#endif
    QPen finePen(Qt::black);
    finePen.setWidth(1. * h / 100.);

    QPen thinPen(Qt::black);
    thinPen.setWidth(2. * h / 100.);

    QPen thickPen(Qt::black);
    thickPen.setWidth(4. * h / 100.);

    painter.setPen(finePen);

    QRectF chartBackground(0.02 * h, 0.02 * h, 2.96 * h, 0.7 * h);
    painter.drawRect(chartBackground);

    if(dbUnit){
        for(int i = 0; i <= (MAX_DB / 10); i++){
            float x = 2.96 * h / (MAX_DB / 10.) * i;

            if(i == 0){
                painter.setPen(thickPen);
                painter.drawLine((0.04 * h) + x, 0.74 * h, (0.04 * h) + x, 0.8 * h);

                painter.drawText(x, 0.96 * h, QString().setNum(i*10));
            }else if(i % 2 == 0){
                painter.setPen(thickPen);
                painter.drawLine(x - (0.01 * h), 0.74 * h, x - (0.01 * h), 0.8 * h);

                painter.setPen(thinPen);
                if(i > 0){
                    x -= 0.08 * h;
                }
                if(i >= 10){
                    x -= 0.04 * h;
                }

                painter.drawText(x, 0.96 * h, QString().setNum(i*10));
            }else{
                painter.setPen(thinPen);

                if(i < 11){
                    painter.drawLine(x - (0.01 * h), 0.73 * h, x - (0.01 * h), 0.77 * h);
                }else{
                    painter.drawLine((0.01 * h) + x, 0.73 * h, (0.01 * h) + x, 0.77 * h);
                }
            }
        }
    }else{
        for(int i = 0; i <= (MAX_LINEAR / 10); i++){
            float x = 2.96 * h / (MAX_LINEAR / 10.)*i;

            if(i == 0){
                painter.setPen(thickPen);
                painter.drawLine((0.04 * h) + x, 0.74 * h, (0.04 * h) + x, 0.8 * h);

                painter.drawText(x, 0.96 * h, QString().setNum(i*10));
            }else if(i % 5 == 0){
                painter.setPen(thickPen);
                if(i < 20){
                    painter.drawLine(x - (0.01 * h), 0.74 * h, x - (0.01 * h), 0.8 * h);

                    painter.setPen(thinPen);
                    if(i > 0){
                        x -= 0.08 * h;
                    }
                    if(i >= 10){
                        x -= 0.04 * h;
                    }

                    painter.drawText(x, 0.96 * h, QString().setNum(i*10));
                }else{
                    painter.drawLine(x - (0.01 * h), 0.74 * h, x - (0.01 * h), 0.8 * h);
                }
            }else{
                painter.setPen(thinPen);
                painter.drawLine(x - (0.01 * h), 0.73 * h, x - (0.01 * h), 0.77 * h);
            }
        }
    }
}
