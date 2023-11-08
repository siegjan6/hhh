/*!

\author dGFuZ3p3

\section 功能

动作数据类：播放TTS。

*/

#ifndef APC_PLAYTTS_H
#define APC_PLAYTTS_H

#include <QString>
#include <QDateTime>
#include <QVariant>
#include "MDActionConstant.h"
#include "MDActionHelper_Global.h"
#include "MDActionProperty_Config.h"

class CAPCData_PlayTTS;
class MDACTIONHELPERSHARED_EXPORT CAPC_PlayTTS : public CMDActionProperty_Config
{
public:
    CAPC_PlayTTS();
    ~CAPC_PlayTTS();
   CAPC_PlayTTS(const CAPC_PlayTTS& obj);
   CAPC_PlayTTS& operator = (const CAPC_PlayTTS& obj);
/*!
\section 属性
*/
public:
    qint8 mode() const;
    void setMode(qint8 mode);

    qint8 repeatCount() const;
    void setRepeatCount(qint8 repeatCount);
public:
    QString ttsText() const;
    void setTtsText(const QString& ttsText);

    QString ttsTextFile() const;
    void setTtsTextFile(const QString& ttsTextFile);
public:
    virtual bool copy(const CMDActionProperty_Config* source);

/*!
\section 序列化
*/
public:
    virtual bool serialize(QDataStream& stream, bool useCurrentVersion = true);
    virtual void deserialize(QDataStream& stream);
/*!
\section 序列化：JSON格式
*/
public:
   virtual void serialize(QJsonObject& json);
   virtual void deserialize(const QJsonObject& json);
protected:
    CAPCData_PlayTTS* m_data1;
};

#endif // APC_PLAYTTS_H
