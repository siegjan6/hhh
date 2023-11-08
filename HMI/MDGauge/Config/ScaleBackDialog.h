/*!
\author emhhbmdsZQ==
\section  刻度背景的属性配置对话框类
*/
#ifndef SCALEBACKDIALOG_H
#define SCALEBACKDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CScaleBackDialog;
}

class CScaleBackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CScaleBackDialog(QWidget *parent = 0);
    ~CScaleBackDialog();
private:
    Ui::CScaleBackDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);
public:
    void setData(CData* data);     //!  将保存的仪表刻度背景的各属性值显示到界面

private:
    CDataScaleBack* m_scaleBack;   //!  刻度背景的数据类指针
    CPaintButton* m_scaleBackStyle;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对刻度背景的各个基本属性进行配置。
    */
    void on_scaleBackBtn_clicked();
    void setWid(qint32 value);
    void setRadius(qint32 value);
    void setEdgeWid(qint32 value);
    /*!
     \section 对刻度背景的风格以及填充样式的配置。
    */
   void on_edgeStyleBtn_clicked();
   void on_gradStyleBtn_clicked();
   /*!
    \section 对刻度背景的各部分颜色进行设置。
   */
    void on_edgeColorBtn_clicked();
    void on_color1Btn_clicked();
    void on_color2Btn_clicked();
    /*!
     \section 对刻度背景的各个基本属性设置超限的警告。
    */
    void widWarning();
    void radiusWarning();
    void edgeWarning();
};

#endif // SCALEBACKDIALOG_H
