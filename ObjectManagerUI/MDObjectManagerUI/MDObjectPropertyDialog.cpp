#include "MDObjectPropertyDialog.h"
#include "MDObjectPropertyDialogParaImp.h"
#include "MDStringResource.h"
#include <QKeyEvent>

CMDObjectPropertyDialog::CMDObjectPropertyDialog(QWidget *parent) :
    QDialog(parent)
{
    m_para.m_imp->m_twTool = CMDMultiLanguage::instance()->createWidgetTool();
}

CMDObjectPropertyDialog::~CMDObjectPropertyDialog()
{
    if(m_para.m_imp->m_twTool)
        delete m_para.m_imp->m_twTool;
}

void CMDObjectPropertyDialog::initial(void* objectUI, void *sourceProperty)
{
    m_para.m_imp->m_objectUI = objectUI;
}

bool CMDObjectPropertyDialog::UI2Property(void *destProperty)
{
    return false;
}

void* CMDObjectPropertyDialog::objectUI()
{
    return m_para.m_imp->m_objectUI;
}

void CMDObjectPropertyDialog::keyPressEvent(QKeyEvent * ke)
{
    if(Qt::Key_Escape != ke->key())
    {
        QDialog::keyPressEvent(ke);
    }
}

void CMDObjectPropertyDialog::setLanguageExceptions(const QStringList* controlTypesNoT, const QStringList* controlNamesT_InNoT)
{
    ITranslateWidgetTool* twTool = m_para.twTool();
    if(twTool)
    {
        twTool->setExceptionTypes(controlTypesNoT ? *controlTypesNoT : QStringList());
        twTool->setTranslateWidgetName(controlNamesT_InNoT ? *controlNamesT_InNoT : QStringList());
    }
}

void CMDObjectPropertyDialog::initChangeLanguage()
{
    ITranslateWidgetTool* twTool = m_para.twTool();
    if(twTool)
    {
        twTool->init(this);
        twTool->changeLanguage();
    }
}
