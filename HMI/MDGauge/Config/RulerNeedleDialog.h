/*!
\author emhhbmdsZQ==
\section  直表指针的属性配置对话框类
*/
#ifndef RULERNEEDLEDIALOG_H
#define RULERNEEDLEDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"
#include "IDrawEnvironment.h"

class CColorDialog;
namespace Ui {
class CRulerNeedleDialog;
}

class CRulerNeedleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRulerNeedleDialog(QWidget *parent = 0);
    ~CRulerNeedleDialog();

private:
    Ui::CRulerNeedleDialog *ui;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);

public:
    void setData(CData* data);  //!  将保存的直表指针的各属性值显示到界面
    /*!
     \section 指针绑定变量。
    */
     void setEnvrionment(IDrawEnvrionment *value);
private:
     IDrawEnvrionment *m_environment;

private:
    CRulerNeedle* m_rNeedle;    //!  直表指针数据类指针
    CPaintButton* m_rneedleStyle;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对直表指针的各个基本属性进行配置。
    */
    void setRNeedleWid(qint32 value);
    void setOffset(qint32 value);
    void setStep(qint32 value);
    void setEdgeWide(qint32 value);
    /*!
     \section 对直表指针的各个基本属性设置超限的警告。
    */
    void widWarning();
    void offsetWarning();
    void stepWarning();
    void edgeWarning();
    /*!
     \section 对直表指针的布局以及边缘线的风格和填充风格进行设置。
    */
    void on_rNeedleArrangeBtn_clicked();
    void on_redgeStyleBtn_clicked();
    void on_rgradStyleBtn_clicked();
    /*!
     \section 对直表指针的各部分颜色进行设置。
    */
    void on_redgeColorBtn_clicked();
    void on_rcolor1Btn_clicked();
    void on_rcolor2Btn_clicked();
    /*!
     \section 对直表指针绑定变量表达式。
    */
    void setExpValue(QString str);
    void on_rExpBtn_clicked();
};

#endif // RULERNEEDLEDIALOG_H
