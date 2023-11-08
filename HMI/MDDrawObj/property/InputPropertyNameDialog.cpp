#include <QMessageBox>
#include "InputPropertyNameDialog.h"
#include "ui_InputPropertyNameDialog.h"
#include "IMDRTDBManager.h"
#include "MDRTDBManagerUI/IMDRTDBManagerUI.h"
//////////////////////////////////////////////////////////////////////////
CInputPropertyNameDialog::CInputPropertyNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CInputPropertyNameDialog)
{
    ui->setupUi(this);

    sHint = tr("提示");
    sRepeatHint = tr("名称重复");
    sInvalidHint = tr("无效名称");
    m_object = NULL;
}
CInputPropertyNameDialog::~CInputPropertyNameDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CInputPropertyNameDialog::on_pushButtonOk_clicked()
{
    setName(name().simplified().remove("@"));
    setExpression(expression().simplified());

    if (name().isEmpty())
    {
        QMessageBox box(this);
        box.setWindowTitle(sHint);
        box.setText(sInvalidHint);
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
    }
    else if (hasSameName())
    {
        QMessageBox box(this);
        box.setWindowTitle(sHint);
        box.setText(sRepeatHint);
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
    }
    else
        accept();
}
void CInputPropertyNameDialog::on_pushButtonCancel_clicked()
{
    reject();
}
void CInputPropertyNameDialog::on_pushButtonVar_clicked()
{
    IMDRTDBManagerUI* varUI = m_object->environment()->studioProject()->rtdbManagerUI();

    QStringList varNames;
    bool result = varUI->selectVariables(varNames);
    if (result)
    {
        if(varNames.length() > 0)
            setExpression(varNames.at(0));
    }
}
//////////////////////////////////////////////////////////////////////////
void CInputPropertyNameDialog::setName(const QString &value)
{
    ui->lineEditName->setText(value);
}
QString CInputPropertyNameDialog::name()
{
    return ui->lineEditName->text();
}
void CInputPropertyNameDialog::setExpression(const QString &value)
{
    ui->lineEditExpression->setText(value);
}
QString CInputPropertyNameDialog::expression()
{
    return ui->lineEditExpression->text();
}
//////////////////////////////////////////////////////////////////////////
bool CInputPropertyNameDialog::hasSameName()
{
    CMDDrawGroup* group = m_object;
    while (group->groupParent() != NULL)
        group = group->groupParent();

    return hasSameName(group, name());
}
bool CInputPropertyNameDialog::hasSameName(CMDDrawGroup *group, const QString &name)
{
    //当前组的名称可能已修改,不能从控件中获取,必须从上层窗体中获取
    if (group == m_object)
    {
        for(int i = 0; i < m_currentNames.count(); i++)
        {
            if (name.compare(m_currentNames.at(i), Qt::CaseInsensitive) == 0)
                return true;
        }
    }
    else
    {
        QList<CMDCustomProperty>* list = group->customPropertys();
        for(int i = 0; i < list->count(); i++)
        {
            CMDCustomProperty p = list->at(i);
            if (name.compare(p.name(), Qt::CaseInsensitive) == 0)
                return true;
        }
    }


    for(int i = 0; i < group->objList()->count(); i++)
    {
        if (group->objList()->at(i)->type() == DrawType::Group)
        {
            bool result = hasSameName(static_cast<CMDDrawGroup*>(group->objList()->at(i)), name);
            if (result)
                return true;
        }
    }

    return false;
}
//////////////////////////////////////////////////////////////////////////



