#ifndef MDRECIPEEXPERT_H
#define MDRECIPEEXPERT_H

#include <QVariant>
#include "MDRecipeExpert_Global.h"

class CMDRecipeContacter;
class CMDRecipeExpertImp;
class CMDRecipe;
class CMDRecipeConfiger;
class MDRECIPEEXPERTSHARED_EXPORT CMDRecipeExpert
{
public:
    CMDRecipeExpert();
    ~CMDRecipeExpert();
/*!
\section 初始化与释放
*/
public:
    void setDownloadProcessContacter(CMDRecipeContacter* contacter);
    void setParaBeforeLoad(const QString& pid, CMDRecipeConfiger* configer, const QString& projectPath, CMDRecipeContacter* contacter);
    void load();
    void unload();
public:
    CMDRecipeConfiger* configer();
public:
    bool download(const QString& RecipeName, int showProcessDialog = 1);
    bool upload(const QString& RecipeName);
public:
    void taskNameList(QStringList& nameList);
    int taskCount();
public:
    void deleteSomeTask(const QStringList& nameList);
    void deleteTask(const QString& name);
    void deleteAllTask();
public:
    void downloadingTask(CMDRecipe& recipe);
    QVariant itemVarValue(const QString& itemID);
protected:
    CMDRecipeExpertImp* m_imp;
};

#endif // MDRECIPEEXPERT_H
