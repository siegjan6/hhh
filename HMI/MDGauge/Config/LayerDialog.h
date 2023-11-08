/*!
\author emhhbmdsZQ==
\section  仪表控制层的属性配置对话框类
*/

#ifndef LAYERDIALOG_H
#define LAYERDIALOG_H
#include <QDialog>
#include "DrawData.h"

namespace Ui {
class CLayerDialog;
}

class CLayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLayerDialog(QWidget *parent = 0);
    ~CLayerDialog();

private:
    Ui::CLayerDialog *ui;
protected:
    void initDialog();

public:
    void setData(CData* data);   //!  将保存的控制层的各属性的数值显示到界面

private:
    /*!
    \section 设置控制层的初始绘制比例以及变化量的初始值
    */
    CDataLayer* m_layer;
    qreal m_xChange;
    qreal m_yChange;
    qreal m_widthChange;
    qreal m_hightChange;
    qreal m_setLeft;
    qreal  m_setTop;
    qreal m_setWid;
    qreal m_setHgt;
   /*!
   \section 设置控制层的大小以及在控制层改变时的一些中间变量的初始值
   */
    QRectF m_rectLayer;
    QRectF m_rectHight;
    QRectF m_rectWidth;
    QRectF m_rectX;
    QRectF m_rectY;
    QRectF m_rectMove;
    QRectF m_rectL;
    QRectF m_rectH;
    QRectF m_rectW;
    QRectF m_recX;
    QRectF m_recY;
    QRectF m_rectM;
    QPointF m_lastPnt;
    bool m_isHover;
    qint32 m_ScaleMove;
    /*!
    \section 控制层移动缩小放大的鼠标事件实现
    */
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent*);
    /*!
    \section 设置控制层的相关属性值
    */
protected slots:
    void setLeftValue(QString str);
    void setTopValue(QString str);
    void setWidthValue(QString str);
    void setHightValue(QString str);
    void setStartValue(QString str);
    void setValueRange(QString str);
    void setScout(QString str);
    void setStartAngle(QString str);
    void setAngleRange(QString str);

private slots:
    void on_horRadioBtn_clicked();
    void on_vertRadioBtn_clicked();

private slots:
    void setLayer();
};

#endif // LAYERDIALOG_H
