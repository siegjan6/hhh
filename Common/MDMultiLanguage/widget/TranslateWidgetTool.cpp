#include <QByteArray>
#include "TranslateWidgetTool.h"
#include "TranslateItem.h"
////////////////////////////////////////////////////////////////////
CTranslateWidgetTool::CTranslateWidgetTool()
{
    m_translate = NULL;
    m_isSystem = true;
}
CTranslateWidgetTool::~CTranslateWidgetTool()
{
    if(m_translate != NULL)
        delete m_translate;
}
////////////////////////////////////////////////////////////////////
void CTranslateWidgetTool::init(QObject *obj, bool isSystem)
{
    m_isSystem = isSystem;
    m_translate = CTranslateItem::create(obj, this);
}
void CTranslateWidgetTool::changeLanguage()
{
    if(m_translate == NULL)
        return;

    m_translate->changeLanguage();
}
bool CTranslateWidgetTool::isValidObject(QObject *obj)
{
    bool result = true;
    for(int i = 0; i < m_execptionTypes.count(); i++)
    {
        QString type = m_execptionTypes.at(i);
        QByteArray bt = type.toLatin1();
        if(obj->inherits(bt.data()))
        {
            for(int j = 0; j < m_translateNames.count(); j++)
            {
                QString name = m_translateNames.at(j);
                if(obj->objectName().compare(name, Qt::CaseInsensitive) == 0)
                    return true;
            }

            result = false;
            break;
        }

    }

    return result;
}
bool CTranslateWidgetTool::isSeveredObject(QObject *obj)
{
    bool result = false;
    for(int i = 0; i < m_severedNames.count(); i++)
    {
        QString name = m_severedNames.at(i);
        if(obj->objectName().compare(name, Qt::CaseInsensitive) == 0)
            return true;
    }

    return result;
}
////////////////////////////////////////////////////////////////////
