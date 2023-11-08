#include "MDRecipeGrid.h"
#include "RecipeGridView.h"
#include "ShareTool.h"
#include "RecipeGridView.h"
#include "ValueMap/ValueMapManager.h"


CMDRecipeGrid::CMDRecipeGrid()
{
    gValueMapManager = CValueMapManager::getInstance();

    m_recipeView = new CRecipeGridView(NULL);
    m_recipeView->init();
}

CMDRecipeGrid::~CMDRecipeGrid()
{
    if(m_recipeView)
    {
        delete m_recipeView;
    }
    m_recipeView = NULL;
}

QWidget *CMDRecipeGrid::widget()
{
    return m_recipeView;
}

void CMDRecipeGrid::setDrawEnvrionment(IDrawEnvrionment *value, IHMIForm *mainform, QString jsObjName)
{
    gValueMapManager->readFile(value);
    CShareTool::setDrawEnvrionment(value);
    m_recipeView->onInitialization(value, mainform, jsObjName);
}

void CMDRecipeGrid::serialize(QJsonObject &json)
{
    m_recipeView->serialize(json);
}

void CMDRecipeGrid::deserialize(const QJsonObject &json)
{
    m_recipeView->deserialize(json);
}

CMDRecipeGrid& CMDRecipeGrid::operator=(const CMDRecipeGrid &other)
{
    *m_recipeView = *other.m_recipeView;
    return *this;
}

void CMDRecipeGrid::getExpressionList(QList<int> &indexs, QList<QString> &exps)
{
    m_recipeView->getExpressionList(indexs, exps);
}

void CMDRecipeGrid::setValue(int index, QVariant variant)
{
    m_recipeView->setValue(index, variant);
}

void CMDRecipeGrid::changeLanguage()
{
    m_recipeView->changeLanguage();
}

void CMDRecipeGrid::collectWords()
{
    m_recipeView->collectWords();
}



/////////////////////////////////////脚本接口///////////////////////////////////////////
void CMDRecipeGrid::redrawAll()
{
    m_recipeView->redrawAll();
}

void CMDRecipeGrid::redrawCell(qint32 row, qint32 col)
{
    m_recipeView->redrawCell(row, col);
}

void CMDRecipeGrid::redrawRange(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol)
{
    m_recipeView->redrawRange(startRow, startCol, endRow, endCol);
}

qint32 CMDRecipeGrid::getNumberRows()
{
    return m_recipeView->getNumberRows();
}

void CMDRecipeGrid::setNumberRows(qint32 count)
{
    m_recipeView->setNumberRows(count);
}

void CMDRecipeGrid::insertRows(qint32 startRow, qint32 count)
{
    m_recipeView->insertRows(startRow,count);
}

void CMDRecipeGrid::deleteRows(qint32 startRow, qint32 count)
{
    m_recipeView->deleteRows(startRow, count);
}

qint32 CMDRecipeGrid::getRowHeight(qint32 row)
{
    return m_recipeView->getRowHeight(row);
}

void CMDRecipeGrid::setRowHeight(qint32 row, qint32 height)
{
    m_recipeView->setRowHeightJs(row, height);
}

qint32 CMDRecipeGrid::getNumberCols()
{
    return m_recipeView->getNumberCols();
}

void CMDRecipeGrid::setNumberCols(qint32 count)
{
    m_recipeView->setNumberCols(count);
}

void CMDRecipeGrid::insertCols(qint32 startCol, qint32 count)
{
    m_recipeView->insertCols(startCol, count);
}

void CMDRecipeGrid::deleteCols(qint32 startCol, qint32 count)
{
    m_recipeView->deleteCols(startCol, count);
}

qint32 CMDRecipeGrid::getColWidth(qint32 col)
{
    return m_recipeView->getColWidth(col);
}

void CMDRecipeGrid::setColWidth(qint32 col, qint32 width)
{
    m_recipeView->setColWidth(col, width);
}

void CMDRecipeGrid::gotoRow(qint32 row)
{
    m_recipeView->gotoRow(row);
}

void CMDRecipeGrid::gotoCol(qint32 col)
{
    m_recipeView->gotoCol(col);
}

QString CMDRecipeGrid::getCellText(qint32 row, qint32 col)
{
    return m_recipeView->getCellText(row,col);
}

void CMDRecipeGrid::setCellText(qint32 row, qint32 col, QString text)
{
    m_recipeView->setCellText(row,col,text);
}

void CMDRecipeGrid::enableRegionEdit(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, bool enableEdit)
{
    m_recipeView->enableRegionEdit(startRow, startCol, endRow, endCol, enableEdit);
}

void CMDRecipeGrid::setBackgroundColor(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 red, qint32 green, qint32 blue)
{
    m_recipeView->setBackgroundColor(startRow, startCol, endRow, endCol, red, green, blue);
}

void CMDRecipeGrid::copyAndReplaceRegion(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 toRow, qint32 toCol, QString strSrc, QString strDes)
{
    m_recipeView->copyAndReplaceRegion(startRow, startCol, endRow, endCol, toRow, toCol, strSrc, strDes);
}

void CMDRecipeGrid::getRangeRecipeName(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, QStringList &nameArray)
{
    m_recipeView->getRangeRecipeName(startRow, startCol, endRow, endCol, nameArray);
}


