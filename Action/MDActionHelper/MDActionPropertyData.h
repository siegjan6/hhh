/*!

\author dGFuZ3p3

\section 功能

动作数据基类及其派生类的实现类。

*/

#ifndef MDACTIONPROPERTYDATA_H
#define MDACTIONPROPERTYDATA_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include "MDActionConstant.h"
#include "MDActionID.h"
#include "MDActionSP.h"
#include <QDataStream>
#include <QRect>

/*!
 * \brief 动作数据基类的实现类
 */
class CMDActionPropertyData_Config
{
public:
    CMDActionPropertyData_Config();
    CMDActionPropertyData_Config(MDActionType actionType);
     virtual ~CMDActionPropertyData_Config();
    CMDActionPropertyData_Config(const CMDActionPropertyData_Config& obj);
    CMDActionPropertyData_Config& operator = (const CMDActionPropertyData_Config& obj);
public:
    QString m_name;
    QString m_comment;
    bool m_enable ;// false;
    bool m_enableLog ;// false;
public:
    CMDActionID m_actionID;
public:
    bool m_enableDelay ;// false;
    qint32 m_delayLength ;// 1000;

    bool m_enableExecExp ;// false;
    QString m_execExp ;// "";
public:
    CMDActionSPList m_spList;
public:
    bool copy(const CMDActionPropertyData_Config* source);
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
 * \brief 写变量数据类的实现类
 */
class CAPCData_VariableWriteValue
{
public:
    CAPCData_VariableWriteValue();
    ~CAPCData_VariableWriteValue();
   CAPCData_VariableWriteValue(const CAPCData_VariableWriteValue& obj);
   CAPCData_VariableWriteValue& operator = (const CAPCData_VariableWriteValue& obj);
public:
    MDWriteMode m_mode ;// MDWriteMode::mode_fixed;
    QVariant m_value;
public:
    qint8 m_valueSource ;// 0;
    QString m_sourceName;
    bool copy(const CAPCData_VariableWriteValue* source);
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
 * \brief 播放TTS数据类的实现类
 */
class CAPCData_PlayTTS
{
public:
    CAPCData_PlayTTS();
    ~CAPCData_PlayTTS();
   CAPCData_PlayTTS(const CAPCData_PlayTTS& obj);
   CAPCData_PlayTTS& operator = (const CAPCData_PlayTTS& obj);
public:
    qint8 m_mode ;// 0;
    qint8 m_repeat ;// 1;
public:
    QString m_ttsText;
    QString m_ttsTextFile;
    bool copy(const CAPCData_PlayTTS* source);
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
 * \brief 窗口操作数据类的实现类
 */
class CAPCData_WindowOperation
{
public:
    CAPCData_WindowOperation();
    ~CAPCData_WindowOperation();
   CAPCData_WindowOperation(const CAPCData_WindowOperation& obj);
   CAPCData_WindowOperation& operator = (const CAPCData_WindowOperation& obj);
public:
    QString m_second;
    int m_left ;// 0;
    int m_top ;// 0;
    int m_width ;// 100;
    int m_height ;// 100;
    int m_pwid ;// 0
    QRect m_gRect; // x, y, w, h (0, 0, 0, 0)
    QString m_metadata; // "" extend meta param
    bool copy(const CAPCData_WindowOperation* source);
public:
    bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    void serializeOnVersion_1(QDataStream& stream);
    void serializeOnVersion_2(QDataStream& stream);
public:
    void deserialize(QDataStream& stream);
    void deserializeOnVersion_1(QDataStream& stream);
    void deserializeOnVersion_2(QDataStream& stream);
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
 * \brief 运行时属性数据类的实现类
 */
class CMDActionPropertyData_Runtime
{
public:
    CMDActionPropertyData_Runtime();
    ~CMDActionPropertyData_Runtime();
   CMDActionPropertyData_Runtime(const CMDActionPropertyData_Runtime& obj);
   CMDActionPropertyData_Runtime& operator = (const CMDActionPropertyData_Runtime& obj);
public:
    long m_exeCount ;// 0;
    QDateTime m_lastExeTime;
    CMDActionSP* m_enabledExeSP ;// NULL;
};

#endif // MDACTIONPROPERTYDATA_H
