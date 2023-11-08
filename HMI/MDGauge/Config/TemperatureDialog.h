/*!
\author emhhbmdsZQ==
\section  温度计的属性配置对话框类
*/

#ifndef TEMPERATUREDIALOG_H
#define TEMPERATUREDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"
#include "IDrawEnvironment.h"

class CColorDialog;
namespace Ui {
class CTemperatureDialog;
}

class CTemperatureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CTemperatureDialog(QWidget *parent = 0);
    ~CTemperatureDialog();

private:
    Ui::CTemperatureDialog *ui;

private:
    CDataTemperature* m_temp;   //!  温度计数据类的指针
    CPaintButton* m_tempStyle;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;
    CPaintButton* m_tempGradient;
    CPaintButton* m_tempColor1;
    CPaintButton* m_tempColor2;
    CPaintButton* m_tempHelp;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);

public:
    void setData(CData* data);    //!  将保存的温度计的各属性值显示到界面
    /*!
     \section 温度计绑定变量。
    */
    void setEnvrionment(IDrawEnvrionment *value);
private:
    IDrawEnvrionment *m_environment;

private slots:
    /*!
     \section 对温度计的各个基本属性进行配置。
    */
    void on_temArrangeBtn_clicked();
    void setW1(qint32 value);
    void setH1(qint32 value);
    void setW2(qint32 value);
    void setH2(qint32 value);
    void setOffset(qint32 value);
    void setStep(qint32 value);
    void setEdgeWid(qint32 value);
    /*!
     \section 对温度计的各个基本属性设置超限的警告。
    */
    void w1Warning();
    void h1Warning();
    void w2Warning();
    void h2Warning();
    void offsetWarning();
    void stepWarning();
    void edgeWidWarning();
    /*!
     \section 对温度计的填充样式以及边缘线的风格进行配置。
    */
    void on_edgeStyleBtn_clicked();
    void on_tempGradBtn_clicked();
    void on_bkGradBtn_clicked();
    /*!
     \section 对温度计的各部分颜色进行设置。
    */
    void on_edgeColorBtn_clicked();
    void on_tempColor1Btn_clicked();
    void on_tempColor2Btn_clicked();
    void on_bkColor1Btn_clicked();
    void on_bkColor2Btn_clicked();
    /*!
     \section 对温度计绑定变量表达式。
    */
    void setExpValue(QString str);
    void on_expBtn_clicked();
private:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // TEMPERATUREDIALOG_H
