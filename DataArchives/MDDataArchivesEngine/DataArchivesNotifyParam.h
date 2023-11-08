#ifndef DATAARCHIVESNOTIFYPARAM_H
#define DATAARCHIVESNOTIFYPARAM_H

/*!

\author bHVvY2hhbw==

\section 功能
1.CMDDataArchivesNotifyParam类的内部实现类。
2.IMDDataItem为每项的值，此类的定义在数据组工程中，通过设置此类的value来修改数据库相应项的值。
3.设置相应的属性。如数据组名、毫秒、时间等等。

*/

#include <QList>
#include <QString>
#include <QVariant>
#include <QDateTime>

class IMDDataItem;

class CDataArchivesNotifyParam
{
public:
    CDataArchivesNotifyParam();
    ~CDataArchivesNotifyParam();
     void operator =(const CDataArchivesNotifyParam &otherObj);

    /*!
    \section 设置/获取相应的属性
    */
public:       
    inline QString dataGroupName(){return m_dataGrpName;}   
    inline void setDataGroupName(const QString &name){m_dataGrpName = name;} //! 设置/获取数据组名


    inline void setRecordDateTime(QDateTime datetime){m_recDatetime = datetime;}
    inline QDateTime recordDateTime(){return m_recDatetime;} //! 设置/获取记录时间


    inline void setRecordMillisecond(int ms){m_iRecordMS = ms;}
    inline int recordMillisecond(){return m_iRecordMS;} //! 设置/获取记录毫秒


    inline void setSql(QString sql){m_sql = sql;}
    inline QString sql(){return m_sql;} //! 设置/获取sql

    /*!
    \section 增加、删除、获取IMDDataItem，用于操作通知需要修改的数据库字段
    */
public:
    bool addItem(IMDDataItem *item); //! 增加修改项
    bool removeItem(IMDDataItem *item); //! 删除修改项
    void removeAllItem(); //! 删除所有修改项
    IMDDataItem *getItem(const QString &name); //! 获取指定名字的修改项
    IMDDataItem *getItem(int index); //! 获取指定的位置的修改项
    int itemCount(){return m_itemList.size();} //! 获取所有修改项总数

private:
    int m_iRecordMS                  ;   //! 记录毫秒
    QString m_sql                    ;   //! 自定义sql
    QString m_dataGrpName            ;   //! 数据组名
    QDateTime m_recDatetime;                //! 记录时间
    QList<IMDDataItem*> m_itemList;         //! item链表
};

#endif // DATAARCHIVESNOTIFYPARAM_H
