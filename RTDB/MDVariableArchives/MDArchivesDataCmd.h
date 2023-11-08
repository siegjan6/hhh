/*!
\author enli 2015.01.08
\section 待归档变量类.存在于变量归档链表。有4属性：名字+值+品质+时间戳
*/
#ifndef MDARCHIVESDATACMD_H
#define MDARCHIVESDATACMD_H

#include "IMDRTDBVariable.h"
class CMDArchivesDataCmd
{
public:
    CMDArchivesDataCmd();
public:
    //!变量名称
    QString varName();
    void setVarName(const QString & name);
    //!当前值
    QVariant value();
    void setValue(const QVariant &value);
    //!时间戳
    QDateTime timeStamp();
    void setTimeStamp(const QDateTime & timeStamp);
    //!品质
    MDVariableDataQuality quality();
    void setQuality(const MDVariableDataQuality & quality);
    //!变量初始化模式
    MDRTInitMode initMode();
    void setInitMode(MDRTInitMode mode);
    //!变量归档模式
    MDRTDataSaveRuler saveRuler();
    void setSaveRuler(MDRTDataSaveRuler ruler);
private:
    QString m_varName;
    QVariant m_value;
    QDateTime m_timeStamp;
    MDVariableDataQuality m_quality;
    MDRTInitMode m_initMode;
    MDRTDataSaveRuler m_saveRuler;
};

#endif // MDARCHIVESDATACMD_H
