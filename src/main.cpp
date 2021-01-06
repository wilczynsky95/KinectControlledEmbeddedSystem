#include "inc/Mainwindow.h"
#include <QApplication>
#include <memory.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.setWindowTitle("Sterowanie systemem wbudowanym");

    return a.exec();
}
