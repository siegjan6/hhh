/*!
\author aGRw 2016.09.21
\section 配方控件,属性类
*/
#ifndef PROPERTYDRAWRECIPEGRID_H
#define PROPERTYDRAWRECIPEGRID_H

#include "MDPropertyDrawObj.h"

class CMDDrawRecipeGrid;

class CPropertyDrawRecipeGrid : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Appearance)

public:
    CPropertyDrawRecipeGrid(CMDDrawObj *parent);
    virtual ~CPropertyDrawRecipeGrid(){}

public:
    QString DILconfig() const;
    void setDILconfig(QString);
protected:
    CMDDrawRecipeGrid* object();
};

#endif // PROPERTYDRAWRECIPEGRID_H
