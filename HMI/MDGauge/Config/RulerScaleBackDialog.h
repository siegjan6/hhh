/*!
\author emhhbmdsZQ==
\section  直表刻度背景的属性配置对话框类
*/
#ifndef RULERSCALEBACKDIALOG_H
#define RULERSCALEBACKDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CRulerScaleBackDialog;
}

class CRulerScaleBackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRulerScaleBackDialog(QWidget *parent = 0);
    ~CRulerScaleBackDialog();

private:
    Ui::CRulerScaleBackDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);

public:
    void setData(CData* data);    //!  将保存的直表刻度背景的各属性值显示到界面

private:
    CRulerScaleBack* m_rScaleBack;   //!  直表刻度背景的数据类指针
    CPaintButton* m_rScaleBackStyle;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对直表刻度背景的各个基本属性进行配置。
    */
    void setBackWid(qint32 value);
    void setOffset(qint32 value);
    void setEdgeWide(qint32 value);
    /*!
     \section 对直表刻度背景的各个基本属性设置超限的警告。
    */
    void widWarning();
    void offsetWarning();
    void edgeWarning();
    /*!
     \section 对直表刻度背景的布局以及边缘线的风格和填充风格进行设置。
    */
    void on_rBackArrangeBtn_clicked();
    void on_redgeStyleBtn_clicked();
    void on_rgradStyleBtn_clicked();
    /*!
     \section 对直表刻度背景的各部分颜色进行设置。
    */
    void on_rcolor1Btn_clicked();
    void on_rcolor2Btn_clicked();
    void on_redgeColorBtn_clicked();
};

#endif // RULERSCALEBACKDIALOG_H
