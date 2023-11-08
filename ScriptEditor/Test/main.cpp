#include <QApplication>
#include "MDJavaScriptCodeEditor.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString demoFilePath;
    #ifdef Q_OS_LINUX
    demoFilePath = "/home/hite/6.0/ScriptEditor/Test/files/demo.js";
    #endif
    #ifdef Q_OS_WIN
    demoFilePath = "c:\\demo.js";
    #endif

//    CMDJavaScriptCodeEditor* j = new CMDJavaScriptCodeEditor("");
//    j->importText("abc","var a = 1+ 1{}");
//    j->show();

    CMDJavaScriptCodeEditor2* j = new CMDJavaScriptCodeEditor2("");
    j->importText("abc","var a = 1+ 1{}");
   j->show();

    QString ss = j->text();
    return a.exec();
}
