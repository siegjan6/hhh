#ifndef IMDDATAGROUPEMANAGER_H
#define IMDDATAGROUPEMANAGER_H

/*!

\author luochao

\section 功能
1.数据组管理器接口类--抽像类。
2.用于加载、存储数据组。
3.用于增加、删除历史组，获取历史、报警、日志组，查找数据组。

*/

#include <QString>

class IMDDataGroup;
class IMDDataGroupManager
{
public:
    virtual ~IMDDataGroupManager(){}

/*!
\section 加载、存储数据源
*/
public:
    virtual bool load(const QString &file) = 0;
    virtual bool load_json(const QString &file) = 0;
    virtual bool save(const QString &file) = 0;
    virtual bool save_json(const QString &file) = 0;
    virtual bool save() = 0;

/*!
\section 增加、删除历史组，获取历史、报警、日志组，查找数据组
*/
public:
    //! 增加历史组
    virtual bool addHisDataGroup(IMDDataGroup *dataGroup) = 0;

    //! 移除指定历史组
    virtual bool removeHisDataGroup(IMDDataGroup *dataGroup) = 0;

    //! 删除所有历史组
    virtual void removeAllHisDataGroup() = 0;

    //! 查找数据组--通过数据组名
    virtual IMDDataGroup *getDataGroup(const QString &name) = 0;

    //! 查找历史组--通过历史组名
    virtual IMDDataGroup *getHisDataGroup(const QString &name) = 0;

    //! 查找历史组--通过索引
    virtual IMDDataGroup *getHisDataGroup(int index) = 0;

    //! 历史组总数
    virtual int hisDataGroupCount() = 0;

    //! 增加报警组
    virtual IMDDataGroup *getAlarmDataGroup() = 0;

    //! 增加日志组
    virtual IMDDataGroup *getLogDataGroup() = 0;
};

#endif // IMDDATAGROUPEMANAGER_H
