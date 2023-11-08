/*!
\author aGRw 2016.09.21
\section 配方控件,数据类
*/
#ifndef DRAWRECIPEGRIDDATA_H
#define DRAWRECIPEGRIDDATA_H

#include "MDRecipeGrid.h"

class CDrawRecipeGridData
{
public:
    CDrawRecipeGridData();
    ~CDrawRecipeGridData(){}

public:
    CMDRecipeGrid m_recipeGrid;

public:
    CDrawRecipeGridData& operator=(const CDrawRecipeGridData& other);
    //! json格式接口
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    /*!
    \section 多语言
    */
    void changeLanguage();
    void collectWords();

};

#endif // DRAWRECIPEGRIDDATA_H
