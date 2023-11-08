/*!
\author emhhbmdsZQ==
\section  仪表LED数字显示的属性配置对话框类
*/
#ifndef LEDDIALOG_H
#define LEDDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"
#include "IDrawEnvironment.h"

class CColorDialog;
namespace Ui {
class CLedDialog;
}

class CLedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLedDialog(QWidget *parent = 0);
    ~CLedDialog();
private:
    Ui::CLedDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);   //!  在各个图标外绘制矩形框
public:
    void setData(CData* data);  //!  将保存的LED数字显示的各属性值显示到界面
    /*!
     \section 显示值绑定变量。
    */
    void setEnvironment(IDrawEnvrionment *value);
private:
    IDrawEnvrionment *m_environment;
private:
    CDataLed* m_led;   //!  LED数字的数据类指针
    CPaintButton* m_color;

    //! LED数字的位置坐标
    qreal m_x;
    qreal m_y;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
    \section 对LED数字的各个属性进行配置。
     */
    void setLeft(qint32 value);
    void setTop(qint32 value);
    void setDecimals(qint32 value);
    void setSize(qint32 value);
    void on_colorBtn_clicked();
     void setExpressionValue(QString str);
     /*!
     \section 设定值超过预定值跳出报警信息。
      */
     void leftWarning();
     void topWarning();
     void decimalsWarning();
     void sizeWarning();
     void on_expBtn_clicked();
};

#endif // LEDDIALOG_H
