#include "FormCommand.h"
#include "MDHMIForm.h"
#include "MDColorManager/MDBrushManager.h"
#include "Studio.h"

CFormCommand::CFormCommand(CMDHMIForm *obj,
    const QVariant &oldValue, const QVariant &newValue, int id, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    Q_ASSERT(obj != NULL);

    m_form = obj;
    m_studio = dynamic_cast<CStudio*>(obj->studio());
    m_oldValue = oldValue;
    m_newValue = newValue;
    m_id = (int)id;
}

CFormCommand::~CFormCommand()
{
    switch (m_id)
    {
    case (int)FormId::Brush:
        delete reinterpret_cast<CMDBrushManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDBrushManager*>(m_newValue.toULongLong());
        break;
    case (int)FormId::Layer:
    {
        QList<QVariant> list = m_oldValue.toList();
        delete reinterpret_cast<QVector<bool>*>(list.at(0).toULongLong());
        delete reinterpret_cast<QVector<bool>*>(list.at(1).toULongLong());
        list = m_newValue.toList();
        delete reinterpret_cast<QVector<bool>*>(list.at(0).toULongLong());
        delete reinterpret_cast<QVector<bool>*>(list.at(1).toULongLong());
    }
        break;
    case (int)FormId::MouseDownAction:
    case (int)FormId::MouseUpAction:
    case (int)FormId::RightMosueDownAction:
    case (int)FormId::RightMouseUpAction:
    case (int)FormId::OpenAction:
    case (int)FormId::CloseAction:
        delete reinterpret_cast<CMDHMIActionManager*>(m_oldValue.toULongLong());
        delete reinterpret_cast<CMDHMIActionManager*>(m_newValue.toULongLong());
        break;
    case (int)FormId::Security:
    {
        QList<QVariant> list = m_oldValue.toList();
        delete reinterpret_cast<CMDActionSP*>(list.at(0).toULongLong());
        delete reinterpret_cast<CMDActionSP*>(list.at(1).toULongLong());
        list = m_newValue.toList();
        delete reinterpret_cast<CMDActionSP*>(list.at(0).toULongLong());
        delete reinterpret_cast<CMDActionSP*>(list.at(1).toULongLong());
    }
        break;
    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////
void CFormCommand::undo()
{
    setValue(m_oldValue);
}
void CFormCommand::redo()
{
    setValue(m_newValue);
}
bool CFormCommand::mergeWith(const QUndoCommand *command)
{
    if (command->id() != m_id)
        return false;
    switch (m_id)
    {
    case (int)FormId::Brush:
    case (int)FormId::Layer:
    case (int)FormId::MouseDownAction:
    case (int)FormId::MouseUpAction:
    case (int)FormId::RightMosueDownAction:
    case (int)FormId::RightMouseUpAction:
    case (int)FormId::OpenAction:
    case (int)FormId::CloseAction:
    case (int)FormId::Security:
        return false;
    default:
        break;
    }

    const CFormCommand *other = static_cast<const CFormCommand*>(command);

    m_newValue = other->m_newValue;
    return true;
}
////////////////////////////////////////////////////////////////////////
void CFormCommand::setValue(const QVariant &value)
{
    switch(m_id)
    {
    case (int)FormId::Rect:
        m_form->setRect(value.toRect());
        break;
    case (int)FormId::Brush:
        m_studio->undoBrush(reinterpret_cast<CMDBrushManager*>(value.toULongLong()));
        break;
    case (int)FormId::Layer:
    {
        QList<QVariant> list = value.toList();
        QVector<bool>* visibles = reinterpret_cast<QVector<bool>*>(list.at(0).toULongLong());
        QVector<bool>* lockeds = reinterpret_cast<QVector<bool>*>(list.at(1).toULongLong());
        m_studio->undoLayer(visibles, lockeds);
    }
        break;
    case (int)FormId::MouseDownAction:
    case (int)FormId::MouseUpAction:
    case (int)FormId::RightMosueDownAction:
    case (int)FormId::RightMouseUpAction:
    case (int)FormId::OpenAction:
    case (int)FormId::CloseAction:
        m_studio->undoAction(m_id, reinterpret_cast<CMDHMIActionManager*>(value.toULongLong()));
        break;
    case (int)FormId::Text:
        m_studio->undoText(value.toString());
        break;
    case (int)FormId::FormType:
    {
        QList<QVariant> list = value.toList();
        int type = list.at(0).toInt();
        bool hasFrame = list.at(1).toBool();
        bool fullScreen = list.at(2).toBool();
        m_studio->undoFormType(type, hasFrame, fullScreen);
    }
        break;
    case (int)FormId::Security:
    {
        QList<QVariant> list = value.toList();
        m_studio->undoSecurity(
            reinterpret_cast<CMDActionSP*>(list.at(0).toULongLong()),
            reinterpret_cast<CMDActionSP*>(list.at(1).toULongLong()));
    }
        break;

    default:
        break;
    }
}
////////////////////////////////////////////////////////////////////////
