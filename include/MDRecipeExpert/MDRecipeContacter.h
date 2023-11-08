#ifndef MDRECIPECONTACTER_H
#define MDRECIPECONTACTER_H

#include <QVariant>
#include "MDRecipeExpert_Global.h"
#include "../MDRecipeHelper/MDRecipeProperty_Runtime.h"

class CMDRecipe;
class MDRECIPEEXPERTSHARED_EXPORT CMDRecipeContacter
{
public:
    CMDRecipeContacter();
public:
    virtual bool writeVarGeneral(CMDRecipe* recipe);
    virtual bool writeVarQuick(CMDRecipe* recipe);
    virtual QVariant varValue(const QString &varName);
    virtual int getVariableQuality(const QString& varName);
    virtual bool isQuickVar(const QString &varName);
public:
    virtual void onDownloadTaskAdded(const QString &name, int showProcessDialog=1);
    virtual void onBeginDownloading(const QString &name, int total);
    virtual void onEndDownloading(const QString &name, DownloadFinishFlag finishFlag);
    virtual void onDownloadProcess(const QString &msg, int total, int completed);
};

#endif // MDRECIPECONTACTER_H
