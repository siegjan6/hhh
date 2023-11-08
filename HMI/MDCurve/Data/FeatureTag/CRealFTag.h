/*!

\author eHVr

\section 功能
    实时标签
*/
#ifndef CREALFTAG_H
#define CREALFTAG_H
#include <QList>
#include <QFont>
#include <QPointF>
#include "CTagPosition.h"
#include "CFeatureTagItem.h"

class CPoint;
class CFeatureTagLine;
class CXAxis;
class CRealFTag
{
public:
    CRealFTag();
    ~CRealFTag();

    // return the type of the item. because the list has only three items and they are
    // they are different type,so ,the return item would never be the same one.
    CFeatureTagItem *item(const CFeatureTagItem::ItemType &type)  const;

    QList<CFeatureTagItem *> itemList() const;
    void setItemList(const QList<CFeatureTagItem *> &itemList);

    double totalHeight();

    void setValue(double value);
    void setTime(const QDateTime &time);

    void setComment(const QString &commnet);
    QString comment() const;

    void setFormat(const QString &format);
    QString format()const;

    QString xName() const;
    void setXName(const QString &xName);

    CXAxis *xAxis() const;
    void setXAxis(CXAxis *xAxis);

public:
    CFeatureTagLine *tagLine();
    CPoint *topPoint();
    CPoint *bottomPoint();

    void clearOldData();
    void calclualte(double areaHeight, double xPoint,const CTagPos &pos);
    void draw(QPainter *p);
public:
    QFont font() const;
    void setFont(const QFont &font);

    QPointF topLeft() const;
    void setTopLeft(const QPointF &topLeft);

    QString name() const;
    void setName(const QString &name);

    double internal() const;
    void setInternal(double internal); //! 内部间距

    bool visible() const;
    void setVisible(bool visible);
public:
    CRealFTag &operator =(const CRealFTag &other);

    //! 序列化和反序列化
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    void clearItemList();
private:
    bool    m_visible;  //! if the tag visible.
    QString m_name;     //! variant name
    QFont   m_font;
    QPointF m_topLeft;
    double  m_internal; //! item internal

    QList<CFeatureTagItem*> m_itemList;

    CFeatureTagLine *m_tagLine;
    CPoint          *m_topPoint;
    CPoint          *m_bottomPoint;
    QString          m_xName;       //! 绑定的X轴名字
    CXAxis          *m_xAxis;       //! 绑定的X轴
/*!
\section 多语言
*/
public:
    void changeLanguage();
    void collectWords();
};

#endif // CREALFTAG_H
