#include "MDPropertyDrawObj.h"
#include "BindVarDialog.h"
#include "IObjectChangedEvent.h"
#include "IHMIStudio.h"
#include "MDActionUI.h"
#include "MDHMIActionManager.h"
#include "IObjName.h"

CMDPropertyDrawObj::CMDPropertyDrawObj(CMDDrawObj *parent)
{
    Q_ASSERT(parent != NULL);

    m_object = parent;

    insertTranslateMap("GUID_Variable", tr("变量"));
    insertTranslateMap("GUID_Layout", tr("布局"));
    insertTranslateMap("GUID_Common", tr("通用"));
    insertTranslateMap("GUID_Action", tr("行为"));
    insertTranslateMap("GUID_Appearance", tr("外观"));

    insertTranslateMap("rect", tr("大小"));
    insertTranslateMap("layer", tr("层"));
    insertTranslateMap("name", tr("名称"));

    insertTranslateMap("showSelectedFrame", tr("显示选中框"));
    insertTranslateMap("variable", tr("绑定变量"));
    insertTranslateMap("toolTip", tr("提示信息"));
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawObj::setRect(const QRectF &value)
{
    if(isBatch())
        setBatchRect(value);
    else
        undo()->push(new CMDDrawObjCommand(m_object, m_object->rect(), value, (int)DrawObjId::Rect));

    m_object->parent()->studio()->statusLabelChanged();
    setModify(true);
}
QRectF CMDPropertyDrawObj::rect()
{
    return m_object->rect();
}
void CMDPropertyDrawObj::setLayer(int value)
{
    if(isBatch())
        setBatchLayer(value);
    else
        undo()->push(new CMDDrawObjCommand(m_object, m_object->layer(), value, (int)DrawObjId::Layer));

    setModify(true);
}
int CMDPropertyDrawObj::layer()
{
    return m_object->layer();
}
void CMDPropertyDrawObj::setVariable(const QString &/*value*/)
{
    QList<QString>* oldValue = new QList<QString>();
    QList<CMDExpInfo*>* list = m_object->expManager()->list();
    for(int i = 0; i < list->count(); i++)
        oldValue->append(list->at(i)->expression());

    CBindVarDialog dlg(list, m_object->environment(), "", container());
    if (dlg.exec() == QDialog::Accepted)
    {
        QList<QString>* newValue = new QList<QString>();
        for(int i = 0; i < list->count(); i++)
            newValue->append(list->at(i)->expression());

        undo()->push(new CMDDrawObjCommand(m_object,
                                           (qulonglong)oldValue, (qulonglong)newValue, (int)DrawObjId::Variable));
        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDPropertyDrawObj::variable()
{
    QString str;
    return str.setNum(m_object->expManager()->expressionCount());
}
void CMDPropertyDrawObj::setName(const QString &value)
{
    undo()->push(new CMDDrawObjCommand(m_object, m_object->name(), value, (int)DrawObjId::Name));
    setModify(true);
}
QString CMDPropertyDrawObj::name()
{
    return m_object->name();
}
void CMDPropertyDrawObj::setShowSelectedFrame(bool value)
{
    if(isBatch())
        setBatchShowSelectedFrame(value);
    else
        undo()->push(new CMDDrawObjCommand(m_object, m_object->showSelectedFrame(), value, (int)DrawObjId::ShowSelectedFrame));
    setModify(true);
}
bool CMDPropertyDrawObj::showSelectedFrame()
{
    return m_object->showSelectedFrame();
}
void CMDPropertyDrawObj::setToolTip(const QString &value)
{
    if(isBatch())
        setBatchToolTip(value);
    else
        m_object->setToolTip(value);
    setModify(true);
}
QString CMDPropertyDrawObj::toolTip()
{
    return m_object->toolTip();
}
//////////////////////////////////////////////////////////////////////////
QUndoStack *CMDPropertyDrawObj::undo()
{
    return m_object->parent()->studio()->undo();
}
QWidget *CMDPropertyDrawObj::container()
{
    return dynamic_cast<QWidget*>(m_object->parent());
}
CPropertyBrowser *CMDPropertyDrawObj::propertyGrid()
{
    return static_cast<CPropertyBrowser*>(m_object->parent()->studio()->property());
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawObj::setModify(bool value)
{
    m_object->parent()->setModify(value);
}
void CMDPropertyDrawObj::insertTranslateMap(const QString &id, const QString &name)
{
    m_translateMap.insert(id, name);
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawObj::appendBatchObjs(QList<CMDDrawObj *> *objs)
{
    m_batchObjs.append(*objs);
}
bool CMDPropertyDrawObj::isBatch()
{
    return m_batchObjs.count() > 0;
}
void CMDPropertyDrawObj::resetBatchProperty(CPropertyBrowser *browser)
{
    if(!isBatch())
        return;

    browser->setSinglePropertyHide("GUID_Variable", true);
    browser->setSinglePropertyHide("GUID_Common", true);
}
void CMDPropertyDrawObj::invalidateBatchControlPoint()
{
    m_object->parent()->studio()->invalidateBatchControlPoint();
}
CMDPropertyDrawObj::RectType CMDPropertyDrawObj::getRectChange(const QRectF &v1, const QRectF &v2)
{
    const qreal Precsion = 0.0001;
    if(qAbs(v1.left() - v2.left()) > Precsion)
        return RectType::X;
    if(qAbs(v1.top() - v2.top()) > Precsion)
        return RectType::Y;
    if(qAbs(v1.width() - v2.width()) > Precsion)
        return RectType::Width;
    if(qAbs(v1.height() - v2.height()) > Precsion)
        return RectType::Height;

    return RectType::X;
}
CMDPropertyDrawObj::PointType CMDPropertyDrawObj::getPointChange(const QPointF &v1, const QPointF &v2)
{
    const qreal Precsion = 0.0001;
    if(qAbs(v1.x() - v2.x()) > Precsion)
        return PointType::X;
    if(qAbs(v1.y() - v2.y()) > Precsion)
        return PointType::Y;

    return PointType::X;
}

CMDPropertyDrawObj::FontType CMDPropertyDrawObj::getFontChange(const QFont &v1, const QFont &v2)
{
    int changeCount = 0;
    FontType type = FontType::Family;

    if(v1.family().compare(v2.family(), Qt::CaseInsensitive) != 0)
    {
        type = FontType::Family;
        changeCount++;
    }
    if(v1.pointSize() != v2.pointSize())
    {
        type = FontType::PointSize;
        changeCount++;
    }
    if(v1.bold() != v2.bold())
    {
        type = FontType::Bold;
        changeCount++;
    }
    if(v1.italic() != v2.italic())
    {
        type = FontType::Italic;
        changeCount++;
    }
    if(v1.underline() != v2.underline())
    {
        type = FontType::Underline;
        changeCount++;
    }
    if(v1.strikeOut() != v2.strikeOut())
    {
        type = FontType::Strikeout;
        changeCount++;
    }
    if(v1.kerning() != v2.kerning())
    {
        type = FontType::Kerning;
        changeCount++;
    }

    if(changeCount > 1)
        return FontType::All;

    return type;
}
//////////////////////////////////////////////////////////////////////////
void CMDPropertyDrawObj::setBatchRect(const QRectF &value)
{
    undo()->beginMacro("");

    RectType type = getRectChange(value, m_object->rect());
    foreach(CMDDrawObj* o, m_batchObjs)
    {
        QRectF rf = o->rect();
        switch(type)
        {
        case RectType::X:
            rf.moveLeft(value.left());
            break;
        case RectType::Y:
            rf.moveTop(value.top());
            break;
        case RectType::Width:
            rf.setWidth(value.width());
            break;
        case RectType::Height:
            rf.setHeight(value.height());
            break;
        default:
            break;
        }

        undo()->push(new CMDDrawObjCommand(o, o->rect(), rf, (int)DrawObjId::Rect));
    }

    undo()->endMacro();
    invalidateBatchControlPoint();
}
void CMDPropertyDrawObj::setBatchLayer(int value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, m_batchObjs)
        undo()->push(new CMDDrawObjCommand(o, o->layer(), value, (int)DrawObjId::Layer));

    undo()->endMacro();
}
void CMDPropertyDrawObj::setBatchShowSelectedFrame(bool value)
{
    undo()->beginMacro("");

    foreach(CMDDrawObj* o, m_batchObjs)
        undo()->push(new CMDDrawObjCommand(o, o->showSelectedFrame(), value, (int)DrawObjId::ShowSelectedFrame));

    undo()->endMacro();
}
void CMDPropertyDrawObj::setBatchToolTip(const QString &value)
{
    foreach(CMDDrawObj* o, m_batchObjs)
        o->setToolTip(value);
}
//////////////////////////////////////////////////////////////////////////

