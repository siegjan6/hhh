#ifndef JSOUTPUTWINDOW_H
#define JSOUTPUTWINDOW_H

#include <QObject>
#include <QString>

class JSOutputwindow : public QObject
{
    Q_OBJECT
public:
    explicit JSOutputwindow(QObject *parent = 0);
    Q_INVOKABLE void log(QString logInfoStr);
signals:
   void  writeLog(const QString&);
public slots:

};

#endif // JSOUTPUTWINDOW_H
