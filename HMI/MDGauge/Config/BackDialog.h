/*!
\author emhhbmdsZQ==
\section  仪表背景的属性配置对话框类
*/
#ifndef BACKDIALOG_H
#define BACKDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"
class CColorDialog;

namespace Ui {
class CBackDialog;
}

class CBackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBackDialog(QWidget *parent = 0);
    ~CBackDialog();

private:
    Ui::CBackDialog *ui;

public:
    void setData(CData* data);   //!  将保存的仪表背景的各属性值显示到界面。

private:
    CDataBack* m_back;  //!  仪表背景数据类指针
    CPaintButton* m_backStyle;
    CPaintButton* m_edgeStyle ;
    CPaintButton* m_edgeColor ;
    CPaintButton* m_gradient ;
    CPaintButton* m_color1 ;
    CPaintButton* m_color2 ;
    CPaintButton* m_frameGradient ;
    CPaintButton* m_frameColor1 ;
    CPaintButton* m_frameColor2;

protected:
     void initDialog();
     void paintEvent(QPaintEvent*);

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
    \section 设置背景风格，线宽和边缘宽度。
    */
    void on_backStyleBtn_clicked();
    void setEdgeWidth(qint32 value);
    void setFrameWidth(qint32 value);
    /*!
    \section 设置线颜色以及各填充色。
    */
    void on_edgeColorBtn_clicked();
    void on_edgeStyleBtn_clicked();
    void on_color1Btn_clicked();
    void on_color2Btn_clicked();
    void on_gradStyleBtn_clicked();

    void on_frameColor1Btn_clicked();
    void on_frameColor2Btn_clicked();
    void on_frameGraBtn_clicked();
    /*!
    \section 设定值超过预定值跳出报警信息。
    */
    void edgeWarning();
    void frameWarning();
};

#endif // BACKDIALOG_H
