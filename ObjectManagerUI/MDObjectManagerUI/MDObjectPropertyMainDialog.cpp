#include "MDObjectPropertyMainDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include "MDObjectPropertyDialog.h"
#include "MDObjectPropertyMainDialogParaImp.h"
#include "MDStringResource.h"

CMDObjectPropertyMainDialog::CMDObjectPropertyMainDialog(QWidget *parent)
    : QDialog(parent)
{
    CMDObjectPropertyMainDialogParaImp* p = m_para.m_imp;

    p->initialUI(this);
    p->m_twTool = CMDMultiLanguage::instance()->createWidgetTool();
}

CMDObjectPropertyMainDialog::~CMDObjectPropertyMainDialog()
{
    CMDObjectPropertyMainDialogParaImp* p = m_para.m_imp;

    if(p->m_destProperty)
        delete p->m_destProperty;

    if(p->m_twTool)
        delete p->m_twTool;
}

/*!
\section 初始化
*/
void CMDObjectPropertyMainDialog::initial(void* objectUI, void* configer, qint16 subType)
{
    CMDObjectPropertyMainDialogParaImp* p = m_para.m_imp;

    p->m_destProperty = newProperty(subType, NULL);
    helper_init(objectUI, configer, NULL);
}

void CMDObjectPropertyMainDialog::initial(void* objectUI, void* configer, void *sourceProperty)
{
    CMDObjectPropertyMainDialogParaImp* p = m_para.m_imp;

    if(sourceProperty)
        p->m_destProperty = newProperty(-1, sourceProperty);
    helper_init(objectUI, configer, sourceProperty);
}

void CMDObjectPropertyMainDialog::helper_init(void* objectUI, void* configer, void *sourceProperty)
{
    CMDObjectPropertyMainDialogParaImp* p = m_para.m_imp;

    p->m_configer = configer;
    p->m_sourceProperty = sourceProperty;

    initPropertyManagerUI(p->m_listWidget, p->m_stackWidget,
                                                sourceProperty, p->m_destProperty);
    initChangeLanguage();

    int propertyCount = p->m_stackWidget->count();
    for(int i = 0; i < propertyCount; i++)
    {
        CMDObjectPropertyDialog* dialog = (CMDObjectPropertyDialog*)p->m_stackWidget->widget(i);
        if(sourceProperty)
            dialog->initial(objectUI, sourceProperty);
        else if(p->m_destProperty)
            dialog->initial(objectUI, p->m_destProperty);
        else{};

        dialog->initChangeLanguage();
    }
}

void CMDObjectPropertyMainDialog::initChangeLanguage()
{
    CMDObjectPropertyMainDialogParaImp* p = m_para.m_imp;
    p->m_twTool->init(this);
    p->m_twTool->changeLanguage();
}

/*!
\section 处理对 OK 和 Cancel 按钮的点击
*/
void CMDObjectPropertyMainDialog::on_okButton_clicked()
{
    CMDObjectPropertyMainDialogParaImp* p = m_para.m_imp;

    if(!p->m_destProperty)
        return;

    int propertyCount = p->m_stackWidget->count();
    for(int i = 0; i < propertyCount; i++)
    {
        CMDObjectPropertyDialog* dialog = (CMDObjectPropertyDialog*)p->m_stackWidget->widget(i);
        if(!dialog->UI2Property(p->m_destProperty))
            return;
    }

    QString resultMessage;
    bool result = false;
    if(p->m_sourceProperty)
         result = modifyProperty(p->m_configer, p->m_sourceProperty, p->m_destProperty, resultMessage);
    else
         result = addProperty(p->m_configer, p->m_destProperty, resultMessage);
    if(!result)
    {
        if(p->m_sourceProperty)
             QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_FAIL_TO_CONFIG) + resultMessage, QMessageBox::Ok);
        else
             QMessageBox::warning(NULL, STRANS(STRING_ERROR), STRANS(STRING_FAIL_TO_NEW) + resultMessage, QMessageBox::Ok);

        return;
    }

    setResult(QDialog::Accepted);
    accept();
}

void CMDObjectPropertyMainDialog::on_cancelButton_clicked()
{
    setResult(QDialog::Rejected);
    reject();
}

/*!
\section 可重载的接口
*/
void* CMDObjectPropertyMainDialog::newProperty(qint16 subType, void* sourceProperty)
{
    return NULL;
}

void CMDObjectPropertyMainDialog::initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty)
{
}

bool CMDObjectPropertyMainDialog::modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage)
{
    return false;
}

bool CMDObjectPropertyMainDialog::addProperty(void* configer, void* destProperty, QString& resultMessage)
{
    return false;
}
