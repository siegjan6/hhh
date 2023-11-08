/*!

\author bWVpeGw=

\section 功能

管理【管道流动模块管理类.
    用来设置管道流动的相关属性】。

*/

#ifndef MDNODESTREAM_H
#define MDNODESTREAM_H

#include<qobject.h>
#include<qpainter.h>
#include <QTimer>
#include <QVector>
#include <QJsonObject>
#include "MDDrawNode.h"
#include "MDExpression.h"

// 管道流动模块
class CMDNodeStream:public QObject
{
    Q_OBJECT
public:
    CMDNodeStream(CMDDrawNode* node);
    virtual ~CMDNodeStream();

    enum class Direction{ forward,reverse};
    CMDNodeStream *clone() ;

/*!
\section 成员变量
*/
private:
    Direction m_direction;              //! 流动方向
    bool m_enable;                      //! 是否显示流动
    bool m_isFlow;                      //! 是否流动
    int m_interval;                     //! 定时器时间间隔
    qreal m_dashSingleOffset;           //! 每次刷新流动的距离
    qreal m_dashOffset;                 //! 当前移动距离
    qreal m_percent;                    //! 流动百分比
    qreal m_maxOffset;                  //! 最大间隔点
    QVector<qreal> m_dashPattern;       //! 流动点间隔风格
    QColor m_color;                     //! 流动线条颜色
    QPen m_pen;                         //! 流动线条画笔
    QVector<QLineF> m_data;             //! 流动线条数据点
    QTimer* m_timer;                    //! 定时器指针
    CMDDrawNode* m_object;              //! 管道控件指针

/*!
\section 成员变量接口函数
*/
public:
    inline void setDirection(Direction d){m_direction = d;}
    inline Direction direction(){return m_direction;}

    inline void setStreamLineVisible(bool enable){m_enable = enable;}
    inline bool streamLineVisible(){return m_enable;}

    inline void setStreamLineFlow(bool bFlow){m_isFlow = bFlow;}
    inline bool streamLineFlow(){return m_isFlow;}

    inline void setInterval(int msec){m_interval = msec;}
    inline int interval(){return m_interval;}

    void setPercent(qreal p);
    qreal percent();

    void setDashSingleOffset(qreal offset);
    int dashSingleOffset();

    QVector<qreal> dashPattern() const;
    void setDashPattern(QVector<qreal> pattern);

    qreal maxOffset();

    QColor color() const;
    void setColor(const QColor &color);

    void setObject(CMDDrawNode *value);

    CMDPenManager m_penManager;         //! 画笔管理器

public:
    void start(); //! 启动定时器
    void stop(); //! 停止定时器

    void drawStreamLine(QPainter* painter); //! 绘制流动线条


    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    void onNodeChanged();
    QPen updatePen();

private:
    void updatePercent(qreal percent);
    void onTimerOut();
    qreal linesLength(); //! 获取总长度
};

#endif // MDNODESTREAM_H
