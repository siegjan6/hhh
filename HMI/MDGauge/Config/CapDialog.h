/*!
\author emhhbmdsZQ==
\section  仪表中心点的属性配置对话框类。
*/

#ifndef CAPDIALOG_H
#define CAPDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CCapDialog;
}

class CCapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CCapDialog(QWidget *parent = 0);
    ~CCapDialog();
private:
    Ui::CCapDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);
public:
    void setData(CData* data);    //!  将保存的仪表中心点的各属性值显示到界面

private:
    CDataCap* m_cap;
    /*!
    \section button类，用于配置仪表部件的基本属性。
    */
    CPaintButton* m_capStyle;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient ;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void valueChanged(qint32 value);

private slots:
    void setEdgeWidth(qint32 value);   //!  设置边缘线宽
    void setRadius(qint32 value);     //! 设置半径
    void edgeWarning();         //!  超出边缘宽度范围报警
    void radiusWarning();       //!  超出半径范围报警
    void on_edgeColorBtn_clicked();
    void on_bkColor1Btn_clicked();
    void on_bkColor2Btn_clicked();
    void on_edgeStyleBtn_clicked();    //!  设置边缘风格
    void on_backGradientBtn_clicked();         //!  设置填充方式
    void on_capStyleBtn_clicked();    //!  设置中心点风格

};

#endif // CAPDIALOG_H
