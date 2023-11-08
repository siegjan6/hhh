/*!
\author aGRw 2015.12.16
\section 表格控件
*/
#ifndef MDDRAWTABLEVIEW_H
#define MDDRAWTABLEVIEW_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWWIDGETS_LIBRARY)
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWWIDGETSSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QTableView>
#include <QStandardItem>
#include "MDDrawBasicWidget.h"



class CDrawTableViewData;

class MDDRAWWIDGETSSHARED_EXPORT CMDDrawTableView : public CMDDrawBasicWidget
{
public:
    CMDDrawTableView();
    virtual ~CMDDrawTableView();
private:
    CDrawTableViewData* m_data;
/*!
\section enum
*/
public:
    enum class ItemType
    {
        String,
        Double
    };
private:
    enum class DelegateType
    {
        Normal,
        ReadOnly,
        Double
    };
/*!
\section property
*/
public:
    QTableView* widget();
    void setColCount(int value);
    int colCount();
    void setRowCount(int value);
    int rowCount();
    void setColWidth(int value);
    int colWidth();
    void setRowHeight(int value);
    int rowHeight();
    void setHideRowHeader(bool value);
    bool hideRowHeader();
    void setHideColHeader(bool value);
    bool hideColHeader();
    void setReadOnly(bool value);
    bool readOnly();
/*!
\section script function
*/
public:
    //!单元类型
    void setItemTypeRange(int row, int rowCount, int col ,int colCount, int type);
    int itemType(int row, int col);
    //！单元值
    void setItemValueRange(int row, int rowCount, int col ,int colCount, const QVariant& value);
    QVariant itemValue(int row, int col);
    //!行头文本
    void setRowHeaderText(int index, const QString& value);
    //!列头文本
    void setColHeaderText(int index, const QString& value);
    void setSingleRowHeight(int index, int value);
    void setSingleColWidth(int index, int value);
    //!限制输入
    void setReadOnlyRowRange(int row, int rowCount, bool value);
    void setReadOnlyColRange(int col, int colCount, bool value);
    void setDecimalRowRange(int row, int rowCount, int value);
    void setDecimalColRange(int col, int colCount, int value);
    void setItemType(int row, int col, int type);
    void setItemValue(int row, int col, const QVariant& value);
private:
    QStandardItem* item(int row, int col);
    QStandardItem* rowHeader(int row);
    QStandardItem* colHeader(int col);
    bool checkIndex(int& start, int& count, bool isRow);
    void setDelegate(CMDDrawTableView::DelegateType type, bool isRow, int start, int count, int decimal = 0);
/*!
\section drawObj
*/
public:
    DrawType type();
    QString className(){return "CMDDrawTableView";}
    CMDPropertyDrawObj *createPropertyDrawObj();
    void onInitialization();
protected:
    CMDDrawObj *createSelf();
/*!
\section common
*/
public:
    CMDDrawObj* clone();
};

#endif // MDDRAWTABLEVIEW_H

