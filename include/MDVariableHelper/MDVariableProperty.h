#ifndef MDVARIABLEPROPERTY_H
#define MDVARIABLEPROPERTY_H

#include <QVariant>
#include "MDVariableHelper_Global.h"
#include "MDVariableConstant.h"
#include "../../include/MDRTDBInterfaces/IMDVariable.h"

class CMDVariablePropertyData_Config;
class MDVARIABLEHELPERSHARED_EXPORT CMDVariableProperty_Config
{
public:
    CMDVariableProperty_Config();
     virtual ~CMDVariableProperty_Config();
    CMDVariableProperty_Config(const CMDVariableProperty_Config& obj);
    CMDVariableProperty_Config& operator = (const CMDVariableProperty_Config& obj);
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);

    bool isEnable() const;
    void setEnable(bool enable);

    quint8 precision() const;
    void setPrecision(quint8 precision);

    MDVariableDataType dataType() const;
    void setDataType(MDVariableDataType type);    

    bool isEnableScript() const;
    void setEnableScript(bool enable);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void deserialize(QDataStream& stream);
protected:
    CMDVariablePropertyData_Config* m_data;
};

class CMDVariablePropertyData_Runtime;
class MDVARIABLEHELPERSHARED_EXPORT CMDVariableProperty_Runtime
{
public:
    CMDVariableProperty_Runtime();
     virtual ~CMDVariableProperty_Runtime();
    CMDVariableProperty_Runtime(const CMDVariableProperty_Runtime& obj);
    CMDVariableProperty_Runtime& operator = (const CMDVariableProperty_Runtime& obj);
public:
    QVariant data() const;
    void setData(QVariant data);

    MDVariableDataQuality quality() const;
    void setQuality(MDVariableDataQuality quality);

    void* projectExpert() const;
    void setProjectExpert(void* ptr);
protected:
    CMDVariablePropertyData_Runtime* m_data;
};

#endif // MDVARIABLEPROPERTY_H
