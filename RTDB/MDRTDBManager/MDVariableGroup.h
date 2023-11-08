/*!
author enli 2015.01.08
\section 变了组管理类
*/
#ifndef CMDVARIABLEGROUP_H
#define CMDVARIABLEGROUP_H

#include <QMap>
#include <QList>
#include <QString>
#include <QMutex>
#include <QObject>
#include <QMutexLocker>
#include "IMDVariableGroup.h"
#include "IMDRTDBVariable.h"
#include "IMDDriverManager.h"
#include "MDVariableManager.h"
#include "MDRTDBManager.h"

#include <QDomElement>
#include <QJsonObject>
#include "SortJsonObject.h"
static quint32 m_uaNodeId = 0;
//变量组
class CMDVariableGroup:public IMDVariableGroup
{
//   Q_OBJECT
//   Q_PROPERTY(QString GUID_General)
//   Q_PROPERTY(QString Name READ localName WRITE setLocalName)
//   Q_PROPERTY(QString FullName READ name WRITE setName)
//   Q_PROPERTY(QString Discription READ description WRITE setDescription)

public:
    CMDVariableGroup();
    virtual ~CMDVariableGroup();
/*!
\section IMDRTDBTreeItem 接口
*/

public:
    //! TreeItem Type
    MDTreeItemType itemType();
    void setItemType(MDTreeItemType itemType);
    void *parentItem(MDTreeItemType parentItemType);
    void setParentItem(void* parent,MDTreeItemType parentItemType);
private:
    MDTreeItemType m_itemType;
/*!
\section 基本属性
*/
public:
    //!组名
    QString name();
    void setName(QString name);
    //!不带父Group的名
    QString localName();
    void setLocalName(QString name);
    //! 描述
    QString description();
    void setDescription(QString description);
    //!组id
    void setId(uint id);
    uint id();

    //!复制
    void copy(IMDVariableGroup *otherVarGroup,const QString &localName);

    //!复制粘贴
    IMDVariableGroup * copyPasteVariableGroup(IMDVariableGroup* varGroup);
    bool copyPasteVariable(IMDRTDBVariable* var);

	QUaFolderObject *uaFolder();
	void setUAFolder(QUaFolderObject * folder);
private:
    QString m_name; 
    QString m_localName;
    QString m_description;
    QString m_orderby;
    uint m_id;
    IMDVariableGroup *m_parentVariableGroup;

	QUaFolderObject *m_uaFolder;

    bool binitSystemVar;
/*!
\section 子变量组
*/
public: 
    //!添加子变量组
    bool addChildVariableGroup(IMDVariableGroup* variableGroup,QString &error);
    bool addChildVariableGroup(QString name,QString description,QString &error);
    //!移除子变量组
    bool removeChildVariableGroup(IMDVariableGroup* variableGroup);
    //!移除子变量组
    bool removeChildVariableGroup(QString groupName);
    //!删除所有子变量组
    void removeAllChildVariableGroup();
    //!获取子变量组
    IMDVariableGroup *getChildVariableGroupAt(int index);
    //!用全名获取子变量组
    IMDVariableGroup *getChildVariableGroupWithName(QString fullGroupName);
    //!获取子变量组
    IMDVariableGroup *getChildVariableGroup(QString groupName);
    //!获取子变量组数量
    int getChildVariableGroupCount();
    //!根据id号深度查找第一个匹配的子组
    IMDVariableGroup *getGroupById(uint id);

private: 
    QList<IMDVariableGroup*> m_childVariableGroupList;//子变量组链表
    QMutex m_childVariableGroupListMutex;
/*!
\section 关联变量
*/
public:    
    //!添加变量
    bool addVariable(IMDRTDBVariable* nsVar,QString &error);
    //!移除变量
    bool removeVariable(IMDRTDBVariable* nsVar);
    //!删除所有变量
    void removeAllVariable();
    //!获取变量
    IMDRTDBVariable* getVariableAt(int index);
    //!获取变量
    IMDRTDBVariable *getVariable(QString varShortName);
    //!获取变量数量
    int getVariableCount(); 
private:
    CMDVariableManager m_mdVariableManager;
/*!
\section 实时数据库管理器
*/
public:    void setRTDBManager(CMDRTDBManager *nsRTDBManager);
    CMDRTDBManager * getRTDBManager();
private:
    CMDRTDBManager *m_mdRTDBManager;
/*!
\section 读写配置
*/
public:
    void saveConfiguration(QDomDocument &doc,QDomElement &parentElement);
    bool readConfiguration(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDomElement &domElement);
    void  save(QDataStream &out);
    bool  read(CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup,QDataStream &in);
    void serialize(CSortJsonObject &json);
    bool deserialize(const QJsonObject &json,CMDVariableManager *nsRTDBVariableManager,IMDDriverManager *nsDriverManager,IMDVariableGroup *parentVariableGroup);
    /*!
\section 系统变量组操作
*/
public:
    bool isSystemVariableGroup();
    //void addSystemVariable(QString &name,MDDataType dtType,QString &description );
    void addSystemVariable(const QString & name,MDDataType dtType,const QString & description );
    void  initialSystemVariableGroup();
    void updateSystemVariable();
private:
    int m_systemVariableCheckCnt;
};

#endif // CMDVARIABLEGROUP_H
