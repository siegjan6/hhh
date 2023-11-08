/*!
\author Y2hhaWhq

\section 功能

列表控件脚本类
*/
#ifndef MDDRAWLISTWRAPPER_H
#define MDDRAWLISTWRAPPER_H

#include "MDDrawWidgetWrapper.h"
class CMDDrawList;

class CMDDrawListWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(QString currentItem READ currentItem WRITE setCurrentItem)
    Q_PROPERTY(int listCount READ listCount WRITE setListCount)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
    Q_PROPERTY(int borderStyle READ borderStyle WRITE setBorderStyle)
    Q_PROPERTY(int sorted READ sorted WRITE setSorted)
    Q_PROPERTY(int selectCount READ selectCount WRITE setSelectCount)

public:
    CMDDrawListWrapper(void* object, const QString& name);
    virtual ~CMDDrawListWrapper(){}

    //property
public Q_SLOTS:
    //! 当前项目
    QString currentItem();
    void setCurrentItem(const QString& str);
    //! 子项数目
    int listCount();
    void setListCount(int value);
    //! 当前索引
    int currentIndex();
    void setCurrentIndex(int value);
    //! 列表样式
    int borderStyle();
    void setBorderStyle(int value);
    //! 子项排序
    int sorted();
    void setSorted(int value);
    //! 多选模式下选中子项数目
    int selectCount();
    void setSelectCount(int value);
    //! 读取指定索引文本
    QString indexString(const int index);
    //! 指定索引处添加子项
    void insertItem(int index, const QVariant &value);
    //! 在列表子项最后项处添加子项
    void addItem(const QString &value);
    //! 清除所有子项
    void clearItems();
    //! 删除指定子项
    void removeItem(int index);
    //! 从索引index开始查找与str内容匹配的第一项
    int findString(int index, QString str);

public:
    CMDDrawList* object();
};

#endif // MDDRAWLISTWRAPPER_H
