/*!
\author bWVpeGw=

\section 功能

组合框控件类
*/
#ifndef MDDRAWCOMBOBOX_H
#define MDDRAWCOMBOBOX_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include <QComboBox>
#include "MDDrawBasicWidget.h"

class CDrawComboBoxData;

class  MDDRAWWIDGETSSHARED_EXPORT CMDDrawComboBox : public CMDDrawBasicWidget
{
public:
    CMDDrawComboBox();
    virtual ~CMDDrawComboBox();

private:
    CDrawComboBoxData* m_data;

    /*!
\section 属性函数
*/
public:
    QComboBox* widget();                        //! 控件
    void setItemStyle(int value);               //! 列表类型
    int itemStyle();
    void setItemAdd(const QStringList &value);  //! 控件子项
    QStringList itemAdd();
    void setLayoutDirection(bool value);        //! 滚动条位置
    bool layoutDirection();

    /*!
\section 脚本属性函数
*/
public:
    void setCurIndex(int num);                  //! 读写当前索引
    int getCurrentIndex();

    void setCurText(QString str);               //! 读写当前文本
    QString getCurText();

    void setSorted(bool value);                 //! 子项排序
    bool Sorted();

    QString indexString(int index);             //! 读取指定索引文本
    void insertItem(int index,QString value);   //! 在索引处添加子项
    void addItem(const QString &value);         //! 下拉框最后出添加子项
    void clearItems();                          //! 清除子项
    void removeItem(int index);                 //! 删除索引项
    int listCount();                            //! 子项数目
    int findString(int index,QString str);      //! 从索引index开始查找与str内容匹配的第一项
    int selectString(int index, QString str);   //! 从索引index开始选中与str内容匹配的第一项

    /*!
\section  控件对象属性
*/
public:
    DrawType type();                            //! 返回控件类型名
    QString className(){return "CMDDrawComboBox";}
    CMDPropertyDrawObj *createPropertyDrawObj();//! 返回属性框对象
    void onInitialization();                    //! 初始化
protected:
    CMDDrawObj *createSelf();                   //! 创建自身
    /*!
\section 序列化
*/
public:
    CMDDrawObj* clone();

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    /*!
\section 绑定相关变量
*/
public:
    static QList<QStringList> expNames();
    void onDataChanged(CMDExpInfo *exp);
    void writeVariable();

    bool writeVarEnabled();
    void setWriteVarEnabled(bool writeVarEnabled);

private:
    void sortItems();                           //! 下拉框子项排序
    bool m_writeVarEnabled;                     //! 可写入变量

/*!
\section batch edit
*/
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
            //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);
    virtual void setTextColor(const QColor& value);
    virtual void setBackColor(const QColor& value);
    virtual void setDisableBackColor(const QColor& value);
    virtual void setFont(const QFont* value);


};

#endif // MDDRAWCOMBOBOX_H

