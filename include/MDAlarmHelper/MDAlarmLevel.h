/*!

\author dGFuZ3p3

\section 功能

报警等级与报警颜色数据类。

*/

#ifndef MDALARMLEVEL_H
#define MDALARMLEVEL_H

#include <QtGui/QRgb>
#include "MDAlarmHelper_Global.h"
#include <QJsonObject>

/*!
 * \brief 报警颜色数据类
 */
class CMDAlarmColorImp;
class MDALARMHELPERSHARED_EXPORT CMDAlarmColor
{
public:
    CMDAlarmColor();
    CMDAlarmColor(QRgb f, QRgb b);
     virtual ~CMDAlarmColor();
    CMDAlarmColor(const CMDAlarmColor& obj);
    CMDAlarmColor& operator = (const CMDAlarmColor& obj);
/*!
\section 属性
*/
public:
    QRgb f() const;
    void setF(QRgb f);
    QRgb b() const;
    void setB(QRgb b);
public:
    virtual bool copy(const CMDAlarmColor* source);

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
    CMDAlarmColorImp* m_data;
};

/*!
 * \brief 报警等级数据类
 */
class CMDAlarmLevelImp;
class MDALARMHELPERSHARED_EXPORT CMDAlarmLevel
{
public:
    CMDAlarmLevel();
     virtual ~CMDAlarmLevel();
    CMDAlarmLevel(const CMDAlarmLevel& obj);
    CMDAlarmLevel& operator = (const CMDAlarmLevel& obj);
/*!
\section 属性
*/
public:
    QString name() const;
    void setName(const QString& name);

    QString comment() const;
    void setComment(const QString& comment);

    qint32 delayLength() const;
    void setDelayLength(qint32 delayLength);
public:
    CMDAlarmColor& color_occur() const;
    void setColor_occur(const CMDAlarmColor& color);

    CMDAlarmColor& color_confirm_not_end() const;
    void setColor_confirm_not_end(const CMDAlarmColor& color);

    CMDAlarmColor& color_confirm_end() const;
    void setColor_confirm_end(const CMDAlarmColor& color);

    CMDAlarmColor& color_end_not_confirm() const;
    void setColor_end_not_confirm(const CMDAlarmColor& color);

    CMDAlarmColor& color_end_confirm() const;
    void setColor_end_confirm(const CMDAlarmColor& color);
public:
    virtual bool copy(const CMDAlarmLevel* source);

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
    CMDAlarmLevelImp* m_data;
};

#endif // MDALARMLEVEL_H
