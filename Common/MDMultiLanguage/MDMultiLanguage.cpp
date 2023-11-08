#include "MDMultiLanguage.h"
#include "DictionaryManager.h"
#include "TranslateWidgetTool.h"
#include "LanguageToolWidget.h"

CMDMultiLanguage* CMDMultiLanguage::m_multiLanguage = NULL;
///////////////////////////////////////////////////////////////////
CMDMultiLanguage::CMDMultiLanguage()
{
    m_manager = NULL;
    m_toolWidget = new CLanguageToolWidget();
}
CMDMultiLanguage::~CMDMultiLanguage()
{
    delete m_toolWidget;
}
///////////////////////////////////////////////////////////////////
IDictionaryManager *CMDMultiLanguage::createDictionaryManager(ILanguageObject *object, const QString &projectPath)
{
    return new CDictionaryManager(object, projectPath);
}
ITranslateWidgetTool *CMDMultiLanguage::createWidgetTool()
{
    return new CTranslateWidgetTool();
}
///////////////////////////////////////////////////////////////////
CMDMultiLanguage *CMDMultiLanguage::instance()
{
    if(m_multiLanguage == NULL)
        m_multiLanguage = new CMDMultiLanguage();
    return m_multiLanguage;
}
void CMDMultiLanguage::release()
{
    if(m_multiLanguage != NULL)
    {
        delete m_multiLanguage;
        m_multiLanguage = NULL;
    }
}
void CMDMultiLanguage::setCurrentProject(IDictionaryManager *dictionaryManager)
{
    m_manager = dictionaryManager;
    m_toolWidget->init(dynamic_cast<CDictionaryManager*>(dictionaryManager));
}
///////////////////////////////////////////////////////////////////
bool CMDMultiLanguage::enable()
{
    if(m_manager == NULL)
        return false;
    return dynamic_cast<CDictionaryManager*>(m_manager)->enable();
}
QWidget *CMDMultiLanguage::toolWidget()
{
    return m_toolWidget;
}
bool CMDMultiLanguage::addWord(const QString &value, bool isSystem)
{
    if(m_manager == NULL)
        return false;
    return dynamic_cast<CDictionaryManager*>(m_manager)->addWord(value, isSystem);
}
QString CMDMultiLanguage::value(const QString &key, bool isSystem)
{
    if(m_manager == NULL)
        return key;
    return dynamic_cast<CDictionaryManager*>(m_manager)->value(key, isSystem);
}
void CMDMultiLanguage::translateWidget(QObject *obj, QStringList *types, QStringList *names, bool isSystem)
{
    ITranslateWidgetTool* t  = CMDMultiLanguage::createWidgetTool();
    if(types != NULL)
        t->setExceptionTypes(*types);
    if(names != NULL)
        t->setTranslateWidgetName(*names);
    t->init(obj, isSystem);
    t->changeLanguage();
    delete t;
}
///////////////////////////////////////////////////////////////////

