/*!
\author aGRw 2015.11.16
\section undo数据类
*/
#ifndef UNDODATA_H
#define UNDODATA_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QList>
#include "IDrawData.h"

class CMDDrawObj;
class CDataObj;
class CDataVector;

/*!
\section redo操作时需要的obj数据
*/
class MDDRAWOBJSHARED_EXPORT CMDUndoDataObj : public IUndoDataObj
{
public:
    CMDUndoDataObj();
    virtual ~CMDUndoDataObj();
public:
    CMDDrawObj* obj();
    void setObj(CMDDrawObj* value);
    IDataObj* data();
    void setData(IDataObj* value);
private:
    CMDDrawObj* m_obj;
    CDataObj* m_data;
};

/*!
\section redo操作时需要的vector数据
*/
class MDDRAWOBJSHARED_EXPORT CMDUndoDataVector : public CMDUndoDataObj
{
public:
    CMDUndoDataVector();
    virtual ~CMDUndoDataVector();
public:
    IDataVector* dataVector();
    void setDataVector(IDataVector* value);
private:
    CDataVector* m_dataVector;
};

/*!
\section redo操作时需要的group数据
*/
class MDDRAWOBJSHARED_EXPORT CMDUndoDataMulti : public CMDUndoDataVector
{
public:
    virtual ~CMDUndoDataMulti();
public:
    QList<IUndoDataObj*>* list(){return &m_list;}
private:
    QList<IUndoDataObj*> m_list;
};

#endif // UNDODATA_H
