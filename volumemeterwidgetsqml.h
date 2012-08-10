#include <stdio.h>
#include <QtDeclarative/QDeclarativeExtensionPlugin>
#include <QtDeclarative/qdeclarative.h>
#include <QtGui/QGraphicsProxyWidget>
#include <QColor>

#include "analogmeterwidget.h"
#include "horizontalvolumebarwidget.h"
#include "volumelinechartwidget.h"
#include "volumemeterwidget.h"

/*
 * Volume Meter Widget base class proxy.
 */
class VolumeMeter : public QGraphicsProxyWidget{
    Q_OBJECT

public:
    VolumeMeter(QGraphicsItem* parent = 0)
        : QGraphicsProxyWidget(parent)
    {
    }

    Q_INVOKABLE void resize(int w, int h){
        widget->resize(w, h);
        update();
    }

    Q_INVOKABLE void updateValue(int value){
        widget->updateValue(value);
        update();
    }

    Q_INVOKABLE void setDbUnit(bool db){
        widget->setDbUnit(db);
        update();
    }

    Q_INVOKABLE void setTriggerValue(int value){
        widget->setTriggerValue(value);
        update();
    }

    Q_INVOKABLE void setColors(QColor lower, QColor mid, QColor upper){
        widget->setColors(lower, mid, upper);
        update();
    }

    Q_INVOKABLE void setColorThresholds(int lower, int upper){
        widget->setColorThresholds(lower, upper);
        update();
    }

    Q_INVOKABLE void setUseColoredScale(bool b){
        widget->setUseColoredScale(b);
        update();
    }

protected:
    VolumeMeterWidget *widget;
};

/*
 * Analog Meter
 */
class AnalogMeter : public VolumeMeter
{
    Q_OBJECT

public:
    AnalogMeter(QGraphicsItem* parent = 0)
        : VolumeMeter(parent)
    {
        widget = new AnalogMeterWidget(0);
        setWidget(widget);
    }
};

/*
 * Horizontal Bar Meter
 */
class HorizontalVolumeBar : public VolumeMeter
{
    Q_OBJECT

public:
    HorizontalVolumeBar(QGraphicsItem* parent = 0)
        : VolumeMeter(parent)
    {
        widget = new HorizontalVolumeBarWidget(0);
        setWidget(widget);
    }
};

/*
 * Line Chart
 */
class VolumeLineChart : public VolumeMeter
{
    Q_OBJECT

public:
    VolumeLineChart(QGraphicsItem* parent = 0)
        : VolumeMeter(parent)
    {
        widget = new VolumeLineChartWidget(0);
        setWidget(widget);
    }
};


/*
class VolumeMeterWidgetsPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri)
    {
        qmlRegisterType<AnalogMeter>(uri, 1, 0, "AnalogMeter");
        qmlRegisterType<HorizontalVolumeBar>(uri, 1, 0, "HorizontalVolumeBar");
        qmlRegisterType<VolumeLineChart>(uri, 1, 0, "VolumeLineChart");
    }
};

#include "volumemeterwidgetsqml.moc"

Q_EXPORT_PLUGIN2(vumeter, VolumeMeterWidgetsPlugin)
*/
