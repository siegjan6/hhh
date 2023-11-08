/*!
\author Y2hhaWhq

\section 功能

组合框控件脚本类
*/
#ifndef MDDRAWCOMBOBOXWRAPPER_H
#define MDDRAWCOMBOBOXWRAPPER_H

#include <QVariant>
#include "MDDrawWidgetWrapper.h"
class CMDDrawComboBox;

class MDHMIFORMSHARED_EXPORT CMDDrawComboBoxWrapper : public CMDDrawWidgetWrapper
{
    Q_OBJECT

    Q_PROPERTY(int listCount READ listCount WRITE setListCount)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
    Q_PROPERTY(int listStyle READ listStyle WRITE setListStyle)
    Q_PROPERTY(QString currentText READ currentText WRITE setCurrentText)
    Q_PROPERTY(int sorted READ sorted WRITE setSorted)

public:
    CMDDrawComboBoxWrapper(void* object, const QString& name);
    virtual ~CMDDrawComboBoxWrapper(){}

public Q_SLOTS:
    //! 子项数目
    int listCount();
    void setListCount(int value);
    //! 当前索引
    int currentIndex();
    void setCurrentIndex(int value);
    //! 列表样式
    int listStyle();
    void setListStyle(int value);
    //! 当前文本
    QString currentText();
    void setCurrentText(const QString& str);
    //! 子项排序
    bool sorted();
    void setSorted(bool sort);
    //! 读取指定索引文本
    QString indexString(int index);
    //! 指定索引处添加子项
    void insertItem(int index, const QVariant &value);
    //! 在下拉框最后项处添加子项
    void addItem(const QString &value);
    //! 清除所有子项
    void clearItems();
    //! 删除指定子项
    void removeItem(int index);
    //! 从索引index开始查找与str内容匹配的第一项
    int findString(int index, QString value);
    //! 从索引index开始选中与str内容匹配的第一项
    int selectString(int index,  QString value);

public:
    CMDDrawComboBox* object();
};

#endif // MDDRAWCOMBOBOXWRAPPER_H
