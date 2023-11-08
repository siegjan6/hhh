#ifndef MDRECIPEFILE_H
#define MDRECIPEFILE_H

#include "MDRecipeHelper_Global.h"

class CMDRecipe;
class CMDRecipeFileData;
class MDRECIPEHELPERSHARED_EXPORT CMDRecipeFile
{
public:
    CMDRecipeFile();
    ~CMDRecipeFile();
    CMDRecipeFile(const CMDRecipeFile& obj);
    CMDRecipeFile& operator = (const CMDRecipeFile& obj);
    bool copy(const CMDRecipeFile* source);
public:
    static bool load(const QString& fileName, CMDRecipe& fileRecipe);
    bool load();
    bool reload();
    void unload();
    void save();
    static int save(const QString& fileName, const QString& content);
    QString getContent(const QString& fileName);
public:
    QString fileName();
    void setFileName(const QString& fileName);
public:
    CMDRecipe* recipe();
protected:
    CMDRecipeFileData* m_data;
};

#endif // MDRECIPEFILE_H
