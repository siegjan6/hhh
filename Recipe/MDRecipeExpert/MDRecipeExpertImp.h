#ifndef MDRECIPEEXPERTIMP_H
#define MDRECIPEEXPERTIMP_H

#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include "MDDownloadExpert.h"

class CMDRecipeContacter;
class CMDRecipeConfiger;
class CMDRecipeExpertImp
{
public:
    CMDRecipeExpertImp();
    ~CMDRecipeExpertImp();
public:
    void setParaBeforeLoad(const QString& pid, CMDRecipeConfiger* configer, const QString& projectPath, CMDRecipeContacter* contacter);
    void load();
    void unload();
public:
    QString m_pid;
    CMDRecipeContacter* m_contater;
    CMDRecipeConfiger* m_configer;
public:
    bool download(const QString& RecipeName, int showProcessDialog = 1);
    bool upload(const QString& RecipeName);
public:
    QString m_projectPath;

public:
    void createSwitchs(bool create);
    CMDDownloadExpert m_downloadExpert;
};

#endif // MDRECIPEEXPERTIMP_H
