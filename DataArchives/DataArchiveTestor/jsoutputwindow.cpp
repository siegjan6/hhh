#include "jsoutputwindow.h"
#include <QPlainTextEdit>

QPlainTextEdit *  GetOutputWindow();

JSOutputwindow::JSOutputwindow(QObject *parent) :
    QObject(parent)
{
    QPlainTextEdit * pOutputWin = GetOutputWindow();
    connect(this, SIGNAL(writeLog(const QString&)), pOutputWin, SLOT(appendPlainText(const QString&)));
}

void JSOutputwindow::log(QString logInfoStr)
{
    emit writeLog(logInfoStr.append("\r\n"));
}
