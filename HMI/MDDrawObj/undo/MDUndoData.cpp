#include "MDUndoData.h"
#include "MDDrawObj.h"
#include "DrawData.h"
////////////////////////////////////////////////////////////
CMDUndoDataObj::CMDUndoDataObj()
{
    m_obj = NULL;
    m_data = new CDataObj;

}
CMDUndoDataObj::~CMDUndoDataObj()
{
    delete m_data;
}
CMDDrawObj *CMDUndoDataObj::obj()
{
    return m_obj;
}
void CMDUndoDataObj::setObj(CMDDrawObj *value)
{
    m_obj = value;
}
IDataObj *CMDUndoDataObj::data()
{
    return m_data;
}
void CMDUndoDataObj::setData(IDataObj *value)
{
    *m_data = *dynamic_cast<CDataObj*>(value);
}
////////////////////////////////////////////////////////////
CMDUndoDataVector::CMDUndoDataVector()
{
   m_dataVector = new CDataVector;
}
CMDUndoDataVector::~CMDUndoDataVector()
{
    delete m_dataVector;
}
IDataVector *CMDUndoDataVector::dataVector()
{
    return m_dataVector;
}
void CMDUndoDataVector::setDataVector(IDataVector *value)
{
    *m_dataVector = *dynamic_cast<CDataVector*>(value);
}
////////////////////////////////////////////////////////////
CMDUndoDataMulti::~CMDUndoDataMulti()
{
    qDeleteAll(m_list);
    m_list.clear();
}
////////////////////////////////////////////////////////////
