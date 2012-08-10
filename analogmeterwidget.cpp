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

#include "analogmeterwidget.h"

AnalogMeterWidget::AnalogMeterWidget(QWidget *parent) : VolumeMeterWidget(parent)
{
}

void AnalogMeterWidget::draw(QPainter *painter){
    painter->rotate(-25);
    if(dbUnit){
        painter->rotate(50. / ((float) MAX_DB) * currentValue);
    }else{
        painter->rotate(50. / ((float) MAX_LINEAR) * currentValue);
    }

    QPen pen(Qt::red);
    pen.setWidth(2 * h / 100.);
    painter->setPen(pen);

    painter->drawLine(0, 0, 0, -3.25 * h);

    if(dbUnit){
        painter->rotate(-50. / ((float) MAX_DB) * currentValue);
    }else{
        painter->rotate(-50. / ((float) MAX_LINEAR) * currentValue);
    }
    painter->rotate(25);

    QRectF bgRect(-0.5 * w, (-3.4 * h) + h, w, (3.4 * h) - h + 10);
    painter->fillRect(bgRect, Qt::white);
}

void AnalogMeterWidget::drawBackground(QPainter *painter){
    painter->drawImage(QRect(-0.5 * w, -3.4 * h, w, h),
                       *background,
                       QRect(0, 0, w, h));
}

void AnalogMeterWidget::drawTriggerIndicator(QPainter *painter){
    painter->rotate(-25);
    if(dbUnit){
        painter->rotate(50. / ((float) MAX_DB) * triggerValue);
    }else{
        painter->rotate(50. / ((float) MAX_LINEAR) * triggerValue);
    }

    QPen pen(triggerIndicatorColor);
    pen.setWidth(2 * h / 100.);
    painter->setPen(pen);

    painter->drawLine(0, 0, 0, -3.15 * h);

    if(dbUnit){
        painter->rotate(-50. / ((float) MAX_DB) * triggerValue);
    }else{
        painter->rotate(-50. / ((float) MAX_LINEAR) * triggerValue);
    }
    painter->rotate(25);
}

void AnalogMeterWidget::prepareBackground(){
    QPainter painter(background);
    initPainter(&painter);

    QRectF bgRectangle(-0.5 * w, -3.4 * h, w, h);
    painter.fillRect(bgRectangle, QBrush(Qt::white));

    QPen thinPen(Qt::black);
    thinPen.setWidth(2 * h / 100.);

    QPen thickPen(Qt::black);
    thickPen.setWidth(4 * h / 100.);


    QFont f = painter.font();
#ifdef Q_WS_MAEMO_5
    f.setPixelSize(f.pointSize() * h / 100.);
#else
    f.setPixelSize(f.pointSize() * h / 100. * 1.75);
#endif
    painter.setFont(f);


    painter.setPen(thinPen);
    QString label = (dbUnit) ? "dB" : "Linear";
    painter.drawText(-0.40 * h, -2.70 * h, 0.80 * h, 0.20 * h, Qt::AlignCenter, label);

    painter.rotate(-25);

    if(dbUnit){
        for(int i = 0; i < 23; i++){
            if(i % 4 == 0){
                if(i >= 16){
                    thickPen.setColor(Qt::red);
                }

                painter.setPen(thickPen);
                painter.drawLine(0, -2.97 * h, 0, -3.15 * h);
                painter.drawText(-0.20 * h, -3.40 * h, 0.40 * h, 0.20 * h, Qt::AlignCenter, QString().setNum((i/4)*20));
            }else{
                if(i >= 16){
                    thinPen.setColor(Qt::red);
                }
                painter.setPen(thinPen);

                if(i % 2 == 0){
                    painter.drawLine(0, -2.96 * h, 0, -3.08 * h);
                }else{
                    painter.drawLine(0, -2.96 * h, 0, -3.03 * h);
                }
            }

            painter.rotate(50. / ((float) MAX_DB) * 5.);
        }
        painter.rotate(-50. / ((float) MAX_DB) * 5.);
    }else{
        for(int i = 0; i < 16; i++){
            if(i % 4 == 0){
                if(i >= 12){
                    thickPen.setColor(Qt::red);
                }

                painter.setPen(thickPen);
                painter.drawLine(0, -2.97 * h, 0, -3.15 * h);
                painter.drawText(-0.20 * h, -3.38 * h, 0.40 * h, 0.20 * h, Qt::AlignCenter, QString().setNum((i/4)*50));
            }else{
                if(i >= 12){
                    thinPen.setColor(Qt::red);
                }
                painter.setPen(thinPen);

                if(i % 2 == 0){
                    painter.drawLine(0, -2.96 * h, 0, -3.08 * h);
                }else{
                    painter.drawLine(0, -2.96 * h, 0, -3.03 * h);
                }
            }

            painter.rotate(50. / ((float)MAX_LINEAR) * 12.5);
        }

        painter.setPen(thickPen);
        painter.drawLine(0, -2.97 * h, 0, -3.15 * h);
    }

    painter.rotate(-25);
    thinPen.setColor(Qt::black);
    painter.setPen(thinPen);
    painter.drawArc(-1.60 * h, -2.95 * h, 3.20 * h, 1.50 * h, 35*16, 110*16);
}

void AnalogMeterWidget::initPainter(QPainter *painter){
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setViewport(0, 0, w, h);

    painter->setWindow(-0.5 * w, -3.4 * h, w, h);
}
