#include "ViewStorage.h"
#include "MDDBQueryConfigure.h"

namespace DBQ
{
void ExtractTokensWithBound1(QString & str, char leftBound, char rightBound, QStringList & tokens);

ViewStorage::ViewStorage()
{
}

VIEW_FIELD_TYPE ViewStorage::GetFieldTypeFromString(QString & typeStr)
{
    return View_GetFieldTypeFromString(typeStr);
}

bool ViewStorage::ParseItem(QDomElement & itemNode, ViewFilterNode * pChildNode)
{
    QString strV = itemNode.text();
    QStringList tokens;
    ExtractTokensWithBound1(strV, '{', '}', tokens);

    //检查item的格式是否合法
    //合法的格式:[左值][比较符][右值][左值类型] //添加[左值类型]
    if(tokens.size() != 4)
        return false;
    QString compareToeken = tokens[1]; //[比较符号]

    if(compareToeken == ">")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_GREATER_THAN;
    else if(compareToeken == "=")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_EQUAL;
    else if(compareToeken == "<")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_LESS_THAN;
    else if(compareToeken == ">=")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_GREATER_EQUAL;
    else if(compareToeken == "<=")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_LESS_EQUAL;
    else if(compareToeken == "!=")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_NOT_EQUAL;
    else if(compareToeken == "like")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_LIKE;
    else if(compareToeken == "not like")
        pChildNode->compare = VIEW_FILTER_COMPARE_TYPE_NOT_LIKE;
    else
        return false;

    pChildNode->leftValue = tokens[0];
    pChildNode->leftType = GetFieldTypeFromString(tokens[3]);

    QStringList vvTokens; //视图变量
    //判断右值的类型
    ExtractTokensWithBound2(tokens[2], "[%", "%]", vvTokens);
    if(vvTokens.empty())
    {
        pChildNode->rightValueType = VIEW_FILTER_RIGHT_VALUE_TYPE_CONST;
        pChildNode->rightValue = tokens[2];
    }
    else
    {
        pChildNode->rightValueType = VIEW_FILTER_RIGHT_VALUE_TYPE_VAR;
        pChildNode->rightValue = vvTokens[0];
    }
    return true;
}

bool ViewStorage::ParserFilterTree(QDomElement & itemNode, ViewFilterNode * pFilter)
{
    //递归终止条件:遍历完某子树的所有节点
    ViewFilterNode * pChildNode = 0;
    while(!itemNode.isNull())
    {
        if(itemNode.tagName().compare("and", Qt::CaseInsensitive) == 0) //and节点
        {
            pChildNode = new ViewFilterNode;

            pChildNode->type = VIEW_FILTER_NODE_TYPE_AND;
            pFilter->child.push_back(pChildNode);
            QDomElement ele = itemNode.firstChildElement();
            if(!ParserFilterTree(ele, pChildNode))
                return false;

            itemNode = itemNode.nextSiblingElement();
            continue;
        }

        if(itemNode.tagName().compare("or", Qt::CaseInsensitive) == 0) //or节点
        {
            pChildNode = new ViewFilterNode;

            pChildNode->type = VIEW_FILTER_NODE_TYPE_OR;
            pFilter->child.push_back(pChildNode);
            QDomElement ele = itemNode.firstChildElement();
            if(!ParserFilterTree(ele, pChildNode))
                return false;

            itemNode = itemNode.nextSiblingElement();
            continue;
        }

        if(itemNode.tagName().compare("item", Qt::CaseInsensitive) == 0)
        {
            pChildNode = new ViewFilterNode;

            pChildNode->type = VIEW_FILTER_NODE_TYPE_LEAF;
            pFilter->child.push_back(pChildNode);
            if(!ParseItem(itemNode,pChildNode))
                return false;

            itemNode = itemNode.nextSiblingElement();
            continue;
        }

        return false;
    }
      return true; //遍历完某子树的所有节点
}

QString ViewStorage::GetFiledTypeStr(VIEW_FIELD_TYPE type)
{
    return View_GetFieldTypeStringFromType(type);
}
}
