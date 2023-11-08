#ifndef MDVARIABLECHANGEDMSG_H
#define MDVARIABLECHANGEDMSG_H

#include "MDVariableHelper_Global.h"
#include "MDVariableConstant.h"
#include "MDVariable.h"
#include <QVariant>

class CMDVariable;
class CMDVariableChangedMsgData;
class IMDVariable;
class MDVARIABLEHELPERSHARED_EXPORT CMDVariableChangedMsg
{
public:
    CMDVariableChangedMsg();
    ~CMDVariableChangedMsg();

    CMDVariableChangedMsg(const CMDVariableChangedMsg& obj);
    CMDVariableChangedMsg& operator = (const CMDVariableChangedMsg& obj);
public:
    CMDVariable* variableObject() const;
    void setVariableObject(CMDVariable* vObj);
public:
    MDVariableChangeType msgType() const;
    void setMsgType(MDVariableChangeType msgType);
public:
    QString name() const;
    void setName(const QString& name);
public:
    QVariant currentData() const;
    void setCurrentData(QVariant currentData);
public:
    QVariant prevData() const;
    void setPrevData(QVariant prevData);
public:
    MDVariableDataQuality currentQuality() const;
    void setCurrentQuality(MDVariableDataQuality currentQuality);
public:
    MDVariableDataQuality prevQuality() const;
    void setPrevQuality(MDVariableDataQuality prevQuality);
public:
    IMDVariable* IVariable() const;
    void setIVariable(IMDVariable* vObj);
public:
    quint32 handle() const;
    void setHandle(quint32 handle);

protected:
    CMDVariableChangedMsgData* m_data;
};

#endif // MDVARIABLECHANGEDMSG_H
