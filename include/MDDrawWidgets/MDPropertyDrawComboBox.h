/*!
\author bWVpeGw=

\section 功能

组合框控件属性类
*/
#ifndef MDPROPERTYDRAWCOMBOBOX_H
#define MDPROPERTYDRAWCOMBOBOX_H

#include "MDPropertyDrawObj.h"
#include "MDDrawComboBox.h"
#include <QTextEdit>
#include <QMap>
#include "MDPropertyDrawBasicWidget.h"
class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawComboBox : public CMDPropertyDrawBasicWidget
{
    Q_OBJECT

    Q_ENUMS(propertyItemStyle)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(bool layoutDirection READ layoutDirection WRITE setLayoutDirection)
    Q_PROPERTY(propertyItemStyle dropDownStyle READ dropDownStyle WRITE setDropDownStyle)
    Q_PROPERTY(QString DILitem READ DILitem WRITE setDILItem)
    Q_PROPERTY(bool Sorted READ Sorted WRITE setSorted)
    /*!
\section 子项显示样式
*/
public:
    enum class propertyItemStyle {DropItem,DropListItem};
    /*!
\section 属性函数
*/
public:
    //! 列表类型
    void setDropDownStyle(propertyItemStyle value);
    propertyItemStyle dropDownStyle();
    //! 控件子项
    void setDILItem(const QString &value);
    QString DILitem();
    //! 滚动条位置
    void setLayoutDirection(bool value);
    bool layoutDirection();
    //! 子项排序
    void setSorted(bool value);
    bool Sorted();

public:
    CMDPropertyDrawComboBox(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawComboBox(){}

protected:
    CMDDrawComboBox* widget();

/*!
\section 设置属性值
*/
public:
    void resetAttribute();

private:
    //! 换行符
    QString breakString() const
    {
#ifdef Q_OS_LINUX
        return {"\n"};
#elif defined(Q_OS_WIN)
        return {"\n"};
#endif
    }

/*!
\section batch edit
*/
private:
    void setBatchDropDownStyle(propertyItemStyle value);
    void setBatchDILItem(QStringList list);
    void setBatchLayoutDirection(bool value);
    void setBatchSorted(bool value);
};

#endif // MDPROPERTYDRAWCOMBOBOX_H
