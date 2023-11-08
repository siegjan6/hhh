/*!
\author zyb 2015.01.08
\section 变量组管理类
*/
#ifndef IMDVARIABLEGROUP_H
#define IMDVARIABLEGROUP_H

#include <QString>

#include "IMDTreeItem.h"
#include "IMDRTDBVariable.h"

class QUaFolderObject;
class IMDVariableGroup:public IMDTreeItem,public QObject
{
public:
    virtual ~IMDVariableGroup(){}
public:
    //!组名
    virtual QString name() = 0;
    virtual void setName(QString name) = 0;
    //!不带父Group的名
    virtual  QString localName()=0;
    virtual  void setLocalName(QString name)=0;
    //!描述
    virtual QString description() = 0;
    virtual void setDescription(QString description) = 0;
    //!组id
    virtual void setId(uint id)=0;
    virtual uint id()=0;

    /*!
    section 子变量组
    */
public:
    //!添加子变量组
    virtual bool addChildVariableGroup(IMDVariableGroup* variableGroup,QString &error)= 0;
    virtual bool addChildVariableGroup(QString name,QString description,QString &error)= 0;
    //!移除子变量组
    virtual bool removeChildVariableGroup(IMDVariableGroup* variableGroup)= 0;
    //!移除子变量组
    virtual bool removeChildVariableGroup(QString groupName)= 0;
    //!删除所有子变量组
    virtual void removeAllChildVariableGroup()= 0;
    //!获取子变量组
    virtual IMDVariableGroup *getChildVariableGroupAt(int index)= 0;
    //!获取子变量组
    virtual IMDVariableGroup *getChildVariableGroup(QString groupName)= 0;
    //!用全名获取子变量组
    virtual IMDVariableGroup *getChildVariableGroupWithName(QString fullGroupName)= 0;
    //!获取子变量组数量
    virtual int getChildVariableGroupCount()= 0;
    //!根据id号深度查找第一个匹配的子组
    virtual IMDVariableGroup *getGroupById(uint id) = 0;
/*!
section 关联变量
*/
public:
    //!添加变量
    virtual bool addVariable(IMDRTDBVariable* nsVar,QString &error)= 0;
    //!移除变量
    virtual bool removeVariable(IMDRTDBVariable* nsVar)= 0;
    //!删除所有变量
    virtual void removeAllVariable()= 0;
    //!获取变量
    virtual IMDRTDBVariable* getVariableAt(int index)= 0;
    //!获取变量
    virtual IMDRTDBVariable *getVariable(QString varShortName)= 0;
    //!获取变量数量
    virtual int getVariableCount()= 0;
    //!复制
    virtual void copy(IMDVariableGroup *otherVarGroup,const QString &localName)=0;
    //!复制粘贴
    virtual IMDVariableGroup * copyPasteVariableGroup(IMDVariableGroup* varGroup)=0;
    virtual bool copyPasteVariable(IMDRTDBVariable* var)=0;
/*!
section OPC UA
*/
public:
	virtual QUaFolderObject *uaFolder()=0;
};

#endif // IMDVARIABLEGROUP_H
