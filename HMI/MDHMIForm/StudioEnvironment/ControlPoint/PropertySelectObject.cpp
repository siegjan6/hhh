#include "PropertySelectObject.h"
#include "MDDrawObj.h"
#include "IStudio.h"
#include "IObjectChangedEvent.h"
#include "IHMIStudio.h"
#include "SelectObjectCommand.h"

//////////////////////////////////////////////////////////////////////////
CPropertySelectObject::CPropertySelectObject(CSelectObjectManager *parent)
{
    m_object = parent;

    initTranslateMap();
}
//////////////////////////////////////////////////////////////////////////
QRectF CPropertySelectObject::rect()
{
    return m_object->rect();
}
void CPropertySelectObject::setRect(const QRectF &value)
{
    IHMIForm* form = m_object->list()->at(0)->parent();
    form->studio()->undo()->push(
        new CSelectObjectCommand(m_object, m_object->rect(), value, (int)SelectObjectId::Rect));
    form->setModify(true);
}
//////////////////////////////////////////////////////////////////////////
void CPropertySelectObject::initTranslateMap()
{
    m_translateMap.insert("GUID_Layout", tr("布局"));
    m_translateMap.insert("rect", tr("大小"));
}
//////////////////////////////////////////////////////////////////////////

