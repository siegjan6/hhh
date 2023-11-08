/*!

\author eHVr

\section 功能

游标显示界面，
用来显示所有可视曲线的鼠标点的最近值
*/
#ifndef CVERNIERWIDGET_H
#define CVERNIERWIDGET_H

#include <QWidget>
#include <QList>

class CVernier;
class CLabelItem;

/*!
 \brief CVernierWidget
    游标界面
*/
class CVernierWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVernierWidget(QWidget *parent = 0);
    ~CVernierWidget();

/*!
\section 基本属性
*/
public:
    QList<CLabelItem *> labelList() const;
    void setLabelList(const QList<CLabelItem *> &labelList);

    QSizeF widgetSize() const;
    void setWidgetSize(const QSizeF &widgetSize);

    QFont itemFont() const;
    void setItemFont(const QFont &itemFont);

    QColor fontColor() const;
    void setFontColor(const QColor &fontColor);
private:
    QFont                 m_itemFont;       //! 字体
    QColor                m_fontColor;      //! 字体颜色

    QList<CLabelItem *>   m_labelList;      //! 标签项链表
    QSizeF                m_widgetSize;     //! 界面大小
public:
    CVernierWidget *clone() const;          //! 克隆函数
protected:
    void paintEvent(QPaintEvent *);
};

#endif // CVERNIERWIDGET_H
