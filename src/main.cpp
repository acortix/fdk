#include "fdkmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FDKMainWindow w;
    w.show();

    return a.exec();
}
