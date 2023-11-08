/*!
\author emhhbmdsZQ==
\section  直表指示块的属性配置对话框类
*/
#ifndef RULERMARKERDIALOG_H
#define RULERMARKERDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"
#include "IDrawEnvironment.h"

class CColorDialog;
namespace Ui {
class CRulerMarkerDialog;
}

class CRulerMarkerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRulerMarkerDialog(QWidget *parent = 0);
    ~CRulerMarkerDialog();

private:
    Ui::CRulerMarkerDialog *ui;

private:
    CRulerMarker* m_marker;   //!  直表指示块的数据类指针
    CPaintButton* m_markStyle;
    CPaintButton* m_markArrange;
    CPaintButton* m_edgeStyle;
    CPaintButton* m_edgeColor;
    CPaintButton* m_gradient;
    CPaintButton* m_color1;
    CPaintButton* m_color2;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);   //! 在对话框中的各个图标外绘制矩形框
private:
    bool eventFilter(QObject *watched, QEvent *event);
public:
    void setData(CData* data);    //!  将保存的直表指示块的各属性值显示到界面
    /*!
     \section 显示块绑定变量。
    */
    void setEnvrionment(IDrawEnvrionment *value);
private:
    IDrawEnvrionment *m_environment;

private slots:
    /*!
     \section 对直表指示块的各个基本属性进行配置。
    */
    void setMarkerWid(qint32 value);
    void setLength(qint32 value);
    void setOffset(qint32 value);
    void setStep(qint32 value);
    void setEdgeWide(qint32 value);
    /*!
     \section 对直表指示块的各个基本属性设置超限的警告。
    */
    void widWarning();
    void lenWarning();
    void offsetWarning();
    void stepWarning();
    void edgeWarning();
    /*!
     \section 对直表指示块的风格,布局以及边缘线的风格和填充风格进行设置。
    */
    void on_rMarkerStyBtn_clicked();
    void on_rMarkerarrange_clicked();
    void on_redgeStyleBtn_clicked();
    void on_rgradStyleBtn_clicked();
    /*!
     \section 对直表指示块的各部分颜色进行设置。
    */
    void on_redgeColorBtn_clicked();
    void on_rcolor1Btn_clicked();
    void on_rcolor2Btn_clicked();
    /*!
     \section 对直表指示块绑定变量表达式。
    */
    void setExpValue(QString str);
    void on_rExpBtn_clicked();
};

#endif // RULERMARKERDIALOG_H
