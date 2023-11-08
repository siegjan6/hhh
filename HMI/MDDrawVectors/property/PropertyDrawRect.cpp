#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include "IHMIForm.h"
#include "IHMIFramework.h"
#include "PropertyDrawRect.h"

CPropertyDrawRect::CPropertyDrawRect(CMDDrawObj *parent) :
    CMDPropertyDrawVector(parent)
{
}
//////////////////////////////////////////////////////////////////////////
CMDDrawRect *CPropertyDrawRect::object()
{
    return dynamic_cast<CMDDrawRect*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
