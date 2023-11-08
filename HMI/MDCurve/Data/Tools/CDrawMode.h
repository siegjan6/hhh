/*!
\author eHVr

\section 功能

    绘制模式
*/
#ifndef CDRAWMODE_H
#define CDRAWMODE_H

#include <QDataStream>
#include <QObject>
#include <QTimer>

class CDrawMode:public QObject
{
    Q_OBJECT
public:
    CDrawMode();
    ~CDrawMode();
public:
    enum class Orientation{
        LeftToRight = 0,
        RigthToLeft
    };
/*!
  \section 基本属性
*/
public:
    //! 绘制模式
    Orientation orietation() const;
    void setOrietation(const Orientation &orietation);

    //! 刷新时间
    qint32 freshTime() const;
    void setFreshTime(const qint32 &freshTime);

    void start();
    void stop();
signals:
    void outOfTime();
private:
    Orientation     m_orietation;
    qint32          m_freshTime ; //! set the update time must large than 1000 ms.
public:
    CDrawMode *clone();
/*!
  \section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    QTimer      *m_timer;

};

#endif // CDRAWMODE_H
