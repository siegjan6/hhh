#include "DrawRecipeGridData.h"

//////////////////////////////////////////////////////////////////////////
CDrawRecipeGridData::CDrawRecipeGridData()
{
}
CDrawRecipeGridData& CDrawRecipeGridData::operator=(const CDrawRecipeGridData &other)
{
    m_recipeGrid = other.m_recipeGrid;
    return *this;
}
void CDrawRecipeGridData::serialize(QJsonObject &json)
{
    m_recipeGrid.serialize(json);
}

void CDrawRecipeGridData::deserialize(const QJsonObject &json)
{
    m_recipeGrid.deserialize(json);
}

void CDrawRecipeGridData::changeLanguage()
{
    m_recipeGrid.changeLanguage();
}

void CDrawRecipeGridData::collectWords()
{
    m_recipeGrid.collectWords();
}



