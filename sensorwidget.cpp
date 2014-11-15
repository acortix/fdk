#include "sensorwidget.h"
#include <QPainter>
SensorWidget::SensorWidget(QWidget *parent) :
    QWidget(parent)
{
}


void SensorWidget::paintEvent(QPaintEvent * /* event */){
    if(_sensor != NULL){
        float hStep = (float) this->width() / (float)_sensor->settings().width;
        float vStep = (float) this->height() / (float)_sensor->settings().height;

        QPainter painter(this);
        int cx = 0;
        int cy = 0;
        for(int x = 0; x < _sensor->settings().width; x++){
            cx = x*hStep;
            for(int y = 0; y < _sensor->settings().height; y++){
                cy = y*vStep;
                if(_sensor->read(x,y)){
                    //painter.setPen( QPen(Qt::black) );
                    //painter.drawPoint(cx,cy);
                    painter.fillRect( QRect(cx,cy,hStep+1,vStep+1),QColor(128,128,128));
                } else {
                    //painter.setPen( QPen(Qt::red) );
                    //painter.drawPoint(cx,cy);
                    painter.fillRect( QRect(cx,cy,hStep+1,vStep+1),QColor(0,0,0));
                }

            }
        }

    }
}
