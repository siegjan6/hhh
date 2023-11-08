/*!
\section 绘制游标
*/
#ifndef DRAWVERNIER_H
#define DRAWVERNIER_H

#include <QDialog>
#include "DrawData.h"

namespace Ui {
class CDrawVernier;
}

class CDrawVernier : public QDialog
{
    Q_OBJECT

public:
    explicit CDrawVernier(QWidget *parent = 0);
    ~CDrawVernier();

private:
    Ui::CDrawVernier *ui;

private:
    CDrawData* m_vernierData;
    qint32 width;
    qint32 height;
    qint32 m_rectNum;
    QList<qreal>m_valueList;
    QList<QString>m_stringList1;
    QList<QString>m_stringList2;
    QList<QString>m_stringList3;

public:
    void setVernierData(CDrawData* vernierData);
    void setNum(qint32 num);
    void setValueList(QList<qreal>valueList);
    void setStringList1(QList<QString>stringList);
    void setStringList2(QList<QString>stringList);
    void setStringList3(QList<QString>stringList);
    qint32 getWidth();   //! 求取游标的长度
    qint32 getheight();  //! 求取游标的总高度

protected:
    void paintEvent(QPaintEvent*);

private:
    void drawValue(QPainter* painter); //! 绘制各数据值

};

#endif // DRAWVERNIER_H
