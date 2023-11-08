#ifndef MDDATAGROUPMANAGER_H
#define MDDATAGROUPMANAGER_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据组管理器。
2.用于加载、存储数据组。
3.用于增加、删除历史组，获取历史、报警、日志组，查找数据组

*/

#include <QList>
#include "IMDDataGroupManager.h"

class CMDDataGroupManager : public IMDDataGroupManager
{
public:
    CMDDataGroupManager();
    ~CMDDataGroupManager();

/*!
\section 加载、存储数据源
*/
public:
    bool load(const QString &file);
    bool save(const QString &file);
    bool save_json(const QString &qfile);
    bool load_json(const QString &qfile);
    bool save();

/*!
\section 初始化系统固定的报警、日志组字段
*/
public:
    void initial();

/*!
\section 增加、删除历史组，获取历史、报警、日志组
*/
public:
    //! 增加历史组
    bool addHisDataGroup(IMDDataGroup *dataGroup);

    //! 移除指定历史组
    bool removeHisDataGroup(IMDDataGroup *dataGroup);

    //! 删除所有历史组
    void removeAllHisDataGroup();

    //! 查找数据组--通过数据组名
    virtual IMDDataGroup *getDataGroup(const QString &name);

    //! 查找历史组--通过历史组名
    IMDDataGroup *getHisDataGroup(const QString &name);

    //! 查找历史组--通过索引
    IMDDataGroup *getHisDataGroup(int index);    

    //! 历史组总数
    inline int hisDataGroupCount(){return m_hisDataGroupList.size();}

    //! 增加报警组
    inline IMDDataGroup *getAlarmDataGroup(){return m_alarmDataGroup;}

    //! 增加日志组
    inline IMDDataGroup *getLogDataGroup(){return m_logDataGroup;}

private:
    QList<IMDDataGroup*> m_hisDataGroupList;        //! 历史组链表
    IMDDataGroup *m_alarmDataGroup         ;  //! 报警组
    IMDDataGroup *m_logDataGroup           ;  //! 日志组
    QString m_fileName                     ;  //! 数据组序列化文件位置
};

#endif // MDDATAGROUPMANAGER_H
