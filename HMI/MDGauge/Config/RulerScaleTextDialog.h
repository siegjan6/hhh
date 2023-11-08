/*!
\author emhhbmdsZQ==
\section  直表刻度文本的属性配置对话框类
*/
#ifndef RULERSCALETEXTDIALOG_H
#define RULERSCALETEXTDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CRulerScaleTextDialog;
}

class CRulerScaleTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CRulerScaleTextDialog(QWidget *parent = 0);
    ~CRulerScaleTextDialog();

private:
    Ui::CRulerScaleTextDialog *ui;

private:
    CRulerScaleText* m_rText;   //!   直表刻度文本数据类指针
    CPaintButton* m_rTextArrange;
    CPaintButton* m_rTextColor;

protected:
    void initDialog();
    void paintEvent(QPaintEvent*);
public:
    void setData(CData* data);      //!  将保存的直表刻度文字的各属性值显示到界面

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    /*!
     \section 对直表刻度文字的各个基本属性进行配置。
    */
    void setrDecimals(qint32 value);
    void setrOffset(qint32 value);
    void setrSize(qint32 value);
    /*!
     \section 对直表刻度文字的各个基本属性设置超限的警告。
    */
    void decimalsWarning();
    void offsetWarning();
    void sizeWarning();

    void on_rTextStyleBtn_clicked();   //!  直表刻度文字的风格设置
    void setFontName(QString str);      //! 直表刻度文字的字体设置
    void setTextBold(qint32 state);     //! 刻度文字是否加粗设置
    /*!
     \section 对直表刻度文字的颜色设置。
    */
    void on_rColorBtn_clicked();
};

#endif // RULERSCALETEXTDIALOG_H
