/*!

\author eHVr

\section 功能

    每一个特征标签
*/
#ifndef CFEATURETAG_H
#define CFEATURETAG_H
#include <QList>
#include <QPainter>
#include "CTagPosition.h"
#include "CFeatureTagItem.h"

class CXAxis;
class CPoint;
class CFeatureTagLine;
class CFeatureTag
{
public:
    CFeatureTag();
    ~CFeatureTag();
/*!
  \section 基本属性
*/
public:
    CFeatureTagItem *item(const CFeatureTagItem::ItemType &type)  const;
    //! 特征标签项链表
    QList<CFeatureTagItem *> &itemList();
    void setItemList(const QList<CFeatureTagItem*> &itemList);

    double totalHeight();
    //! 绘制区域
    void setDrawArea(const QRectF &drawArea);

    //!视图名
    void setViewName(const QString &viewName);
    QString viewName() const;

    //!绑定时间字段
    void setTimeVariant(const QString &timeVariant);
    QString timeVariant() const;

    //! 项目地址
    void setProjectPath(const QString &path);
    QString projectPath() const;

    //! 各项间距
    double internal() const;
    void setInternal(double internal);

    //! 绘制的x坐标点
    double xPoint() const;
    void setXPoint(double xPoint);
    //! 时间标签
    CFeatureTagItem *timeItem() const;

    QString xName() const;
    void setXName(const QString &xName);

    CXAxis *xAxis() const;
    void setXAxis(CXAxis *xAxis);

    QString name() const;
    void setName(const QString &name);

    bool visible() const;
    void setVisible(bool visible);

    void setComment(const QString &commnet);
    QString comment() const;

    void setValue(double value);
    void setTime(const QDateTime &time);

    QPointF topLeft() const;
    void setTopLeft(const QPointF &topLeft);
private:
    QList<CFeatureTagItem*>  m_itemList;
    QRectF                   m_drawArea;
    double                   m_internal;
    double                   m_xPoint;
    QPointF                  m_topLeft;

    QString        m_xName;        //! 绑定的X轴名字
    CXAxis        *m_xAxis;        //! 绑定的X轴
    QString        m_name;         //! 变量名字
    bool           m_visible;      //! 标签是否可见
    QString        m_viewName;     //! 视图名称
    QString        m_timeVariant;  //! 时间字段名
    QString        m_projectPath;

/*!
\section 计算，绘制
*/
public:
    CFeatureTagLine *tagLine();
    CPoint          *topPoint();
    CPoint          *bottomPoint();

    void clearOldData();
    void calculate(double areaHeight, double xPoint,const CTagPos &pos);
    void draw(QPainter *p);

/*!
  \section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    CFeatureTagLine *m_tagLine;
    CPoint          *m_topPoint;
    CPoint          *m_bottomPoint;

public:
    CFeatureTag &operator = (const CFeatureTag &tag);
private:
    void clearItemList();       //! 清空item列表
public:
    void changeLanguage();
    void collectWords();
};

#endif // CFEATURETAG_H
