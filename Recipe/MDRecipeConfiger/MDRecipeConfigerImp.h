#ifndef MDRECIPECONFIGERIMP_H
#define MDRECIPECONFIGERIMP_H

#include <QMap>
#include "MDRecipeFile.h"
#include "MDRecipeProperty_Config.h"

class CMDRecipeConfigerImp
{
public:
    CMDRecipeConfigerImp();

public:
    void load(const QString& projectPath);
    void unload();
    QString makePath();
    QString recipePath();
public:
    QMap<QString, CMDRecipeFile*> m_editingRecipeMap;
    bool beginEdit(const QString& RecipeName);
    bool endEdit(const QString& RecipeName, bool save);
    CMDRecipeFile* findEdit(const QString& RecipeName);
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
    QStringList enumRecipe(bool onlyFile);
    QStringList m_recipeNameList;
    QString m_projectPath;
public:
    void test();
private:
    bool FindFile(const QString& path, QStringList& nameList, bool onlyFile);
};

#endif // MDRECIPECONFIGERIMP_H
