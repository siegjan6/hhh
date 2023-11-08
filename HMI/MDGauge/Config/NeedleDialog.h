/*!
\author emhhbmdsZQ==
\section  仪表指针的属性配置对话框类
*/
#ifndef NEEDLEDIALOG_H
#define NEEDLEDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"
#include "IDrawEnvironment.h"
class CColorDialog;
namespace Ui {
class CNeedleDialog;
}

class CNeedleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CNeedleDialog(QWidget *parent = 0);
    ~CNeedleDialog();
private:
    Ui::CNeedleDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);    //!  在各个图标外绘制矩形框
public:
    void setData(CData* data);    //!  将保存的仪表指针的各属性值显示到界面
    /*!
     \section 指针绑定变量。
    */
    void setEnvrionment(IDrawEnvrionment *value);
private:
    IDrawEnvrionment* m_environment;
private:
    CDataNeedle* m_needle;    //!  指针数据类指针
    CPaintButton* m_needleStyle;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对指针的各个基本属性进行配置。
    */
    void setNeedleWidth(qint32 value);
    void setLength(qint32 value);
    void setRadius(qint32 value);
    void setStep(qint32 value);
    void setEdgeWide(qint32 value);
    /*!
     \section 对指针的各个基本属性设置超限的警告。
    */
    void widthWarning();
    void lengthWarning();
    void radiusWarning();
    void stepWarning();
    void edgeWarning();
    /*!
     \section 对指针的风格，指针边缘线的风格以及指针的填充样式进行配置。
    */

    void on_edgeStyleBtn_clicked();
    void on_gradStyleBtn_clicked();
    void on_needleStyBtn_clicked();
    /*!
     \section 对指针的各部分颜色进行设置。
    */
    void on_color1Btn_clicked();
    void on_color2Btn_clicked();
    void on_edgeColorBtn_clicked();
    /*!
     \section 对指针绑定变量表达式。
    */
    void setExpressionValue(QString str);
    void on_expBtn_clicked();
};

#endif // NEEDLEDIALOG_H
