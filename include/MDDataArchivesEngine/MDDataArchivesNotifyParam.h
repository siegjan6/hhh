#ifndef MDDATAARCHIVESNOTIFYPARAM_H
#define MDDATAARCHIVESNOTIFYPARAM_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据库操作具体修改项的值，用于外部调用进行组合。
2.对外接口的封闭类，具体实现在CDataArchivesNotifyParam类中。
3.IMDDataItem为每项的值，此类的定义在数据组工程中，通过设置此类的value来修改数据库相应项的值。

*/

#include "mddataarchivesengine_global.h"

#include <QList>
#include <QString>

class IMDDataItem;
class CDataArchivesNotifyParam;

class QDateTime;

namespace DataArchives {
//消息类型
enum DB_NOTIFY_TYPE
{
    DB_NOTIFY_ADD			=	0,
    DB_NOTIFY_UPDATE		=	1,
    DB_NOTIFY_DELETE		=	2,
    DB_NOTIFY_ALM_OFF		=	3,
    DB_NOTIFY_SQL           =	4,
};

}

class MDDATAARCHIVESENGINESHARED_EXPORT CMDDataArchivesNotifyParam
{
public:
    CMDDataArchivesNotifyParam();
    CMDDataArchivesNotifyParam(const CMDDataArchivesNotifyParam &otehrObj);
    CMDDataArchivesNotifyParam &operator=(const CMDDataArchivesNotifyParam &otherObj);
    ~CMDDataArchivesNotifyParam();

    /*!
    \section 设置/获取相应的属性
    */
public:

    QString dataGroupName();
    void setDataGroupName(const QString &name); //! 设置/获取数据组名

    void setRecordDateTime(QDateTime datetime);
    QDateTime recordDateTime(); //! 设置/获取记录时间

    void setRecordMillisecond(int recordMS);
    int recordMillisecond(); //! 设置/获取记录毫秒

    void setSql(QString sql);
    QString sql(); //! 设置/获取sql

    /*!
    \section 增加、删除、获取IMDDataItem，用于操作通知需要修改的数据库字段
    */
public:
    bool addItem(IMDDataItem *item); //! 增加修改项
    bool removeItem(IMDDataItem *item); //! 删除修改项
    void removeAllItem(); //! 删除所有修改项
    IMDDataItem *getItem(const QString &name); //! 获取指定名字的修改项
    IMDDataItem *getItem(int index); //! 获取指定的位置的修改项
    int itemCount(); //! 获取所有修改项总数
private:
    CDataArchivesNotifyParam *m_pNotifyParam; //! 内部真正实现类
};

#endif // MDDATAARCHIVESNOTIFYPARAM_H
