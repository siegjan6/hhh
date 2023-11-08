/*!
\author bWVpeGw=

\section 功能

列表控件类
*/
#ifndef MDDRAWLISTWIDGET_H
#define MDDRAWLISTWIDGET_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDDrawBasicWidget.h"
#include <QListWidget>
#include <QJsonObject>

class CDrawListData;

class  MDDRAWWIDGETSSHARED_EXPORT CMDDrawList : public CMDDrawBasicWidget
{
public:
    CMDDrawList();
    virtual ~CMDDrawList();

private:
    CDrawListData* m_data;
/*!
\section 属性函数
*/
public:
    QListWidget* widget();                                          //! 控件
    void setHasScrollBar(bool value);                               //! 滚动条
    bool hasScrollBar();
    void setBorderStyle(QFrame::Shape value);                       //! 边框
    QFrame::Shape borderStyle();
    void setSelectionMode(QAbstractItemView::SelectionMode value);  //! 显示模式
    QAbstractItemView::SelectionMode selectionMode();
    void setLayoutDirection(Qt::LayoutDirection value);             //! 滚动条位置
    Qt::LayoutDirection layoutDirection();
    void setItemAdd(const QStringList &value);                      //! 添加子项
    QStringList itemAdd();

/*!
\section 脚本属性函数
*/
public:
    void setCurrentIndex(int num);              //! 当前索引
    int currentIndex();
    QString currentItem();                      //! 当前选项
    void setSorted(int value);                  //! 子项排序
    int sorted();
    int listCount();                            //! 子项数目
    int selectCount();                          //! 多选模式下选中的子项数目
    QString indexString(int index);             //! 读取指定索引文本
    void insertItem(int index,QString value);   //! 指定索引处添加子项
    void addItem(const QString &value);         //! 列表子项最后出添加子项
    void clearItems();                          //! 清除子项
    void removeItem(int index);                 //! 移除指定索引的子项
    int findString(int index,QString str);      //! 从索引index开始查找与str内容匹配的第一项
    void setSortMode(qint32 value);             //! 设置排序模式

/*!
\section 控件对象属性
*/
public:  
    DrawType type();                            //! 返回控件类型名
    QString className(){return "CMDDrawList";}
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
\section batch edit
*/
public:
    virtual CMDPropertyDrawObj* createPropertyDrawObj(const QString& className);
protected:
    //初始化类信息
    virtual void initClassInfo(QStringList &classInfo);
};

#endif // MDDRAWLISTWIDGET_H
