#include "MDRecipeContacter.h"

CMDRecipeContacter::CMDRecipeContacter()
{
}

bool CMDRecipeContacter::writeVarGeneral(CMDRecipe* recipe)
{
    return false;
}

bool CMDRecipeContacter::writeVarQuick(CMDRecipe* recipe)
{
    return false;
}

QVariant CMDRecipeContacter::varValue(const QString &varName)
{
    return QVariant();
}

int CMDRecipeContacter::getVariableQuality(const QString& varName)
{
    return -1;
}

bool CMDRecipeContacter::isQuickVar(const QString &varName)
{
    return false;
}

void CMDRecipeContacter::onDownloadTaskAdded(const QString &name, int showProcessDialog/*=1*/)
{
}

void CMDRecipeContacter::onBeginDownloading(const QString &name, int total)
{
}

void CMDRecipeContacter::onEndDownloading(const QString &name, DownloadFinishFlag finishFlag)
{
}

void CMDRecipeContacter::onDownloadProcess(const QString &msg, int total, int completed)
{
}

