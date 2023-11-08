/*!
\author Y2hvdWps 2016.09.13
\section 打印设置对话框
*/
#ifndef PRINTCONFIGDIALOG_H
#define PRINTCONFIGDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QPrinter>

typedef struct printpara{
    //!打印参数
    bool printHorizontalView;
    bool printVerticalView;
    bool printScale;
    qint32 printTop;
    qint32 printBottom;
    qint32 printLeft;
    qint32 printRight;
    qint32 printHeader;
    qint32 printFooter;
    QString printLeftHeader;
    QString printMiddleHeader;
    QString printRightHeader;
    QString printLeftFooter;
    QString printMiddleFooter;
    QString printRightFooter;
    QString printDateFormat;
    QString printTimeFormat;
}PrintPara;

namespace Ui {
class CPrintConfigDialog;
}

class CPrintConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPrintConfigDialog(const PrintPara& printPara, QWidget *parent = 0);
    virtual ~CPrintConfigDialog();

    static void setDefault(PrintPara& printPara);
    PrintPara m_printPara;
private slots:
    //!添加自定义字符
    void on_pushButtonDate_clicked();
    void on_pushButtonTime_clicked();
    void on_pushButtonPage_clicked();
    void on_pushButtonPageCount_clicked();

    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CPrintConfigDialog *ui;
//////////////////////////////////////////////////////////////
protected:
    bool eventFilter(QObject *o, QEvent *e);
//////////////////////////////////////////////////////////////
private:
    QString sDate;
    QString sTime;
    QString sPage;
    QString sPageCount;
//////////////////////////////////////////////////////////////
private:
    void initWidgets();
    void saveData();
    //!添加自定义字符
    void addCustomString(const QString& custom);
private:
    QTextEdit* m_lastFocusInEdit;
};

#endif // PRINTCONFIGDIALOG_H
