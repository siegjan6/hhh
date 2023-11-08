/*!

\author eHVr

\section 功能

    标签子项，属性
    \a visible  可见
    \a itemName 标签名
    \a comment  说明
    \a format   显示格式,精度只能保证小数点后位数
    \a font     字体

    派生：CFeatureTagCommentItem
          CFeatureTagTimeItem
          CFeatureTagValueItem
*/
#ifndef CFEATURETAGITEM_H
#define CFEATURETAGITEM_H
#include <QString>
#include <QFont>
#include <QRectF>
#include <QDateTime>

#include "CTagPosition.h"
#include "ILanguageObject.h"
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

class CFeatureTagItem
{
public:
    CFeatureTagItem();
    CFeatureTagItem(const QString &itemName,const QString &cm,const QString &fm);
    virtual  ~CFeatureTagItem();

    enum class ItemType
    {
        Comment = 0,
        Time,
        Value
    };
/*!
\section 基本属性
*/
public:
    //! 可见
    bool visible() const;
    void setVisible(bool visible);

    //! 标签名
    QString itemName() const;
    void setItemName(const QString &itemName);

    //! 说明
    virtual QString comment() const;
    virtual void setComment(const QString &comment);

    //! 显示格式,精度只能保证小数点后位数
    virtual QString format() const;
    virtual void setFormat(const QString &format);

    QFont font() const;
    void setFont(const QFont &font);

    //! 位置点
    QPointF point() const;
    void setPoint(const QPointF &point);
private:
    bool      m_visible;
    QString   m_itemName;
    QString   m_comment;
    QString   m_format;
    QFont     m_font;
    QPointF   m_point;
/*!
\section 位置
*/
public:
    //! 垂直方向位置
    CTagVPos vPos() const;
    void setVPos(const CTagVPos &vPos);

    //! 水平方向位置
    CTagHPos hPos() const;
    void setHPos(const CTagHPos &hPos);
private:
    CTagVPos  m_vPos;
    CTagHPos  m_hPos;
/*!
\section 重载部分
*/
public:
    virtual void setValue(double value){ Q_UNUSED(value)}
    virtual double value() const {return -1;}

    virtual QDateTime dateTime() const{return QDateTime();}
    virtual void setDateTime(const QDateTime &dateTime){Q_UNUSED(dateTime)}

    //! 类型
    virtual ItemType type() const {return ItemType::Comment;}
/*!
\section 计算，绘制
*/
public:
    virtual void calculate() = 0;
    virtual void drawItem(QPainter *) = 0;

    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

    double textHeight () const;
    void setDrawTopLeft(const QPointF &topLeft);
private:
    QRectF  m_drawArea;//! 绘制区域
/*!
\section 序列化和反序列化
*/
public:
    virtual void serialize(QJsonObject& json);
    virtual void deserialize(const QJsonObject& json);

//    CFeatureTagItem & operator =(const CFeatureTagItem &item);
public:
    void changeLanguage();
    void collectWords();
private:
    QString m_translateComment;
    QString m_translateName;
};

#endif // CFEATURETAGITEM_H
