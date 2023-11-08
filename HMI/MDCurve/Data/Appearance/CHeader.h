/*!

\author eHVr

\section 功能

    标题类,属性
    \a startPoint
    \a caption
    \a font;
    \a fontColor
*/
#ifndef CHEADER_H
#define CHEADER_H

#include <QPointF>
#include <QString>
#include <QColor>
#include <QFont>
#include <QPainter>
#include "ILanguageObject.h"
#include "MDMultiLanguage.h"
#include "ITranslateWidgetTool.h"

class CHeader
{
public:
    CHeader();
    ~CHeader();

/*!
\section 基本属性
*/
public:
    //! 绘制起点
    void setStart(const QPoint &point);
    QPoint start() const;

    //! 内容
    void setText(const QString &string);
    QString text() const;

    //! 字体
    void setFont(const QFont &font);
    QFont font() const;

    //! 字体颜色
    QColor fontColor() const;
    void setFontColor(const QColor &fontColor);
public:
    QPoint  m_startPoint;
    QString m_caption ;

    QFont   m_font;
    QColor  m_fontColor ;

public:
    void draw(QPainter *p); //! 绘制
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

public:
    void changeLanguage();
    void collectWords();
private:
    QString m_translateCaption;
};

#endif // CHEADER_H
