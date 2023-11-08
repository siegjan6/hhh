#include "MDRecipeConfigerImp.h"
#include "MDRecipe.h"
#include "MDRecipeFile.h"
#include <QDir>
#include <QCoreApplication>

CMDRecipeConfigerImp::CMDRecipeConfigerImp()
{
}

void CMDRecipeConfigerImp::load(const QString& projectPath)
{
    m_projectPath = projectPath;

    makePath();

//    test();
}

void CMDRecipeConfigerImp::unload()
{
    m_recipeNameList.clear();

    foreach (CMDRecipeFile* recipe, m_editingRecipeMap)
    {
        delete recipe;
    }
    m_editingRecipeMap.clear();
}

QString CMDRecipeConfigerImp::makePath()
{
    QString modulePath = recipePath();
    QDir dir;
    dir.mkpath(modulePath);
    return modulePath;
}

QString CMDRecipeConfigerImp::recipePath()
{
    QString modulePath;
    #ifdef Q_OS_LINUX
    modulePath = m_projectPath + "/Recipe/";
    #else
    modulePath = m_projectPath + "\\Recipe\\";
    #endif
    return modulePath;
}

bool CMDRecipeConfigerImp::beginEdit(const QString& RecipeName)
{
    CMDRecipeFile* recipe = findEdit(RecipeName);
    if(recipe)
        return true;

    CMDRecipeFile* newEditRecipe = new CMDRecipeFile;
    newEditRecipe->setFileName(toFileName(RecipeName));
    newEditRecipe->load();
    m_editingRecipeMap.insert(RecipeName.toUpper(), newEditRecipe);

    return true;
}

bool CMDRecipeConfigerImp::endEdit(const QString& RecipeName, bool save)
{
    bool result = false;

    QMap<QString, CMDRecipeFile*>::iterator iPosition = m_editingRecipeMap.find(RecipeName.toUpper());
    if(iPosition != m_editingRecipeMap.constEnd())
   {
        CMDRecipeFile* object = iPosition.value();
        if(save)
        {
            object->save();
            object->unload();
        }
        delete object;

        m_editingRecipeMap.erase(iPosition);

        result = true;
    }
    return result;
}

CMDRecipeFile* CMDRecipeConfigerImp::findEdit(const QString& RecipeName)
{
    CMDRecipeFile* object = NULL;

    QMap<QString, CMDRecipeFile*>::const_iterator iPosition = m_editingRecipeMap.find(RecipeName.toUpper());
    if(iPosition != m_editingRecipeMap.constEnd())
        object = iPosition.value();

    return object;
}

bool CMDRecipeConfigerImp::saveEdit(const QString& name)
{
    CMDRecipeFile* recipe = findEdit(name);
    if(recipe)
        recipe->save();
    return (recipe != NULL);
}

bool CMDRecipeConfigerImp::create(const QString& RecipeName)
{
    QString fileName = toFileName(RecipeName);
    if(QFile::exists(fileName))
        return false;

    CMDRecipeFile newFile;
    newFile.setFileName(fileName);
    newFile.save();
    return true;
}

bool CMDRecipeConfigerImp::remove(const QString& RecipeName)
{
    QString fileName = toFileName(RecipeName);
    if(!QFile::exists(fileName))
        return false;

    return QFile::remove(fileName);
}

bool CMDRecipeConfigerImp::create(const QString& srcRecipeName, const QString& destRecipeName)
{
    QString srcFileName = toFileName(srcRecipeName);
    if(!QFile::exists(srcFileName))
        return false;

    QString destFileName = toFileName(destRecipeName);
    if(QFile::exists(destFileName))
        return false;

//    CMDRecipeFile srcFile;
//    srcFile.setFileName(srcFileName);
//    srcFile.load();

//    CMDRecipeFile destFile(srcFile);
//    destFile.setFileName(destFileName);
//    destFile.save();
    if (!QFile::copy(srcFileName, destFileName)){
        return false;
    }
    return true;
}

bool CMDRecipeConfigerImp::exists(const QString& RecipeName)
{
    QString fileName = toFileName(RecipeName);
    return QFile::exists(fileName);
}

int CMDRecipeConfigerImp::save(const QString& recipeName, const QString& content)
{
    QString fileName = toFileName(recipeName);
    if(QFile::exists(fileName))
    {
        CMDRecipeFile saveFile;
        return saveFile.save(fileName, content);
    }
    return -1;
}

QString CMDRecipeConfigerImp::getContent(const QString& recipeName)
{
    QString fileName = toFileName(recipeName);
    if(QFile::exists(fileName))
    {
        CMDRecipeFile saveFile;
        return saveFile.getContent(fileName);
    }
    return "";
}

QString CMDRecipeConfigerImp::toFileName(const QString& RecipeName)
{
    QString rcpName = RecipeName;
    if(rcpName.endsWith(".rcp"))
        rcpName = rcpName.mid(0, rcpName.length()-4);
    QString fileName = recipePath() + rcpName + ".rcp";
    return fileName;
}

int CMDRecipeConfigerImp::getRecipeCount(bool onlyFile)
{
    m_recipeNameList.clear();

//    QDir dir(recipePath());
//    QStringList filters;
//    filters << "*.rep";
//    dir.setNameFilters(filters);
//    m_recipeNameList = dir.entryList(filters, QDir::Files|QDir::Dirs);
    QString rcpPath = recipePath();
    FindFile(rcpPath, m_recipeNameList, onlyFile);
    return m_recipeNameList.size();
}

QStringList CMDRecipeConfigerImp::enumRecipe(bool onlyFile)
{
    QStringList list;
    QString rcpPath = recipePath();
    FindFile(rcpPath, list, onlyFile);
    return list;
}

QString CMDRecipeConfigerImp::recipeName(int index)
{
    if(index < 0 || index >= m_recipeNameList.size())
        return "";

    return m_recipeNameList.at(index);
}

void CMDRecipeConfigerImp::test()
{
    QString file = toFileName("MyRecipe");
    CMDRecipeFile jsFile;
    jsFile.setFileName(file);
    jsFile.recipe()->configProperty()->head()->setComment("MindSCADA 配方");
    jsFile.recipe()->configProperty()->head()->setDownloadMode(DownloadMode::quick);
    jsFile.recipe()->configProperty()->items()->addItem("test.var1", "1");
    jsFile.recipe()->configProperty()->items()->addItem("test.str1", "和都不就");
    jsFile.recipe()->configProperty()->items()->addItem("test.date1", "2015-12-18");
    jsFile.save();

    CMDRecipeFile jsFile2;
    jsFile2.setFileName(file);
    jsFile2.load();

    CMDRecipeFile jsFile3;
    file = toFileName("MyRecipe3");
    jsFile3.setFileName(file);
    jsFile3 = jsFile2;
    jsFile3.save();
}

bool CMDRecipeConfigerImp::FindFile(const QString& path, QStringList& nameList, bool onlyFile)
{
    QDir dir(path);
    if (!dir.exists())
        return false;
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    while(i<list.size()) {
        QFileInfo fileInfo = list.at(i);
        bool bisDir=fileInfo.isDir();
        if(bisDir)
        {
            if(!onlyFile) {
                QString p = fileInfo.filePath();
                nameList << p.right(p.length()-recipePath().length());
            }
            FindFile(fileInfo.filePath(), nameList, onlyFile);
        }
        else
        {
            QString p = fileInfo.filePath();
            nameList << p.right(p.length()-recipePath().length());
        }
        i++;
    }
    return true;
}
