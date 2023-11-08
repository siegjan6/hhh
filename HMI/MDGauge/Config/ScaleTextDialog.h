/*!
\author emhhbmdsZQ==
\section  刻度文本的属性配置对话框类
*/

#ifndef SCALETEXTDIALOG_H
#define SCALETEXTDIALOG_H
#include <QDialog>
#include "DrawData.h"
#include "PaintButton.h"

class CColorDialog;
namespace Ui {
class CScaleTextDialog;
}

class CScaleTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CScaleTextDialog(QWidget *parent = 0);
    ~CScaleTextDialog();

private:
    Ui::CScaleTextDialog *ui;  

protected:
    void initDialog();
     void paintEvent(QPaintEvent*);

public:
    void setData(CData* data);     //!  将保存的仪表刻度文本的各属性值显示到界面

private:
    CDataScaleText* m_text;    //!  刻度文本数据类的指针
    CPaintButton* m_textArrange;
    CPaintButton* m_textColor;

private:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
     /*!
      \section 对刻度文本的各个基本属性进行配置。
     */
    void setDecimalsV(qint32 value);
    void setRadiusV(qint32 value);
    void setSizeV(qint32 value);
    void setTextBold(qint32 state);
    void setFontName(QString str);
    /*!
     \section 对刻度文本的风格以及颜色进行配置。
    */
    void on_textStyleBtn_clicked();
    void on_colorBtn_clicked();
    /*!
     \section 对刻度文本的各个基本属性设置超限的警告。
    */
    void decimalsWarning();
    void radiusWarning();
    void sizeWarning();
};

#endif // SCALETEXTDIALOG_H
