#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <FDK/fdk.h>

class SensorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SensorWidget(QWidget *parent = 0);

    void setSensor(FDK::Sensor * sensor) { _sensor = sensor; }

private:
    FDK::Sensor * _sensor = NULL;

protected:
     void paintEvent(QPaintEvent *event);

signals:

public slots:

};

#endif // SENSORWIDGET_H
