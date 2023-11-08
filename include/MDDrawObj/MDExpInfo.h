/*!
\author aGRw 2016.05.16
\section 表达式属性信息
*/
#ifndef MDEXPINFOINFO_H
#define MDEXPINFOINFO_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWOBJ_LIBRARY)
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWOBJSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QString>
#include <QVariant>
#include <QMap>
#include <QVector>
#include <QJsonObject>

#include "IVarObject.h"
#include "../MDExpression/MDExpression.h"

class CMDParameter;
class CMDExpManager;

/*!
\section 表达式组头信息
*/
class MDDRAWOBJSHARED_EXPORT CMDExpGroup
{
public:
    CMDExpGroup(const QString& name="", const QString& showText="",
        const QString& comment="");
public:
    QString name(){return m_name;}
    QString showText(){return m_showText;}
    QString comment(){return m_comment;}
private:
    QString m_name;         //名称
    QString m_showText;     //显示文本
    QString m_comment;      //注释
};

/*!
\section 配置辅助类
*/
class MDDRAWOBJSHARED_EXPORT CMDExpConfig
{
/*!
\section property
*/
public:
    //返回索引，每次+1
    static int getNextIndex(int type);
    //设置当前组
    static CMDExpGroup* setCurrentGroup(const QString& name="", const QString& showText="",
        const QString& comment="");
    static QMap<QString, CMDParameter*>* paraDict(){return &m_paraDict;}
    static CMDExpManager* expManager(int type);
    //初始化基类表达式对象列表
    static void initExpManager(int type, QList<QStringList> names);
    //初始化实际类表达式对象列表
    static void initFactExpManager(int type, QList<QStringList> names,
            bool isVector = false, bool isWidget = false);
private:
    //索引，每种控件一个索引
    static QVector<int> m_indexCount;
    //表达式组字典
    static QMap<QString, CMDExpGroup*> m_groupDict;
    //变量字典
    static QMap<QString, CMDParameter*> m_paraDict;
    //表达式对象列表，拷贝后赋值给各个实际对象
    static QVector<CMDExpManager*> m_expManagerList;
public:
    //需要最后调用
    static void clear();
};
/*!
\section 表达式属性
*/
class MDDRAWOBJSHARED_EXPORT CMDExpInfo
{
public:
    CMDExpInfo(const QString& name="", const QString& showText="",
                  const QString& scriptName="", const QString& comment="");
    CMDExpInfo(const QString& expression, int type, int index);
    CMDExpInfo* clone();
/*!
\section property
*/
public:
    void setName(const QString& value){m_name = value;}
    QString name(){return m_name;}
    void setShowText(const QString& value){m_showText = value;}
    QString showText(){return m_showText;}
    void setComment(const QString& value){m_comment = value;}
    QString comment(){return m_comment;}
    void setScriptName(const QString& value){m_scriptName = value;}
    QString scriptName(){return m_scriptName;}
    void setIndex(int value){m_index = value;}
    int index(){return m_index;}
    void setType(int value){m_type = value;}
    int type(){return m_type;}
    void setExpression(const QString& value){m_expression = value;}
    QString expression(){return m_expression;}
    //返回小写的表达式
    QString lowerExpression(){return m_expression.toLower();}
    void setGroup(CMDExpGroup* value){m_group = value;}
    CMDExpGroup* group(){return m_group;}
    void setContainer(IVarObject* value){m_container = value;}
    IVarObject* container(){return m_container;}
    qreal decimalValue();
    QString stringValue();
    QString stringValue(int decimal, const QString& dateFormat, int maxLength = 99999);
    QDateTime dateTimeValue();
    QList<CMDParameter*>* parameters(){return &m_parameters;}
    QVariant value(){return m_value;}
private:
    QString m_name;         //名称
    QString m_showText;     //显示文本
    QString m_comment;      //注释
    QString m_scriptName;   //脚本名称
    qint32 m_index;         //索引
    qint32 m_type;          //控件类型
    QString m_expression;   //表达式
    CMDExpGroup* m_group;   //组信息
    IVarObject* m_container;//所属控件
    QVariant m_value;       //值
    QList<CMDParameter*> m_parameters;    //表达式中的变量列表
/*!
\section public function
*/
public:
    void onDataChanged();
    QList<QString> getReferencedVarList();
/*!
\section private function
*/
private:
    void calculateExpression();
/*!
\section nsexpression
*/
private:
    CMDExpression m_expTool;//表达式
};

/*!
\section 表达式管理
*/
class MDDRAWOBJSHARED_EXPORT CMDExpManager
{
public:
    CMDExpManager(){}
    CMDExpManager(int type, QList<QStringList>* list);
    ~CMDExpManager();
    CMDExpManager* clone();
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
public:
    QList<CMDExpInfo*>* list(){return &m_list;}
private:
    QList<CMDExpInfo*> m_list;
public:
    //根据数据生成对象
    void append(int type, QList<QStringList>* list);
    void append(CMDExpManager* manager);
    void append(CMDExpInfo* exp);
    void setContainer(IVarObject* container);
    int expressionCount();
    void clear();
private:
    void setExpression(const QString& name, const QString& expression, int &index);
    void removeExpInfo(const QString& name);
};

#endif // MDEXPINFOINFO_H
