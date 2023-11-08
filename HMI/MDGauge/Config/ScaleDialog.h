/*!
\author emhhbmdsZQ==
\section  刻度的属性配置对话框类
*/
#ifndef SCALEDIALOG_H
#define SCALEDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CScaleDialog;
}

class CScaleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CScaleDialog(QWidget *parent = 0);
    ~CScaleDialog();

private:
    Ui::CScaleDialog *ui;   

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);

public:
    void setData(CData* data);    //!  将保存的仪表刻度的各属性值显示到界面

private:
    CDataScale* m_scale;    //!  刻度的数据类指针
    CPaintButton* m_scaleStyle;
    CPaintButton* m_scaleArrange;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对刻度的风格，填充样式以及边缘线的风格进行配置。
    */
    void on_scaleStyleBtn_clicked();
    void on_layStyleBtn_clicked();
    void on_gradStyleBtn_clicked();
    void on_edgeStyleBtn_clicked();
    /*!
     \section 对刻度的各个基本属性进行配置。
    */
    void setWid(qint32 value);
    void setLen(qint32 value);
    void setRadius(qint32 value);
    void setSubScale(qint32 value);
    void setEdgeWid(qint32 value);
    void setSub(qint32 state);
    /*!
     \section 对刻度的各个基本属性设置超限的警告。
    */
    void widWarning();
    void lenWarning();
    void radiusWarning();
    void subScaleWarning();
    void edgeWidWarning();
    /*!
     \section 对刻度的各部分颜色进行设置。
    */
    void on_edgeColorBtn_clicked();
    void on_color1Btn_clicked();
    void on_color2Btn_clicked();

};

#endif // SCALEDIALOG_H
