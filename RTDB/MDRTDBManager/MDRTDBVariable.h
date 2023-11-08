/*!
author luyj 2021.02.18
\section 实时数据库内部对象
*/
#ifndef CMDRTDBVariableT_H
#define CMDRTDBVariableT_H

#include <QList>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QDomElement>
#include "IMDRTDBVariable.h"
#include "IMDTreeItem.h"
#include "MDRTDBManager.h"
#include "MDExpression.h"
#include "MDActionSP.h"
#include "SortJsonObject.h"


class CMDVariableGroup;
//实时数据库变量
class CMDRTDBVariableT: public IMDRTDBVariable
{
public:
    CMDRTDBVariableT();
    virtual ~CMDRTDBVariableT();

/*!
\section IMDRTDBVariable 接口
*/
public:
    //!变量名称(全名）：GroupName.LocalName
    QString name();
    void setName(QString name);

	QString getUnit();
	void setUnit(QString unit);

	QString getDescription();
	void setDescription(QString description);

    QString getExt();
    void setExt(QString ext);

	void removeFromUa();

    //!组名
    QString groupName();
    void setGroupName(QString name);
    
    //!不带Group的名
    QString localName();
    void setLocalName(QString name);

    //!变量类型
    MDVariableType variableType();
    void setVariableType(MDVariableType varType);
    QString variableTypeDescription();

    //!数据类型
    MDDataType dataType();
    void setDataType(MDDataType dataType);
    QString dataTypeDescription();

    //!精度
    int precision();
    void setPrecision(int precision);

    //!当前值
    QVariant currentValue();
    void setCurrentValue(QVariant value);
    //!上次的值
    QVariant lastValue();
    void setLastValue(QVariant value);

    //!当前品质
    QString currentQualityDescription();
    MDVariableDataQuality  currentQuality();
    void setCurrentQuality(MDVariableDataQuality quality);

    //!上次的品质
    QString lastQualityDescription();
    MDVariableDataQuality  lastQuality();
    void setLastQuality(MDVariableDataQuality quality);

    //!时间戳
    QDateTime timeStamp();
    void setTimeStamp(QDateTime timeStamp);

    //!地址
    QString address();
    void setAddress(QString address);

    //!设备名称
    QString deviceName();
    void setDeviceName(QString deviceName);

    //!原始值数据类型
    MDDataType originalDataType();
    QString originalDataTypeDescription();
    void setOriginalDataType(MDDataType dataType);

    //!原始值
    QVariant originalValue();
    void setOriginalValue(QVariant value);

    //!初始化参数
    MDVariableInitializeParameter initializeParameter();
    void setInitializeParameter(const MDVariableInitializeParameter &parameter);

    //!品质错误参数
    MDVariableQualityIsBadParameter qualityIsBadParameter();
    void setQualityIsBadParameter(const MDVariableQualityIsBadParameter &parameter);

    //!数据转换参数
    MDVariableDataConversionParameter dataConversionParameter();
    void setDataConversionParameter(const MDVariableDataConversionParameter &parameter);

    //!变量写保护参数
    MDVariableWriteProtectParameter writeProtectParameter();
    void setWriteProtectParameter(const MDVariableWriteProtectParameter &parameter);

    //!变量限值报警参数
    MDVariableAlarmParameter variableAlarmParameter();
    void setVariableAlarmParameter(const MDVariableAlarmParameter &parameter);

    //!存档
    MDRTDataSaveRulerParameter dataSaveRulerParameter();
    void setDataSaveRulerParameter(const MDRTDataSaveRulerParameter  &dataSaveRulerParameter);
    MDRTDataSaveRuler dataSaveRuler();

    //!数据块名称
    QString dataBlockName();
    void setDataBlockName(QString dbName); 
    
    //!关联数据块编号
    int dataBlockIndexID();
    void setDataBlockIndexID(int dataBlockIndexID);

    //!关联数据区地址
    int internalAddress();
    void setInternalAddress(int internalAddress);

    //!关联数据区子地址
    int internalSubAddress();
    void setInternalSubAddress(int internalSubAddress);

    //!动作安全策略
    CMDActionSPList * actionSPList();
    void setActionSPList(CMDActionSPList *actionSPList);

    //!有效性
    bool isValid();
    void setValid(bool isValid);

    //!模拟数据
    void simulate();

    //!更新数据:返回值 true：数值变化或者品质变化
    bool refreshData(QVariant dataValue,QDateTime TimeStamp,MDVariableDataQuality quality,bool OnlyChangedQuality=false);
    qint64 changeCount();

    //!复制
    void copy(IMDRTDBVariable *otherVariable);

    //!写变量时，限值
    bool getWriteLimitValue(QVariant valueIn, QVariant& valueOut);

    void* uaBaseDataVariable() {return NULL;}
    void setUaBaseDataVariable(void* pUaBaseDataVariable){Q_UNUSED(pUaBaseDataVariable);}

private:
    QString nsDataTypeDescription(const MDDataType &dataType);
    QString qualityDescription(const MDVariableDataQuality &quality);
    //!发送变量变化到云
    void    sendChangeVariablesToCloud(QStringList& varNameList, QStringList& varValueList);
    //!发送变量变化到KV Cache
    void sendChangeVariablesToKVCache(QStringList& varNameList, QList<QVariant>& varValueList);
protected:
    QString m_name;
    QString m_groupName;
    QString m_localName;
    QVariant m_curValue;
    QVariant m_lastValue;
    QVariant m_originalValue;
    QString m_description;
    QString m_ext;
	QString m_unit;

    CMDActionSPList  m_actionSPList;

    QString m_address;
    QString m_deviceName;
    QString m_dataBlockName;
    int m_dataBlockIndexID;
    int m_internalAddress;
    int m_internalSubAddress;

    MDRTDataSaveRulerParameter  m_dataSaveRulerParameter;
    MDVariableType m_variableType;

    int m_precision;
    MDDataType m_dataType;
    MDDataType m_originalDataType;
    MDVariableDataQuality m_curQuality;
    MDVariableDataQuality m_lastQuality;
    QDateTime m_timeStamp;
    bool m_isValid;

    MDVariableInitializeParameter m_initializeParameter;
    MDVariableQualityIsBadParameter m_qualityIsBadParameter ;
    MDVariableDataConversionParameter m_dataConversionParameter;
    MDVariableWriteProtectParameter m_writeProtectParameter;
    MDVariableAlarmParameter m_variableAlarmParameter;

    qint64 m_changeCount;//变化更新次数

    QMutex m_objectMutex;
/*!
\section IMDRTDBTreeItem 接口
*/

public:
    //!TreeItem Type
        //VariableGroup = 0,
        //Variable = 1,
        //Device = 2,
        //DataBlock = 3,
        //DriverManager = 4,
    MDTreeItemType itemType();
    void setItemType(MDTreeItemType itemType);
    void *parentItem(MDTreeItemType parentItemType);
    void setParentItem(void* parent,MDTreeItemType parentItemType);
private:
    MDTreeItemType m_itemType;
    CMDVariableGroup *m_parentVariableGroup;
    IDataBlock *m_parentDataBlock;

/*!
\section 表达式等关联变量管理（中间变量）
*/
public:
    //!移除所有关联变量
    void removeAllRefrenceVariables();
    //!添加关联变量
    void addRefrenceVariable(IMDVariable* refVar);
    //!移除关联变量
    void removeRefrenceVariable(IMDVariable* refVar);
    //!获取关联变量
    IMDVariable* getRefrenceVariable(int index);
    //!获取关联变量数量
    int getRefrenceVariableCount();
    //!是否有关联变量
    bool hasRefrenceVariable();
    //!表达式计算
    void doExpression(const QString& varName,QVariant varValue);
    void doExpression();
    void initialMiddleVariableExpression();
    void initialze();
private:
    QList<IMDVariable*> m_refrenceVarList;//引用变量
    QMutex m_refrenceVarListMutex;
    CMDExpression m_varExpression;
/*!
\section MDRTDBManager
*/
public:
    void setRTDBManager(CMDRTDBManager *rtdbManager);
    CMDRTDBManager * getRTDBManager();
private:
    CMDRTDBManager *m_rtdbManager;
/*!
\section 读写变量属性
*/
public:
    void saveConfiguration(QDomDocument &doc,QDomElement &parentElement);
    void readConfiguration(QDomElement &domElement);
    void save(QDataStream &out);
    void read(QDataStream &in);
    void serialize(CSortJsonObject& json);
    virtual void deserialize(const QJsonObject &json);
public:
    bool writeValue(QVariant varValue);

/*!
\\section 关联数据块存在变量变化时通知实时数据管理器
*/
public:
    /*!
     设备多个变量变化通知
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    void notifyDeviceVariablesChange(QList<IMDRTDBVariable*>& varlist,bool OnlyChangedQuality=false);
    void dynamicInitialMiddleVariableExpression();

public:
    virtual void setValue(const QVariant &value);
};

#endif // CMDRTDBVariableT_H
