#include "cdialogtest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CDialogTest w;
    w.show();

    return a.exec();
}
