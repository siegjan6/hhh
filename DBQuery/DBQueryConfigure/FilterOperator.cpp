#include "MDDBQueryConfigure.h"
#include <QStringBuilder>
#include "utils.h"

namespace DBQ
{
DBQUERYCONFIGURE_EXPORT const char * FilterTree_GetCompareString(VIEW_FILTER_COMPARE_TYPE type)
{
    switch(type)
    {
    case VIEW_FILTER_COMPARE_TYPE_GREATER_THAN: return ">";
    case VIEW_FILTER_COMPARE_TYPE_EQUAL: return "=";
    case VIEW_FILTER_COMPARE_TYPE_LESS_THAN: return "<";
    case VIEW_FILTER_COMPARE_TYPE_GREATER_EQUAL: return ">=";
    case VIEW_FILTER_COMPARE_TYPE_LESS_EQUAL: return "<=";
    case VIEW_FILTER_COMPARE_TYPE_NOT_EQUAL:return "!=";
    case VIEW_FILTER_COMPARE_TYPE_LIKE: return "like";
    case VIEW_FILTER_COMPARE_TYPE_NOT_LIKE:return "not like";
    default:
        return "unknown";
    }

    return "unknown";
}

ViewFilterNode::ViewFilterNode()
    : leftType(VIEW_FIELD_TYPE_UNKNOWN),
      compare(VIEW_FILTER_COMPARE_TYPE_GREATER_THAN),
      rightValueType(VIEW_FILTER_RIGHT_VALUE_TYPE_CONST),
      type(VIEW_FILTER_NODE_TYPE_LEAF)
{}

//遍历Filter树，生成where语句
class FilterTreeVistor
{
public:
    //函数功能:遍历Filter树，生成where语句
    bool Visit(ViewFilterNode * pFilter,  QString & whereClauseOut,  QString & errStrOut);
private:
    bool VisitFilterTree(ViewFilterNode * pFilter, QString & clauseStrOut);
    bool VisitAndOrNode(ViewFilterNode * pFilter, QString & clauseStrOut);
};

bool FilterTreeVistor::Visit(ViewFilterNode * pFilter, QString & whereClauseOut, QString & errStrOut)
{
    if(!pFilter) //可以没有过滤条件
        return false;

    if(pFilter->type != VIEW_FILTER_NODE_TYPE_ROOT) //第一个节点必须是根节点
    {
        errStrOut = "hasn't root node";
        WriteInfoLog("FilterTreeVistor:Visit:"+errStrOut);
        return false;
    }

    //根节点默认为and
    whereClauseOut = "where ";
    QString clauseStr;
    for(int i=0; i<pFilter->child.size(); i++)
    {
        if(!VisitFilterTree(pFilter->child[i], clauseStr))
        {
            whereClauseOut.clear();
            return false;
         }
        //whereClauseOut.append(clauseStr).append(" and ");
        whereClauseOut = whereClauseOut%clauseStr%" and ";//使用QStringBuild代替append
    }

    if(whereClauseOut == "where ") //即没有过滤条件
    {
        whereClauseOut.clear();
        return true;
    }

    //去掉多添加的" and "
    whereClauseOut = whereClauseOut.left(whereClauseOut.size() - 5);
    return true;
}

bool FilterTreeVistor::VisitFilterTree(ViewFilterNode * pFilter, QString & clauseStrOut)
{
    if(!pFilter)
        return false;

    //递归终止条件:遍历完某子树的所有节点
    if(VIEW_FILTER_NODE_TYPE_AND == pFilter->type   //and节点 有子节点
        ||VIEW_FILTER_NODE_TYPE_OR == pFilter->type)   //or节点 有子节点
        return VisitAndOrNode(pFilter, clauseStrOut);


    if(VIEW_FILTER_NODE_TYPE_LEAF == pFilter->type) //叶子节点 有子节点
    {
        QString rightValue = pFilter->rightValue;
        if(VIEW_FILTER_RIGHT_VALUE_TYPE_VAR == pFilter->rightValueType) //如果是变量类型要加上[%变量%]
            rightValue = "[%"%rightValue%"%]";

        //如果操作符为like或not like 则左值和操作符之间要加空格分隔开
        //且为右值加上两个单引号
        if(VIEW_FILTER_COMPARE_TYPE_LIKE == pFilter->compare
            ||VIEW_FILTER_COMPARE_TYPE_NOT_LIKE == pFilter->compare)
        {
            clauseStrOut = clauseStrOut%pFilter->leftValue%" "%FilterTree_GetCompareString(pFilter->compare)%" '"%rightValue%"'";
            return true;
        }

        //如果为变量类型又为时间或字符串类型，则右值的格式为'[%变量名%]'。
        //所以在替换变量时，如果变量的类型为时间或字符串类型时，则不用再加单引号
        //字符串连接使用QStringBuilder 减少内存分配次数提高效率
        if(VIEW_FIELD_TYPE_DATETIME == pFilter->leftType
            ||VIEW_FIELD_TYPE_STRING == pFilter->leftType)
        {
            clauseStrOut = clauseStrOut%pFilter->leftValue%FilterTree_GetCompareString(pFilter->compare)%"'"%rightValue%"'";
        }
        else
            clauseStrOut = clauseStrOut%pFilter->leftValue%FilterTree_GetCompareString(pFilter->compare)%rightValue;

        return true;
    }

    WriteInfoLog("FilterTreeVistor::VisitFilterTree:type error");
    return false;
}

bool FilterTreeVistor::VisitAndOrNode(ViewFilterNode * pFilter, QString & clauseStrOut)
{
    int size = pFilter->child.size();
    if(0 == size) //允许and节点为空
        return true;

    const char * andOrStr = 0;
    if(VIEW_FILTER_NODE_TYPE_AND == pFilter->type)
        andOrStr = " and ";
    else if(VIEW_FILTER_NODE_TYPE_OR == pFilter->type)
        andOrStr = " or ";
    else
    {
        WriteInfoLog("FilterTreeVistor::VisitAndOrNode:type error");
        return false; //不可能出现的情况
    }

    QString itemStr;
    for(int i=0; i<size; i++)
    {
        itemStr.clear();
        if(!VisitFilterTree(pFilter->child[i], itemStr))
            return false;

        clauseStrOut.append(itemStr).append(andOrStr);
    }

    clauseStrOut = clauseStrOut.left(clauseStrOut.size() - strlen(andOrStr)); //去掉最后加入的" and"或者" or"

    if(1 == size) //只有一个子节点 不用加括号
        return true;

    //size > 1 前后要加上括号
    clauseStrOut = "("+clauseStrOut +")";
    return true;
}

//函数功能:遍历Filter树 生成where语句
DBQUERYCONFIGURE_EXPORT bool FilterTree_CreateWhereClause(ViewFilterNode * pFilter,  QString & whereClauseOut,  QString & errStrOut)
{
    if(!pFilter)
        return true; //不存在while语句

    FilterTreeVistor vistor;
    return vistor.Visit(pFilter, whereClauseOut, errStrOut);
}

DBQUERYCONFIGURE_EXPORT void FilterTree_Traverse(DBQ::ViewFilterNode * pFilter, void * parentNode, IFilterTreeTraverse * traverse)
{
    if(0 == pFilter || 0 == traverse)
        return;

    if(DBQ::VIEW_FILTER_NODE_TYPE_ROOT == pFilter->type)
    {
        void * pSelfNode = traverse->VisitRootNode(pFilter, parentNode);
        for(int i=0; i<pFilter->child.size(); i++)
            FilterTree_Traverse(pFilter->child[i], pSelfNode, traverse);
        return ;
    }
    //! 递归终止条件:遍历完某子树的所有节点
    if(DBQ::VIEW_FILTER_NODE_TYPE_AND == pFilter->type) //! and节点 有子节点
    {
         void * pSelfNode =  traverse->VisitAndNode(pFilter, parentNode);

        for(int i=0; i<pFilter->child.size(); i++)
            FilterTree_Traverse(pFilter->child[i], pSelfNode, traverse);
        return;
    }

    if(DBQ::VIEW_FILTER_NODE_TYPE_OR == pFilter->type) //! or节点 有子节点
    {
        void * pSelfNode =  traverse->VisitOrNode(pFilter, parentNode);

        for(int i=0; i<pFilter->child.size(); i++)
            FilterTree_Traverse(pFilter->child[i], pSelfNode, traverse);
        return;
    }

    if(DBQ::VIEW_FILTER_NODE_TYPE_LEAF == pFilter->type) //!叶子节点 有子节点
    {
        traverse->VisitLeafNode(pFilter, parentNode);
        return ;
    }

    WriteInfoLog("FilterTree_Traverse:type error");
}

DBQUERYCONFIGURE_EXPORT bool FilterTree_Add(DBQ::ViewFilterNode * parentNode, DBQ::ViewFilterNode * newNode)
{
    if(0 == parentNode || 0 == newNode)
        return false;


    bool b = (VIEW_FILTER_NODE_TYPE_AND == newNode->type
                    || VIEW_FILTER_NODE_TYPE_OR == newNode->type
                    || VIEW_FILTER_NODE_TYPE_LEAF == newNode->type);
    if(!b)
        return false;

    b = (VIEW_FILTER_NODE_TYPE_ROOT  == parentNode->type
            ||VIEW_FILTER_NODE_TYPE_AND == parentNode->type
            ||VIEW_FILTER_NODE_TYPE_OR == parentNode->type);
    if(!b)
        return false;

    parentNode->child.append(newNode);
    return true;
}

DBQUERYCONFIGURE_EXPORT bool FilterTree_Move(DBQ::ViewFilterNode * destNode, DBQ::ViewFilterNode * parentNode, DBQ::ViewFilterNode * childNode)
{
    if(0 == destNode|| 0 == parentNode || 0 == childNode)
        return false;

    if(!parentNode->child.removeOne(childNode))
        return false;

    if(destNode->child.contains(childNode))
        return false;

    destNode->child.append(childNode);

    return true;
}

//函数功能:上移节点 同级子节点向上移动 不能从父节点中移出
DBQUERYCONFIGURE_EXPORT bool FilterTree_Up(ViewFilterNode * parentNode, ViewFilterNode * filterNode)
{
    if(0==parentNode ||filterNode==0)
        return false;

    int index = parentNode->child.indexOf(filterNode);
    if(-1 == index)
        return false;

    if(0 == index)
        return true; //子节点已经在最顶端了 无须上移

    //index>0;
    parentNode->child.swap(index-1, index);
    return true;
}

//函数功能:下移节点 同级子节点向下移动 不能从父节点中移出
DBQUERYCONFIGURE_EXPORT bool FilterTree_Down(ViewFilterNode * parentNode, ViewFilterNode * filterNode)
{
    if(0==parentNode ||filterNode==0)
        return false;

    int index = parentNode->child.indexOf(filterNode);
    if(-1 == index)
        return false;

    if(index == (parentNode->child.size()-1))
        return true; //子节点已经在最底部了 无须下移

    //index没有指向最后一个元素
    parentNode->child.swap(index, index+1);
    return true;
}

//函数功能:升级节点 即子节点从父节点中移出 添加到祖父节点中
//说明:升级之后的子节点在祖父节点中的位置:原父节点的下方
DBQUERYCONFIGURE_EXPORT bool FilterTree_Upgrade(ViewFilterNode * grandfatherNode, ViewFilterNode * parentNode, ViewFilterNode * childNode)
{
    if(0==grandfatherNode || 0==parentNode || childNode==0)
        return false;

    int index = grandfatherNode->child.indexOf(parentNode);
    if(-1 == index)
        return false;

    if(!parentNode->child.removeOne(childNode))
        return false;

    grandfatherNode->child.insert(++index, childNode);
    return true;
}

//函数功能:降级节点 将子节点添加到父节点中的And/Or节点中
//说明:如果存在多个And/Or节点 移到距离最近And/Or节点中
//          如果不存在And/Or节点 则不能降级
DBQUERYCONFIGURE_EXPORT bool FilterTree_Degrade(ViewFilterNode * parentNode, ViewFilterNode * childNode)
{
    if(0==parentNode || childNode==0)
        return false;

    int childIndex = parentNode->child.indexOf(childNode);
    if(-1 == childIndex)
        return false;

    //计算距离parentNode的所有And/Or子节点中最近的子节点
    int distance = -1;//不可能有么这多节点
    ViewFilterNode * destNode = 0;
    for(int i=0; i<parentNode->child.size(); i++)
    {
        if(VIEW_FILTER_NODE_TYPE_AND == parentNode->child[i]->type
            ||VIEW_FILTER_NODE_TYPE_OR == parentNode->child[i]->type)
        {
            if(-1 == distance)
            {
                distance = abs(childIndex - i);
                destNode = parentNode->child[i];
                continue;
            }

            if(distance>abs(childIndex - i))
            {
                distance = abs(childIndex - i);
                destNode = parentNode->child[i];
            }
        }
    }//end for

    if(!destNode)
        return false; //没有And/Or节点

    return FilterTree_Move(destNode, parentNode, childNode);
}

class DeletionTraverse : public IFilterTreeTraverse
{
public:
    ~DeletionTraverse();
    void * VisitRootNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
    {Q_UNUSED(parentNode);m_Nodes.append(pFilter); return 0;}
    virtual void * VisitAndNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
    {Q_UNUSED(parentNode);m_Nodes.append(pFilter); return 0;}
    virtual void * VisitOrNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
    {Q_UNUSED(parentNode);m_Nodes.append(pFilter); return 0;}
    virtual void * VisitLeafNode(DBQ::ViewFilterNode * pFilter, void * parentNode)
    {Q_UNUSED(parentNode);m_Nodes.append(pFilter); return 0;}
private:
    QList<DBQ::ViewFilterNode *> m_Nodes;
};

DeletionTraverse::~DeletionTraverse()
{
    while(!m_Nodes.isEmpty())
    {
        delete m_Nodes.takeFirst();
    }
}

DBQUERYCONFIGURE_EXPORT bool FilterTree_Delete(DBQ::ViewFilterNode * parentNode, DBQ::ViewFilterNode * childNode)
{
    if(!childNode)
        return false;

    if(parentNode)
    {
        if(!parentNode->child.removeOne(childNode))
            return false;
    }

    DeletionTraverse traverse;
    FilterTree_Traverse(childNode, 0, &traverse);
    return true;
}

class CopyTraverse : public IFilterTreeTraverse
{
public:
    CopyTraverse():cpNode(0)
    {}
    void * VisitRootNode(ViewFilterNode * pFilter, void * parentNode);
    void * VisitAndNode(ViewFilterNode * pFilter, void * parentNode);
    void * VisitOrNode(ViewFilterNode * pFilter, void * parentNode);
    void * VisitLeafNode(ViewFilterNode * pFilter, void * parentNode);
    ViewFilterNode * cpNode;
};

void * CopyTraverse::VisitRootNode(ViewFilterNode * pFilter, void * parentNode)
{
    Q_UNUSED(parentNode);

    ViewFilterNode * pRootFilter = new ViewFilterNode;
    pRootFilter->type = pFilter->type;
    if(!cpNode)
        cpNode = pRootFilter; //遍历的第一个节点作为复制的树的根节点

    return pRootFilter;
}

void * CopyTraverse::VisitAndNode(ViewFilterNode * pAndNodeFilter, void * parentNode)
{
    ViewFilterNode * andNodeFilterCp = new ViewFilterNode;
    andNodeFilterCp->type = pAndNodeFilter->type;

    if(parentNode)
    {
        ViewFilterNode * parentFilter = (ViewFilterNode *)parentNode;
        parentFilter->child.append(andNodeFilterCp);
    }

    if(!cpNode)
        cpNode = andNodeFilterCp;

    return andNodeFilterCp;
}

void * CopyTraverse::VisitOrNode(ViewFilterNode * pOrNodeFilter, void * parentNode)
{
    ViewFilterNode * orNodeFilterCp = new ViewFilterNode;
    orNodeFilterCp->type = pOrNodeFilter->type;

    if(parentNode)
    {
        ViewFilterNode * parentFilter = (ViewFilterNode *)parentNode;
        parentFilter->child.append(orNodeFilterCp);
    }

    if(!cpNode)
        cpNode = orNodeFilterCp;

    return orNodeFilterCp;
}

void * CopyTraverse::VisitLeafNode(ViewFilterNode * pLeftFilter, void * parentNode)
{
    ViewFilterNode * leafNodeCp = new ViewFilterNode;
    leafNodeCp->compare = pLeftFilter->compare;
    leafNodeCp->leftType = pLeftFilter->leftType;
    leafNodeCp->leftValue = pLeftFilter->leftValue;
    leafNodeCp->rightValue = pLeftFilter->rightValue;
    leafNodeCp->rightValueType = pLeftFilter->rightValueType;
    leafNodeCp->type = pLeftFilter->type;

    if(parentNode)
    {
        ViewFilterNode * parentFilter = (ViewFilterNode *)parentNode;
        parentFilter->child.append(leafNodeCp);
    }

    if(!cpNode)
        cpNode = leafNodeCp;

    return leafNodeCp;
}

DBQUERYCONFIGURE_EXPORT ViewFilterNode * FilterTree_Copy(ViewFilterNode * filterNode)
{
    if(!filterNode)
        return 0;

    CopyTraverse traverse;
    FilterTree_Traverse(filterNode, 0, &traverse);
    return traverse.cpNode;
}

DBQUERYCONFIGURE_EXPORT bool FilterTree_CheckType(DBQ::ViewFilterNode * node)
{
    if(!node)
        return false;

    if(VIEW_FILTER_NODE_TYPE_ROOT  == node->type
        ||VIEW_FILTER_NODE_TYPE_AND == node->type
        ||VIEW_FILTER_NODE_TYPE_OR == node->type
        ||VIEW_FILTER_NODE_TYPE_LEAF == node->type)
    {
        return true;
    }

    return false;
}

DBQUERYCONFIGURE_EXPORT QString FilterTree_GetViewVarNameFromRightValue(ViewFilterNode * pFilterNode)
{
    if(!pFilterNode)
        return "";

    QStringList tokens;
    ExtractTokensWithBound2(pFilterNode->rightValue, "[%", "%]", tokens);

    if(tokens.isEmpty())
        return "";

    return tokens[0];
}
}//end namespace


