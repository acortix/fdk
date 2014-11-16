#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QList>
class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);

    void addValue(float v);
private:
    float _min = 0.0f;
    float _max = 0.0f;
    int _maxValues = 1000;
    QList<float> _values;
protected:
     void paintEvent(QPaintEvent *event);
signals:

public slots:

};

#endif // GRAPHWIDGET_H
