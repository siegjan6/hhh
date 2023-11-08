/*!
\author emhhbmdsZQ==
\section  仪表半圆背景的属性配置对话框类
*/

#ifndef HALFBACKDIALOG_H
#define HALFBACKDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CHalfBackDialog;
}

class CHalfBackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CHalfBackDialog(QWidget *parent = 0);
    ~CHalfBackDialog();

private:
    Ui::CHalfBackDialog *ui;

protected:
     void initDialog();
     void paintEvent(QPaintEvent*);  //!  在各个图标外绘制矩形框
public:
    void setData(CData* data);  //!  将保存的仪表半圆背景的各属性的数值显示到界面
private:
     CDataHalfBack* m_halfBack;  //!  半圆背景数据类指针
     CPaintButton* m_halfBackStyle;
     CPaintButton* m_halfArrange;
     CPaintButton* m_edgeStyle;
     CPaintButton* m_edgeColor;
     CPaintButton* m_gradient;
     CPaintButton* m_color1;
     CPaintButton* m_color2;
     CPaintButton* m_frameGradient;
     CPaintButton* m_frameColor1;
     CPaintButton* m_frameColor2;

private:
     bool eventFilter(QObject *watched, QEvent *event);

private slots:
     /*!
     \section 对半圆背景的各个属性进行配置。
     */
    void setEdgeWidth(qint32 value);
    void setFrameWidth(qint32 value);
    void edgeWarning();
    void frameWarning();
    void on_halfBackStyBtn_clicked();
    void on_halfArrangeBtn_clicked();
    void on_edgeStyleBtn_clicked();
    void on_bkGraBtn_clicked();
    void on_frameGraBtn_clicked();

    void on_edgeColorBtn_clicked();
    void on_bkColor1Btn_clicked();
    void on_bkColor2Btn_clicked();
    void on_frameColor1Btn_clicked();
    void on_frameColor2Btn_clicked();

};

#endif // HALFBACKDIALOG_H
