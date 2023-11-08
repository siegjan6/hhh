#include <QDebug>
#include <QVariant>
#include "MDDrawRecipeGrid.h"
#include "DrawRecipeGridData.h"
#include "PropertyDrawRecipeGrid.h"

CMDDrawRecipeGrid::CMDDrawRecipeGrid() :
    m_data(new CDrawRecipeGridData)
{
    QWidget *widget = m_data->m_recipeGrid.widget();
    setWidget(widget);
}
CMDDrawRecipeGrid::~CMDDrawRecipeGrid()
{
    delete m_data;
}
void CMDDrawRecipeGrid::changeLanguage()
{
    m_data->changeLanguage();
}
void CMDDrawRecipeGrid::collectWords()
{
    m_data->collectWords();
}
void CMDDrawRecipeGrid::addAddtionalExpression()
{
    QList<int> indexs;
    QList<QString> exps;
    m_data->m_recipeGrid.getExpressionList(indexs, exps);

    addExpressionToManager(exps);
}
void CMDDrawRecipeGrid::onDataChanged(CMDExpInfo *exp)
{
    m_data->m_recipeGrid.setValue(exp->index(), exp->value());
}

//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawRecipeGrid::type()
{
    return DrawType::RecipeGrid;
}
CMDPropertyDrawObj *CMDDrawRecipeGrid::createPropertyDrawObj()
{
    return new CPropertyDrawRecipeGrid(this);
}
void CMDDrawRecipeGrid::onInitialization()
{
    CMDDrawWidget::onInitialization();
    m_data->m_recipeGrid.setDrawEnvrionment(environment(), parent(), name());
    if(isStudioMode())
        m_data->m_recipeGrid.widget()->setEnabled(true);
}
void CMDDrawRecipeGrid::updateEnabled()
{
    CMDDrawWidget::updateEnabled();
    if(isStudioMode())
        m_data->m_recipeGrid.widget()->setEnabled(true);
}
bool CMDDrawRecipeGrid::isVisible(const QPointF& point)
{
    if(isStudioMode())
    {
        QRectF rf = rect();
        rf.adjust(-10, -10, 10, 10);
        return rf.contains(point);
    }

    return CMDDrawWidget::isVisible(point);
}
CMDDrawObj *CMDDrawRecipeGrid::createSelf()
{
    return new CMDDrawRecipeGrid();
}

void CMDDrawRecipeGrid::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);
    QJsonObject recipeJson;
    m_data->serialize(recipeJson);
    json.insert("CMDDrawRecipeGrid", recipeJson);
}

void CMDDrawRecipeGrid::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);
    QJsonObject recipeJson = json.value("CMDDrawRecipeGrid").toObject();
    m_data->deserialize(recipeJson);
}
CMDDrawObj *CMDDrawRecipeGrid::clone()
{
    CMDDrawRecipeGrid* obj = static_cast<CMDDrawRecipeGrid*>(CMDDrawWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

/////////////////////////////////////脚本接口///////////////////////////////////////////
void CMDDrawRecipeGrid::redrawAll()
{
    m_data->m_recipeGrid.redrawAll();
}

void CMDDrawRecipeGrid::redrawCell(qint32 row, qint32 col)
{
    m_data->m_recipeGrid.redrawCell(row, col);
}

void CMDDrawRecipeGrid::redrawRange(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol)
{
    m_data->m_recipeGrid.redrawRange(startRow, startCol, endRow, endCol);
}

qint32 CMDDrawRecipeGrid::getNumberRows()
{
    return m_data->m_recipeGrid.getNumberRows();
}

void CMDDrawRecipeGrid::setNumberRows(qint32 count)
{
    m_data->m_recipeGrid.setNumberRows(count);
}

void CMDDrawRecipeGrid::insertRows(qint32 startRow, qint32 count)
{
    m_data->m_recipeGrid.insertRows(startRow, count);
}

void CMDDrawRecipeGrid::deleteRows(qint32 startRow, qint32 count)
{
    m_data->m_recipeGrid.deleteRows(startRow,count);
}

qint32 CMDDrawRecipeGrid::getRowHeight(qint32 row)
{
    return m_data->m_recipeGrid.getRowHeight(row);
}

void CMDDrawRecipeGrid::setRowHeight(qint32 row, qint32 height)
{
    m_data->m_recipeGrid.setRowHeight(row, height);
}

qint32 CMDDrawRecipeGrid::getNumberCols()
{
    return m_data->m_recipeGrid.getNumberCols();
}

void CMDDrawRecipeGrid::setNumberCols(qint32 count)
{
    m_data->m_recipeGrid.setNumberCols(count);
}

void CMDDrawRecipeGrid::insertCols(qint32 startCol, qint32 count)
{
    m_data->m_recipeGrid.insertCols(startCol, count);
}

void CMDDrawRecipeGrid::deleteCols(qint32 startCol, qint32 count)
{
    m_data->m_recipeGrid.deleteCols(startCol, count);
}

qint32 CMDDrawRecipeGrid::getColWidth(qint32 col)
{
    return m_data->m_recipeGrid.getColWidth(col);
}

void CMDDrawRecipeGrid::setColWidth(qint32 col, qint32 width)
{
    m_data->m_recipeGrid.setColWidth(col, width);
}

void CMDDrawRecipeGrid::gotoRow(qint32 row)
{
    m_data->m_recipeGrid.gotoRow(row);
}

void CMDDrawRecipeGrid::gotoCol(qint32 col)
{
    m_data->m_recipeGrid.gotoCol(col);
}

QString CMDDrawRecipeGrid::getCellText(qint32 row, qint32 col)
{
    return m_data->m_recipeGrid.getCellText(row,col);
}

void CMDDrawRecipeGrid::setCellText(qint32 row, qint32 col, QString text)
{
    m_data->m_recipeGrid.setCellText(row,col,text);
}

void CMDDrawRecipeGrid::enableRegionEdit(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, bool enableEdit)
{
    m_data->m_recipeGrid.enableRegionEdit(startRow, startCol, endRow, endCol, enableEdit);
}

void CMDDrawRecipeGrid::setBackgroundColor(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 red, qint32 green, qint32 blue)
{
    m_data->m_recipeGrid.setBackgroundColor(startRow, startCol, endRow, endCol, red, green, blue);
}

void CMDDrawRecipeGrid::copyAndReplaceRegion(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 toRow, qint32 toCol, QString strSrc, QString strDes)
{
    m_data->m_recipeGrid.copyAndReplaceRegion(startRow, startCol, endRow, endCol, toRow, toCol, strSrc, strDes);
}

void CMDDrawRecipeGrid::getRangeRecipeName(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, QStringList &nameArray)
{
    m_data->m_recipeGrid.getRangeRecipeName(startRow, startCol, endRow, endCol, nameArray);
}
