#ifndef MDRECIPEFILEDATA_H
#define MDRECIPEFILEDATA_H
#include <QString>
#include "MDRecipe.h"

class CMDRecipeFileData
{
public:
    CMDRecipeFileData();
    ~CMDRecipeFileData();
    QString m_fileName;
public:
    CMDRecipe m_recipe;

public:
    static bool load(const QString& fileName, CMDRecipe& fileRecipe);
    bool load();
    bool reload();
    void unload();
    void save();
    static int save(const QString& fileName, const QString& content);
    static QString getContent(const QString& fileName);
};

#endif // MDRECIPEFILEDATA_H
