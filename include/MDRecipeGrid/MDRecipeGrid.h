/*!
author Y2hvdWps 2016.09.21
section 配方表格控件导出类
*/
#ifndef MDRECIPEGRID_H
#define MDRECIPEGRID_H

#include <QtCore/qglobal.h>
#if defined(MDRECIPEGRID_LIBRARY)
#  define MDRECIPEGRIDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDRECIPEGRIDSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QWidget>
#include <QEvent>
#include "IDrawEnvironment.h"

class IHMIForm;
class CRecipeGridView;

class MDRECIPEGRIDSHARED_EXPORT CMDRecipeGrid
{

public:
    CMDRecipeGrid();
    virtual ~CMDRecipeGrid();

public:
     QWidget *widget();
     void setDrawEnvrionment(IDrawEnvrionment* value, IHMIForm* mainform, QString jsObjName);

     /*!
     \section  序列化
     */
     void serialize(QDataStream& stream, bool isOrigVersion = false);
     void deserialize(QDataStream& stream);
     //! json格式接口
     void serialize(QJsonObject& json);
     void deserialize(const QJsonObject& json);

     CMDRecipeGrid& operator=(const CMDRecipeGrid& other);
     /*!
     \section  变量相关
     */
     void getExpressionList(QList<int>& indexs, QList<QString>& exps);
     void setValue(int index, QVariant variant);
     /*!
     \section  多语言
     */
     void changeLanguage();
     void collectWords();
     /*!
     \section  脚本接口函数
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
private:
     CRecipeGridView* m_recipeView;
};

#endif // MDRECIPEGRID_H
