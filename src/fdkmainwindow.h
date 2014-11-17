#ifndef FDKMAINWINDOW_H
#define FDKMAINWINDOW_H

#include <QMainWindow>
#include <FDK/fdk.h>

namespace Ui {
class FDKMainWindow;
}

class FDKMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FDKMainWindow(QWidget *parent = 0);
    ~FDKMainWindow();

private:
    Ui::FDKMainWindow *ui;


    FDK::Brain *                brain;
    FDK::Region *               region;
    FDK::SparseCharEncoder *    encoder;
    FDK::SensorConnection *     connection;

public slots:
    void step();

};

#endif // FDKMAINWINDOW_H
