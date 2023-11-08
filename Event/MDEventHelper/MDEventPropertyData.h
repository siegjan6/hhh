/*!

\author dGFuZ3p3

\section 功能

事件数据基类的实现类 ：
CMDEventPropertyData_Config ；

具体事件数据类的实现类 ：
CEPCData_VariableValueChangedInRange
CEPCData_VariableQualityChanged
CEPCData_VariableQualityChanged
CEPCData_VariableValueWrite
CEPCData_TimeCycle
CEPCData_Combined

事件运行时的属性类的实现类 ：
CMDEventPropertyData_Runtime

*/

#ifndef MDEVENTPROPERTYDATA_H
#define MDEVENTPROPERTYDATA_H

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QVariant>
#include <QDataStream>
#include "MDEventConstant.h"
#include "MDEventHelper.h"

/*!
 * \brief 事件数据基类的实现类
 */
class CMDEventPropertyData_Config
{
public:
    CMDEventPropertyData_Config();
    CMDEventPropertyData_Config(MDEventType eventType);
     virtual ~CMDEventPropertyData_Config();
    CMDEventPropertyData_Config(const CMDEventPropertyData_Config& obj);
    CMDEventPropertyData_Config& operator = (const CMDEventPropertyData_Config& obj);
public:
    QString m_name;
    QString m_comment;
    bool m_enable ;// false;
    MDEventType m_type ;// MDEventType::null_event;
    bool m_enableLog ;// false;
public:
    QString m_objName;
public:
    QString m_expressionString;
public:
    QStringList m_actionNameList;
    bool copy(const CMDEventPropertyData_Config* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

/*!
 * \brief 变量数值变化的实现类
 */
class CEPCData_VariableValueChangedInRange
{
public:
    CEPCData_VariableValueChangedInRange();
    ~CEPCData_VariableValueChangedInRange();
   CEPCData_VariableValueChangedInRange(const CEPCData_VariableValueChangedInRange& obj);
   CEPCData_VariableValueChangedInRange& operator = (const CEPCData_VariableValueChangedInRange& obj);
public:
    CValueRangeCondition m_rangeCondition;
    bool copy(const CEPCData_VariableValueChangedInRange* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

/*!
 * \brief 变量品质变化的实现类
 */
class CEPCData_VariableQualityChanged
{
public:
    CEPCData_VariableQualityChanged();
    ~CEPCData_VariableQualityChanged();
   CEPCData_VariableQualityChanged(const CEPCData_VariableQualityChanged& obj);
   CEPCData_VariableQualityChanged& operator = (const CEPCData_VariableQualityChanged& obj);
public:
    CValueRangeCondition m_rangeCondition;
    bool copy(const CEPCData_VariableQualityChanged* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

/*!
 * \brief 变量写的实现类
 */
class CEPCData_VariableValueWrite
{
public:
    CEPCData_VariableValueWrite();
    ~CEPCData_VariableValueWrite();
   CEPCData_VariableValueWrite(const CEPCData_VariableValueWrite& obj);
   CEPCData_VariableValueWrite& operator = (const CEPCData_VariableValueWrite& obj);
public:
    quint8 m_checkConditionBeforeOrAfterWrite ;// 0;
public:
    CValueRangeCondition m_rangeCondition;
    bool copy(const CEPCData_VariableValueWrite* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

/*!
 * \brief 周期的实现类
 */
class CEPCData_TimeCycle
{
public:
    CEPCData_TimeCycle();
    ~CEPCData_TimeCycle();
   CEPCData_TimeCycle(const CEPCData_TimeCycle& obj);
   CEPCData_TimeCycle& operator = (const CEPCData_TimeCycle& obj);
public:
    quint32 m_timeCycle ;// 1000;
public:
    bool copy(const CEPCData_TimeCycle* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

/*!
 * \brief 组合的实现类
 */
class CEPCData_Combined
{
public:
    CEPCData_Combined();
    ~CEPCData_Combined();
   CEPCData_Combined(const CEPCData_Combined& obj);
   CEPCData_Combined& operator = (const CEPCData_Combined& obj);
public:
    bool copy(const CEPCData_Combined* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream, bool useCurrentVersion);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    quint8 m_version ;// 0;
};

/*!
 * \brief 事件运行时的属性类的实现类
 */
class CMDEventPropertyData_Runtime
{
public:
    CMDEventPropertyData_Runtime();
    ~CMDEventPropertyData_Runtime();
   CMDEventPropertyData_Runtime(const CMDEventPropertyData_Runtime& obj);
   CMDEventPropertyData_Runtime& operator = (const CMDEventPropertyData_Runtime& obj);
public:
    bool m_expFlag ;// false;
public:
    bool m_autoResetExpFlag ;// false;
public:
    long m_triggerCount ;// 0;
    QDateTime m_lastTriggerTime;
};

#endif // MDEVENTPROPERTYDATA_H
