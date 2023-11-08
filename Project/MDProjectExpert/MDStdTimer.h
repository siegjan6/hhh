/*!

\author dGFuZ3p3

\section 功能

运行时的基准定时器类，其他定时器都只能是此基准定时器的整数倍。

*/

#ifndef MDSTDTIMER_H
#define MDSTDTIMER_H

#include <QObject>
#include <QTimer>

class CMDProjectExpertImp;
class CMDStdTimer : public QObject
{
    Q_OBJECT
public:
    explicit CMDStdTimer(QObject *parent = 0);
    void start(int msec);
    void stop();

    QTimer m_stdTimer;
    CMDProjectExpertImp* m_projectExpert ;// NULL;
public slots:
    void doTimeout();
};

#endif // MDSTDTIMER_H
