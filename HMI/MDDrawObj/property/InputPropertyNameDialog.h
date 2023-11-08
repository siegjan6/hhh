/*!
\author aGRw 2015.06.04
\section 自定义属性名称输入对话框
*/
#ifndef INPUTPROPERTYNAMEDIALOG_H
#define INPUTPROPERTYNAMEDIALOG_H

#include <QDialog>
#include "MDDrawGroup.h"

namespace Ui {
class CInputPropertyNameDialog;
}

class CInputPropertyNameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CInputPropertyNameDialog(QWidget *parent = 0);
    ~CInputPropertyNameDialog();
private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonVar_clicked();
private:
    Ui::CInputPropertyNameDialog *ui;
//////////////////////////////////////////////////////////////////////////
private:
    QString sHint;
    QString sRepeatHint;
    QString sInvalidHint;
//////////////////////////////////////////////////////////////////////////
public:
    void setName(const QString& value);
    QString name();
    void setExpression(const QString& value);
    QString expression();
    void setDrawObject(CMDDrawGroup* value){m_object = value;}
    //!当前组的名称可能已修改,不能从控件中获取,必须从上层窗体中动态获取
    void setCurrentNames(const QStringList& value){m_currentNames = value;}
private:
    CMDDrawGroup* m_object;
    QStringList m_currentNames;
//////////////////////////////////////////////////////////////////////////
private:
    //!判断是否存在同名
    bool hasSameName();
    bool hasSameName(CMDDrawGroup* group, const QString& name);


};

#endif // INPUTPROPERTYNAMEDIALOG_H
