#ifndef MDRECIPECONFIGER_H
#define MDRECIPECONFIGER_H

#include "MDRecipeConfiger_Global.h"

class CMDRecipe;
class CMDRecipeConfigerImp;
class MDRECIPECONFIGERSHARED_EXPORT CMDRecipeConfiger
{
public:
    CMDRecipeConfiger();
    ~CMDRecipeConfiger();
public:
    void load(const QString& projectPath);
    void unload();
public:
    QString recipePath();
public:
    bool beginEdit(const QString& RecipeName);
    bool endEdit(const QString& RecipeName, bool save);
    CMDRecipe* findEdit(const QString& RecipeName);
    bool saveEdit(const QString& name);
public:
    bool create(const QString& RecipeName);
    bool create(const QString& srcRecipeName, const QString& destRecipeName);
    bool remove(const QString& RecipeName);
    bool exists(const QString& RecipeName);
    int save(const QString& recipeName, const QString& content);
    QString getContent(const QString& recipeName);
    QString toFileName(const QString& RecipeName);
public:
    int getRecipeCount(bool onlyFile);
    QString recipeName(int index);
    QList<QString> enumRecipe(bool onlyFile);

protected:
    CMDRecipeConfigerImp* m_imp;
};

#endif // MDRECIPECONFIGER_H
