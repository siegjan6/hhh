#ifndef DBQUERYCONFIGURE_2016_10_24_H
#define DBQUERYCONFIGURE_2016_10_24_H
#include <QString>
#include <QList>
#include <QStringList>
#include "../MDDBConnect/MDDBCommonDef.h"
//#include "D:\checkout\MindSCADA_CPP\include\MDDBQueryConfigure\MDDBQueryConfigure.h

#if defined(DBQUERYCONFIGURE_LIBRARY)
#  define DBQUERYCONFIGURE_EXPORT Q_DECL_EXPORT
#else
#  define DBQUERYCONFIGURE_EXPORT Q_DECL_IMPORT
#endif

//-------------------------------查询数据源配置文件---------------------------
namespace DBQ
{
//用在数据源结构体中 代表一个数据库
struct DB
{
    bool bDefault;         //是否是默认数据库
    QString dbName;    //数据库名称
};

//读写文件的类型 配置文件有两种格式.一种是XML,一种是JSON.
//目前只使用JSON格式.XML文件格式不完全支持.
enum FILE_TYPE
{
    FILE_TYPE_XML = 0,
    FILE_TYPE_JSON
};

struct DBSource
{
    QString dbSrcName;                //数据源名称
    QString dbServerName;          //数据库服务器名称
    bool bDefault;                         //是否是默认数据源
    QString host;                           //主机IP 目前不支持主机名
    int port;                                   //端口号
    QString userName;
    QString password;
    DBSOURCE_TYPE dbType;
    QList<struct DB> dbs;           //数据源中的数据库   模板类型不使用指针 因为DB数量少
    QString tablePrefix;                //表前缀,可以为空.SQL Server时为dbo.金仓时为public 2016.12.27 c2pj
};

//--------------------------------视图变量文件-----------------------------------
//视图变量类型
enum VIEW_VARIABLE_TYPE
{
    VIEW_VARIABLE_TYPE_UNKNOWN = 0,
    VIEW_VARIABLE_TYPE_CONST,                  //常量
    VIEW_VARIABLE_TYPE_ENUM,                   //枚举
    VIEW_VARIABLE_TYPE_FIELD,                    //字段
    VIEW_VARIABLE_TYPE_FUNC                     //函数
};

//视图字段类型
enum VIEW_FIELD_TYPE
{
    VIEW_FIELD_TYPE_UNKNOWN = 0,
    VIEW_FIELD_TYPE_BOOL,
    VIEW_FIELD_TYPE_SBYTE,
    VIEW_FIELD_TYPE_BYTE,
    VIEW_FIELD_TYPE_INT16,
    VIEW_FIELD_TYPE_UINT16,
    VIEW_FIELD_TYPE_INT32,
    VIEW_FIELD_TYPE_UINT32,
    VIEW_FIELD_TYPE_INT64,
    VIEW_FIELD_TYPE_UINT64,
    VIEW_FIELD_TYPE_FLOAT,
    VIEW_FIELD_TYPE_DOUBLE,
    VIEW_FIELD_TYPE_DATETIME,
    VIEW_FIELD_TYPE_STRING
};

//当视图变量类型为枚举值时 此结构体表示一个枚举值
struct ViewVariableEnumValue
{
    QString value;//枚举值
    QString desc; //枚举值描述
};

//视图变量
struct ViewVariable
{
    ViewVariable()
        :type(VIEW_VARIABLE_TYPE_UNKNOWN),
          valueType(VIEW_FIELD_TYPE_UNKNOWN)
    {}
    QString name;
    VIEW_VARIABLE_TYPE type;           //变量类型
    VIEW_FIELD_TYPE valueType;         //变量值类型
    QString value;
    QList<ViewVariableEnumValue> evs;   //枚举值
    QString description;                               //变量描述
};

//------------------------------视图配置文件--------------------------------
//视图中的数据源
struct ViewDBSource
{
    QString dbSrcName; //数据源名
    QStringList dbNames; //多个数据库名称
};

struct ViewField
{
    QString fieldName;                 //字段名
    QString tableName;
    VIEW_FIELD_TYPE type;   //字段类型
    QString alias;                          //字段别名 TODO：更新文档
    QString description;                //字段描述
};

//N叉树节点类型 过滤树为N叉树
enum VIEW_FILTER_NODE_TYPE
{
    VIEW_FILTER_NODE_TYPE_LEAF = 0,      //叶子节点
    VIEW_FILTER_NODE_TYPE_ROOT,            //根节点
    VIEW_FILTER_NODE_TYPE_AND,             //and节点
    VIEW_FILTER_NODE_TYPE_OR                 //or节点
};

//过滤树中的比较类型
enum VIEW_FILTER_COMPARE_TYPE
{
    VIEW_FILTER_COMPARE_TYPE_GREATER_THAN = 0,      //>
    VIEW_FILTER_COMPARE_TYPE_EQUAL,                             //=
    VIEW_FILTER_COMPARE_TYPE_LESS_THAN,                     //<
    VIEW_FILTER_COMPARE_TYPE_GREATER_EQUAL,          //>=
    VIEW_FILTER_COMPARE_TYPE_LESS_EQUAL,                   //<=
    VIEW_FILTER_COMPARE_TYPE_NOT_EQUAL,                    //!=
    VIEW_FILTER_COMPARE_TYPE_LIKE,                                  //like
    VIEW_FILTER_COMPARE_TYPE_NOT_LIKE                         //not like
};

//过滤树中过滤值的右值的类型
enum VIEW_FILTER_RIGHT_VALUE_TYPE
{
    VIEW_FILTER_RIGHT_VALUE_TYPE_CONST,   //常量类型
    VIEW_FILTER_RIGHT_VALUE_TYPE_VAR         //变量类型
};

//ViewFilterNode构成一颗多叉树 用来保存where语句.
//where语句为简单的where语句 语句中只有 and or 及比较符号.
//where语句例子:where A>b and (B<u or C>8)
struct ViewFilterNode
{
    DBQUERYCONFIGURE_EXPORT ViewFilterNode();
    //<值域> 节点为叶子节点时 此值域有效
    //左值格式为表名.字段   例子 table.var1>3
    QString leftValue;
    VIEW_FIELD_TYPE leftType;                                                //左值类型
    VIEW_FILTER_COMPARE_TYPE compare;                         //比较关系
    QString rightValue;                                                                     //右值类型与左值类型相同
    VIEW_FILTER_RIGHT_VALUE_TYPE rightValueType;       //常量 变量
     //</值域>
    VIEW_FILTER_NODE_TYPE type; //节点类型 and节点 or节点是非子节点
    QList<ViewFilterNode * > child; //子节点
};

//过滤树遍历访问者接口
class IFilterTreeTraverse
{
public:
    virtual void * VisitRootNode(ViewFilterNode * pFilter, void * parentNode) = 0;
    virtual void * VisitAndNode(ViewFilterNode * pFilter, void * parentNode) = 0;
    virtual void * VisitOrNode(ViewFilterNode * pFilter, void * parentNode) = 0;
    virtual void * VisitLeafNode(ViewFilterNode * pFilter, void * parentNode) = 0;
};

//视图
struct View
{
    View():pFilter(0){}
    DBQUERYCONFIGURE_EXPORT ~View();
    QString viewName;
    QList<ViewDBSource> dbSources; //数据源
    QList<ViewField *> fields;
    ViewFilterNode * pFilter;
};

//------------------------------------------运行时视图变量文件------------------------------------
struct RuntimeViewVariable
{
    QString key;
    QString value;
};
//------------------------------------------接口----------------------------------------------
//----------------------------------------数据源-------------------------------------------
DBQUERYCONFIGURE_EXPORT bool DataSource_Load(const QString & fileName, FILE_TYPE fileType, QList<DBSource *> & dbSourcesOut, QString & errOut);
DBQUERYCONFIGURE_EXPORT bool DataSource_Write(const QString & fileName, FILE_TYPE fileType, QList<DBSource *> & dbSrcsIn, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT DBSOURCE_TYPE DataSource_GetDBType(const QString & dbTypeStr);
//函数功能:获得某路径下的所有数据源名称
DBQUERYCONFIGURE_EXPORT bool DBSource_GetNames(const QString & viewPath, QStringList & dbsourceNamesOut);

//-------------------------------------------视图变量----------------------------------------
DBQUERYCONFIGURE_EXPORT bool ViewVariable_Load(const QString & fileName, FILE_TYPE fileType, QList<ViewVariable *> & viewVarsOut, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool ViewVariable_Write(const QString & fileName, FILE_TYPE fileType, QList<ViewVariable *> & viewVarsIn, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool ViewVariable_LoadFromDir(const QString & viewPath, QList<ViewVariable *> & viewVarsOut, QString & errStrOut);
//函数功能:从视图中获得此视图过滤条件中使用到的视图变量名.并从视图变量文件中加载对应的视图变量.
//之后,使用动态视图变量，更新视图变量的值.调用者负责释放viewVarsOut
DBQUERYCONFIGURE_EXPORT bool ViewVariable_GetVarFromViewWhenRuntime(const QString & viewPath, const QString & viewName, QList<DBQ::ViewVariable *> & viewVarsOut);
DBQUERYCONFIGURE_EXPORT DBQ::ViewVariable * ViewVariable_GetVarFromDir(const QString & viewPath, const QString & viewVarName);
//函数功能:获得视图变量对应的表名称.
//说明:视图变量是where语句中过滤项的右值.对应的左值为一个字段.视图变量的名称直接使用此字段的名称.字段名称的格式为"表名.字段名".
DBQUERYCONFIGURE_EXPORT QString ViewVariable_GetTableName(ViewVariable * pVar);
DBQUERYCONFIGURE_EXPORT QString ViewVariable_GetFieldName(ViewVariable * pVar);

//-------------------------------------------------视图-----------------------------------------
//TODO:去掉View_Load
DBQUERYCONFIGURE_EXPORT bool View_Load(QString fileName, FILE_TYPE fileType, View ** ppViewOut, QString & errOut);
DBQUERYCONFIGURE_EXPORT bool View_LoadFromDir(const QString & viewPath, const QString & viewName, View ** ppView, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool View_Write(QString fileName, FILE_TYPE fileType, View * pViewIn, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool View_WriteToDir(const QString & viewPath, const QString & viewName, View * pViewIn, QString & errStrOut);
//函数功能:删除View中配置数据库 如果删除完数据库之后 对应的数据源没有数据库了 则自动删除数据源
DBQUERYCONFIGURE_EXPORT bool View_DeleteDB(const QString & viewPath, const QString & viewName, const QString & dsName, const QString & dbName);
DBQUERYCONFIGURE_EXPORT bool View_AddDB(const QString & viewPath, const QString & viewName, const QString & dsName, const QString & dbName);
DBQUERYCONFIGURE_EXPORT bool View_Delete(const QString & viewPath, const QString & viewName);
DBQUERYCONFIGURE_EXPORT bool View_Rename(const QString & viewPath, const QString & viewNameOld, const QString & viewNameNew);
//函数功能:通过表名字段名查找对应的字段 tbFieldName的格式为"表.字段"
//返回值: 成功 返回对应的字段指针   失败则返回0
DBQUERYCONFIGURE_EXPORT ViewField * View_GetFieldByTableFieldName(View * pViewIn, const QString & tbFieldName);
//函数功能:返回字符串对应的字段类型
//返回值:成功 返回字符串对应的类型 不成功 返回unknown类型
DBQUERYCONFIGURE_EXPORT VIEW_FIELD_TYPE View_GetFieldTypeFromString(QString & typeStr);
//函数功能:返回字段类型对应的字符串
//返回值:成功 返回类型对应的字符串 不成功 返回unknown
DBQUERYCONFIGURE_EXPORT QString View_GetFieldTypeStringFromType(VIEW_FIELD_TYPE type);
//DBQUERYCONFIGURE_EXPORT View * View_Create(const QString & viewName);
DBQUERYCONFIGURE_EXPORT bool View_AddField(View * pView, ViewField * pField);
DBQUERYCONFIGURE_EXPORT bool View_ReplaceDBSources(View * pDestView, View * pSourceView);
//函数功能:由视图获得SQL模板.模板中包含视图变量.
DBQUERYCONFIGURE_EXPORT bool View_CreateSQLTemplate(View * pView, const QString & dbName, QString & sqlTemplateOut, QString & errStrOut);
//函数功能:从给定的目录下获得所有View的名称
DBQUERYCONFIGURE_EXPORT bool View_GetAllViewNameFromDir(const QString & dirStr, QStringList & viewNamesOut);
//函数功能:从给定的目录下指定的视图获取视图中的字段链表
DBQUERYCONFIGURE_EXPORT QList<DBQ::ViewField *> View_GetAllViewFieldsInViewFromDir(const QString & viewPath, const QString & viewName, QString & errStrOut);

//-----------------------------------------------运行时视图变量----------------------------------------
DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_Load(QString fileName, FILE_TYPE fileType, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_LoadFromDir(const QString & viewPath, const QString & viewName, QList<RuntimeViewVariable *> & rtVarsOut, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_Write(const QString & fileName, FILE_TYPE fileType, QList<RuntimeViewVariable *> & rtVarsIn, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_Update(const QString & fileName, FILE_TYPE fileType, QList<RuntimeViewVariable *> & rtVarsNew, QString & errStrOut);
DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_UpdateInDir(const QString & viewPath, const QString & viewName, QList<RuntimeViewVariable *> & rtVarsNew, QString & errStrOut);
//函数功能:删除viewPath目录下所有运行时视图变量文件
DBQUERYCONFIGURE_EXPORT bool RuntimeViewVariable_DeleteAllFile(const QString & viewPath);

//------------------------------------------------过滤树操作--------------------------------------------
DBQUERYCONFIGURE_EXPORT void FilterTree_Traverse(ViewFilterNode * pFilter, void * parentNode, IFilterTreeTraverse * traverse);
DBQUERYCONFIGURE_EXPORT bool FilterTree_Add(ViewFilterNode * parentNode, ViewFilterNode * newNode);
DBQUERYCONFIGURE_EXPORT bool FilterTree_Move(ViewFilterNode * destNode, ViewFilterNode * parentNode, ViewFilterNode * childNode);
//函数功能:从父节点中删除子节点 并释放子节点内存 父节点为空时 说明子节点为树的根节点
DBQUERYCONFIGURE_EXPORT bool FilterTree_Delete(ViewFilterNode * parentNode, ViewFilterNode * childNode);
DBQUERYCONFIGURE_EXPORT ViewFilterNode * FilterTree_Copy(ViewFilterNode * filterNode);
//函数功能:上移节点 同级子节点向上移动 不能从父节点中移出
DBQUERYCONFIGURE_EXPORT bool FilterTree_Up(ViewFilterNode * parentNode, ViewFilterNode * filterNode);
//函数功能:下移节点 同级子节点向下移动 不能从父节点中移出
DBQUERYCONFIGURE_EXPORT bool FilterTree_Down(ViewFilterNode * parentNode, ViewFilterNode * filterNode);
//函数功能:升级节点 即子节点从父节点中移出 添加到祖父节点中
//说明:升级之后的子节点在祖父节点中的位置:原父节点的下方
DBQUERYCONFIGURE_EXPORT bool FilterTree_Upgrade(ViewFilterNode * grandfatherNode, ViewFilterNode * parentNode, ViewFilterNode * filterNode);
//函数功能:降级节点 将子节点添加到父节点中的And/Or节点中
//说明:如果存在多个And/Or节点 移到距离最近And/Or节点中
//          如果不存在And/Or节点 则不能降级
DBQUERYCONFIGURE_EXPORT bool FilterTree_Degrade(ViewFilterNode * parentNode, ViewFilterNode * filterNode);
//函数功能:检查过滤节点是类型是否合法
DBQUERYCONFIGURE_EXPORT bool FilterTree_CheckType(DBQ::ViewFilterNode * node);
//函数功能:遍历Filter树 生成where语句
DBQUERYCONFIGURE_EXPORT bool FilterTree_CreateWhereClause(ViewFilterNode * pFilter,  QString & whereClauseOut,  QString & errStrOut);
DBQUERYCONFIGURE_EXPORT QString FilterTree_GetViewVarNameFromRightValue(ViewFilterNode * pFilterNode);
//函数功能:由比较类型获得其字符串表示
DBQUERYCONFIGURE_EXPORT const char * FilterTree_GetCompareString(VIEW_FILTER_COMPARE_TYPE type);

//------------------------------------------------其它--------------------------------------------
//函数功能:从字符中抽取被边界符包裹的token.左边界符为由两个字符组成.右边界符也是.
//参数说明:
//                  str 从此字符串中抽取token
//                  leftBound 左边界符 例如"[%"
//                  rightBound右边界符 例如"%]"
//                  tokens 保存抽取出的token
//说明:例如, "v>1 and b<[%t1%] and c=[%t2%]".执行结果为t1, t2.保存在tokens中.
DBQUERYCONFIGURE_EXPORT bool ExtractTokensWithBound2(QString & str, const char * leftBound, const char * rightBound, QStringList & tokens);
}
#endif // DBQUERYCONFIGURE_H
