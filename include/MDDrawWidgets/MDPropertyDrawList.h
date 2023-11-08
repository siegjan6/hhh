/*!
\author bWVpeGw=

\section 功能

列表控件属性类
*/
#ifndef MDPROPERTYDRAWLISTWIDGET_H
#define MDPROPERTYDRAWLISTWIDGET_H

#include "MDPropertyDrawObj.h"
#include "MDDrawList.h"
#include "MDPropertyDrawBasicWidget.h"
class MDDRAWWIDGETSSHARED_EXPORT CMDPropertyDrawList : public CMDPropertyDrawBasicWidget
{
    Q_OBJECT
    Q_ENUMS(SelectionMode)
    Q_ENUMS(PropertyBorderStyle)
    Q_ENUMS(PropertyItemSortedStyle)
    Q_ENUMS(LayoutDirection)

    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(SelectionMode selectionMode READ selectionMode WRITE setSelectionMode)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(bool hasScrollBar READ hasScrollBar WRITE setHasScrollBar)
    Q_PROPERTY(LayoutDirection layoutDirection READ layoutDirection WRITE setLayoutDirection)
    Q_PROPERTY(PropertyBorderStyle borderStyle READ borderStyle WRITE setBorderStyle)
    Q_PROPERTY(PropertyItemSortedStyle Sorted READ Sorted WRITE setSorted)
    Q_PROPERTY(QString DILitem READ DILitem WRITE setDILItem)
public:
    /*!
    \section 选择模式
    */
    enum class SelectionMode {NoSelection,SingleSelection,MultiSelection};
    /*!
    \section 控件边框样式
    */
    enum class PropertyBorderStyle {NoFrame,StyledPanel,WinPanel};
    /*!
    \section 滚动条位置
    */
    enum class LayoutDirection {LeftToRight,RightToLeft};
    /*!
    \section 子项排序方式
    */
    enum class PropertyItemSortedStyle {NoOrder, AscendingOrder, DescendingOrder};

    /*!
    \section 列表模式
    */
    QMap<QAbstractItemView::SelectionMode, QString> m_selectModeToString;
public:
    //! 滚动条
    void setHasScrollBar(bool value);
    bool hasScrollBar();
    //! 边框
    void setBorderStyle(PropertyBorderStyle value);
    PropertyBorderStyle borderStyle();
    //! 模式选择
    void setSelectionMode(SelectionMode value);
    SelectionMode selectionMode();
    //! 滚动条居左
    void setLayoutDirection(LayoutDirection value);
    LayoutDirection layoutDirection();
    //! 控件子项
    void setDILItem(const QString &value);
    QString DILitem();
    //! 子项排序
    void setSorted(PropertyItemSortedStyle value);
    PropertyItemSortedStyle Sorted();

public:
    CMDPropertyDrawList(CMDDrawObj *parent);
    virtual ~CMDPropertyDrawList(){}

protected:
    CMDDrawList* widget();

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
    void setBatchHasScrollBar(bool value);
    void setBatchBorderStyle(PropertyBorderStyle value);
    void setBatchSelectionMode(SelectionMode value);
    void setBatchLayoutDirection(LayoutDirection value);
    void setBatchDILItem(QStringList list);
    void setBatchSorted(PropertyItemSortedStyle value);
};

#endif // MDPROPERTYDRAWLISTWIDGET_H

