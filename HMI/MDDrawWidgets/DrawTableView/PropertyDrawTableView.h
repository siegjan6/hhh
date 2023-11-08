/*!
\author aGRw 2015.12.19
\section 表格属性
*/
#ifndef MDPROPERTYDRAWTABLEVIEW_H
#define MDPROPERTYDRAWTABLEVIEW_H

#include "MDDrawTableView.h"
#include "MDPropertyDrawBasicWidget.h"

class CPropertyDrawTableView : public CMDPropertyDrawBasicWidget
{
    Q_OBJECT
    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount)
    Q_PROPERTY(int colCount READ colCount WRITE setColCount)
    Q_PROPERTY(int rowHeight READ rowHeight WRITE setRowHeight)
    Q_PROPERTY(int colWidth READ colWidth WRITE setColWidth)
    Q_PROPERTY(bool hideRowHeader READ hideRowHeader WRITE setHideRowHeader)
    Q_PROPERTY(bool hideColHeader READ hideColHeader WRITE setHideColHeader)
public:
    CPropertyDrawTableView(CMDDrawObj *parent);
    virtual ~CPropertyDrawTableView(){}
protected:
    CMDDrawTableView *object();
/*!
\section  function
*/
public:
    void setReadOnly(bool value);
    bool readOnly();
    void setRowCount(int value);
    int rowCount();
    void setColCount(int value);
    int colCount();
    void setRowHeight(int value);
    int rowHeight();
    void setColWidth(int value);
    int colWidth();
    void setHideRowHeader(bool value);
    bool hideRowHeader();
    void setHideColHeader(bool value);
    bool hideColHeader();
};

#endif  //MDPROPERTYDRAWTABLEVIEW_H


