/*!
\author zyb 2015.01.08
\section 提供备或变量列表控件中树节点Data属性
*/
#ifndef IMDRTDBTREEITEM_H
#define IMDRTDBTREEITEM_H

//实时数据库TreeItem Type
enum class MDTreeItemType
{
    VariableGroup = 0,
    Variable = 1,
    Device = 2,
    DataBlock = 3,
    DriverManager = 4,
};
class IMDTreeItem
{
public:
    virtual ~IMDTreeItem(){}
public:
    //TreeItem Type
    virtual  MDTreeItemType itemType()=0;
    virtual  void setItemType(MDTreeItemType itemType)=0;
    virtual  void *parentItem(MDTreeItemType parentItemType)=0;
    virtual  void setParentItem(void *parent,MDTreeItemType parentItemType)=0;
};

#endif // IMDVARIABLE_H
