#include "graphwidget.h"
#include <QPainter>
GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent)
{
}

void GraphWidget::addValue(float v){
    _values.append(v);
    if(v > _max){
        _max = v;
    }
    if(v < _min){
        _min = v;
    }
    if(_values.length() > _maxValues){
        _values.pop_front();
    }
}

void GraphWidget::paintEvent(QPaintEvent * /* event */){

    float hStep = 0;
    if(_values.length() == 0){
        hStep = 1;
    } else {
        hStep = (float) this->width() / (float)_values.length();
    }
    float contentHeight = _max-_min;
    if(contentHeight < 1) contentHeight = 1;
    float vStep = (float) this->height() / (float)(contentHeight*2);


    QPainter painter(this);
    painter.fillRect(0,0,this->width(),this->height(),Qt::white);
    painter.setPen( QPen(Qt::red));
    painter.drawLine(0,this->height()/2,this->width(),this->height()/2);
    int step = 0;
    int cx = 0;
    int cy = 0;
    for(float v : _values){
        cx = step*hStep;
        cy = (float) (this->height()/2) - (v*vStep);
        painter.setPen( QPen(Qt::black) );
        painter.drawPoint(cx,cy);
        step++;
    }

}
