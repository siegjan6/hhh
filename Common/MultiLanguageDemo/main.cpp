#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //CGenerateDictDialog dlg;
    //dlg.show();

    return a.exec();
}
