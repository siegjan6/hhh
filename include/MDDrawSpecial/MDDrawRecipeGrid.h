/*!
\author aGRw 2016.09.21
\section 配方控件
*/
#ifndef MDDRAWRECIPEGRID_H
#define MDDRAWRECIPEGRID_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDDrawWidget.h"

class CDrawRecipeGridData;


class MDDRAWSPECIALSHARED_EXPORT CMDDrawRecipeGrid : public CMDDrawWidget
{
public:
    CMDDrawRecipeGrid();
    virtual ~CMDDrawRecipeGrid();
private:
    CDrawRecipeGridData *m_data;
/*!
\section virtual property
*/
public:
    virtual DrawType type();
    QString className(){return "CMDDrawRecipeGrid";}
/*!
\section virtual function
*/
public:
    //返回属性框对象
    virtual CMDPropertyDrawObj *createPropertyDrawObj();
    //初始化
    virtual void onInitialization();
    virtual void updateEnabled();
    virtual bool isVisible(const QPointF& point);
protected:
    //创建自身
    virtual CMDDrawObj *createSelf();
public:
    //json格式接口
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    CMDDrawObj* clone() ;

/*!
\section 多语言
*/
public:
    void changeLanguage();
    void collectWords();
/*!
\section 变量相关
*/
public:
    void addAddtionalExpression();
    void onDataChanged(CMDExpInfo *expression);

    /*!
    \section 脚本接口函数
    */
    void redrawAll();
    void redrawCell(qint32 row, qint32 col);
    void redrawRange(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol);
    qint32 getNumberRows();
    void setNumberRows(qint32 count);
    void insertRows(qint32 startRow, qint32 count);
    void deleteRows(qint32 startRow, qint32 count);
    qint32 getRowHeight(qint32 row);
    void setRowHeight(qint32 row, qint32 height);
    qint32 getNumberCols();
    void setNumberCols(qint32 count);
    void insertCols(qint32 startCol, qint32 count);
    void deleteCols(qint32 startCol, qint32 count);
    qint32 getColWidth(qint32 col);
    void setColWidth(qint32 col, qint32 width);
    void gotoRow(qint32 row);
    void gotoCol(qint32 col);
    QString getCellText(qint32 row, qint32 col);
    void setCellText(qint32 row, qint32 col, QString text);
    void enableRegionEdit(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, bool enableEdit);
    void setBackgroundColor(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 red, qint32 green, qint32 blue);
    void copyAndReplaceRegion(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 toRow, qint32 toCol, QString strSrc, QString strDes);
    void getRangeRecipeName(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, QStringList& nameArray);
};

#endif // MDDRAWRECIPEGRID_H
