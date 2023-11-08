#ifndef MDRECIPE_H
#define MDRECIPE_H

#include "MDRecipeHelper_Global.h"
#include "MDRecipeProperty_Config.h"
#include "MDRecipeProperty_Runtime.h"

class CMDRecipeData;
class MDRECIPEHELPERSHARED_EXPORT CMDRecipe
{
public:
    CMDRecipe();
    virtual ~CMDRecipe();
    CMDRecipe(const CMDRecipe& obj);
    CMDRecipe& operator = (const CMDRecipe& obj);
public:
    void copy(const CMDRecipe* source);
public:
    QString name() const;
    DownloadMode downloadMode() const;
public:
    void clear();
public:
    CMDRecipeProperty_Config* configProperty() const;
    CMDRecipeProperty_Runtime* runtimeProperty() const;
/*!
\section 辅助函数
*/
public:
   static void enumDownloadModes(QStringList& modeList);
   static QString modeToString(DownloadMode mode);
   static DownloadMode stringToMode(const QString& str);
protected:
    CMDRecipeData* m_data;
};

#endif // MDRECIPE_H
