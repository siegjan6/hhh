#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <QEvent>
#include <QVariant>

//void postEvent(QObject *receiver, QEvent *event, int priority = Qt::NormalEventPriority);
//该函数实现将自定义的消息发送到队列，且new QEvent(CustomEvent_Login))只能动态分配
class 	CCustomEvent : public QEvent
{
public:
    CCustomEvent(QEvent::Type type, int fromModule);
    //!来自哪个模块 1:Driver,2:RTDBUI
    int  fromModule();

    QVariant param();
    void setParam(QVariant param);

    QVariant value();
    void setValue(QVariant value);
private:
    int m_fromModule;
    QVariant m_param;
    QVariant m_value;
};
#endif // CUSTOMEVENT_H
