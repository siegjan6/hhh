#ifndef PRINTFORM_H
#define PRINTFORM_H

#include <QDialog>
#include <QTextEdit>
#include "DrawWidget.h"

namespace Ui {
class CPrintForm;
}

class CPrintForm : public QDialog
{
    Q_OBJECT

public:
    explicit CPrintForm(QWidget *parent = 0);
    ~CPrintForm();

private:
    Ui::CPrintForm *ui;

private slots:
    //!添加自定义字符
    void on_pushButtonDate_clicked();
    void on_pushButtonTime_clicked();
    void on_pushButtonPage_clicked();
    void on_pushButtonPageCount_clicked();

    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
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
public:
    void init(CDrawWidget* drawWidget);
private:
    void saveData();
    //!添加自定义字符
    void addCustomString(const QString& custom);
private:
    CDrawWidget* m_drawWidget;
    QTextEdit* m_lastFocusInEdit;
};

#endif // PRINTFORM_H
