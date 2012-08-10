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

#ifndef VOLUMEMETERWIDGET_H
#define VOLUMEMETERWIDGET_H

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QtOpenGL/QGLWidget>

#define MAX_DB 110
#define MAX_LINEAR 200
#define RATIO 3.

/**
 * Base class for all volume displays.
 */
class VolumeMeterWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit VolumeMeterWidget(QWidget *parent = 0);
    ~VolumeMeterWidget();

    virtual void updateValue(int value);
    virtual void setDbUnit(bool b);
    virtual void setTriggerValue(int value);

    void setColors(QColor lower, QColor mid, QColor upper);
    void setColorThresholds(int lower, int upper);
    void setUseColoredScale(bool b);

signals:
    /**
     * This signal is triggered on a mouse release event.
     */
    void clicked();
    void redraw();

protected:
    /** QImage for holding the widgets background.
     *
     * This image is used to cache the non moving parts of the display.
     * The idea is to paint the background only when needed in order to improve the performance.
     * Ideally, the background is only re-painted on size changes.
     */
    QImage          *background;
    int             currentValue;
    bool            dbUnit;
    /** Height of the widget */
    float           h;
    Qt::GlobalColor triggerIndicatorColor;
    int             triggerValue;
    bool            useColoredScale;
    /** Width of the widget */
    float           w;

    int             lowerThreshold;
    int             upperThreshold;

    QColor          lowerColor;
    QColor          midColor;
    QColor          upperColor;


    /**
     * Calculate the current size of the widget.
     *
     * Calculates the widget size taking the aspect ration into account.
     * The size will be calculated such that the displayed widget fits
     * into the available space without being deformed.
     * This method sets the h and w variables.
     */
    virtual void calculateSize();

    /**
     * Method for drawing the widgets content.
     *
     * This is a pure virtual function that has to be implemented by all subclasses.
     * This method is responsible for painting the widgets content.
     */
    virtual void draw(QPainter *painter) = 0;
    virtual void drawBackground(QPainter *painter);
    virtual void drawTriggerIndicator(QPainter *painter);

    /**
     * Initialize the QImage which holds the background.
     */
    virtual void initBackground();
    /**
     * Initialize the painter.
     *
     * This method sets the viewport, window etc.
     */
    virtual void initPainter(QPainter *painter);
    /**
     * Method used to trigger the @ref VolumeMeterWidget#clicked() signal.
     */
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void paintGL();

    /**
     * Prepare the background.
     *
     * Draws the background on a QImage.
     * @see VolumeMeterWidget#background
     */
    virtual void prepareBackground();

    virtual void resizeEvent(QResizeEvent *event);
};

#endif // VOLUMEMETERWIDGET_H
