/*!
author enli 2015.01.08
\section 通知上层变量变化导出类
*/

#ifndef CMDCHANGEVARIABLE_H
#define CMDCHANGEVARIABLE_H

#include <QString>
#include "IMDChangeVariable.h"

//实时数据库变量
class CMDChangeVariable:public IMDChangeVariable
{
public:
    CMDChangeVariable();
    virtual ~CMDChangeVariable();
/*!
\section 接口
*/
public:
    //!变量名称(全名）：GroupName.LocalName
    QString name();
    void setName(QString name);

	//!单位
	QString getUnit();
	void setUnit(QString unit);

    //!描述
    QString getDescription();
    void setDescription(QString description);

    //!扩展
    QString getExt();
    void setExt(QString ext);

    //!数据类型
    MDDataType dataType();
    void setDataType(MDDataType dataType);

    //!当前值
    QVariant currentValue();
    void setCurrentValue(QVariant value);

    //!上次的值
    QVariant lastValue();
    void setLastValue(QVariant value);

    //!当前品质
    MDVariableDataQuality  currentQuality();
    void setCurrentQuality(MDVariableDataQuality quality);

    //!上次的品质
    MDVariableDataQuality  lastQuality();
    void setLastQuality(MDVariableDataQuality quality);


    //!时间戳
    QDateTime timeStamp();
    void setTimeStamp(QDateTime timeStamp);

    //!变化通知类型
    void  setVariableChangeType(VariableChangeType varChangeType);
    VariableChangeType  variableChangeType();

    //!归档属性
    MDRTDataSaveRuler dataSaveRuler();

private:
    QString m_name;
    QString m_unit;
    QString m_description;
    QString m_ext;
    QVariant m_curValue;
    QVariant m_lastValue;
    MDDataType m_dataType;
    MDVariableDataQuality m_curQuality;
    MDVariableDataQuality m_lastQuality;
    QDateTime m_timeStamp;
    VariableChangeType m_variableChangeType;
    MDRTDataSaveRuler m_variableSaveRuler;

/*!
\section 操作符重载
*/
public:
    void copy(IMDChangeVariable *otherChangeVariable);
    void copyEx(IMDVariable *otherVariable);
};
#endif // CMDCHANGEVARIABLE_H
