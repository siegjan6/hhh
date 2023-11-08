#include "MDRecipeFileData.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QCoreApplication>

CMDRecipeFileData::CMDRecipeFileData()
{
}

CMDRecipeFileData::~CMDRecipeFileData()
{
    m_recipe.configProperty()->items()->deleteAll();
}

bool CMDRecipeFileData::load(const QString& fileName, CMDRecipe& fileRecipe)
{
    QFile loadFile(fileName);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QFileInfo fi(fileName);
//    fileRecipe.configProperty()->head()->setName(fi.completeBaseName());
    fileRecipe.configProperty()->deserialize(loadDoc.object());

    return true;
}

bool CMDRecipeFileData::load()
{
    QFile loadFile(m_fileName);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QFileInfo fi(m_fileName);
    m_recipe.configProperty()->head()->setName(fi.completeBaseName());
    m_recipe.configProperty()->deserialize(loadDoc.object());

    return true;
}

bool CMDRecipeFileData::reload()
{
    unload();
    return load();
}

void CMDRecipeFileData::unload()
{
    m_recipe.configProperty()->items()->deleteAll();
}

void CMDRecipeFileData::save()
{
    QFile saveFile(m_fileName);
    saveFile.open(QIODevice::WriteOnly);

    QJsonObject jsRecipe;
    m_recipe.configProperty()->serialize(jsRecipe);
    QJsonDocument saveDoc(jsRecipe);
    saveFile.write(saveDoc.toJson());
    saveFile.close();
}

int CMDRecipeFileData::save(const QString& fileName, const QString& content)
{
    QFile rcpFile(fileName);
    if(rcpFile.open(QIODevice::ReadWrite)){
        QByteArray rcpData = rcpFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(rcpData));
        QJsonObject rcpJson = loadDoc.object();
        //QJsonArray rcpItems = rcpJson["RecipeItems"].toArray();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8(), &error);
        if(error.error == QJsonParseError::NoError){
            if(doc.isArray()){
                QJsonArray arr = doc.array();
                rcpJson["RecipeItems"] = arr;
                rcpFile.reset();
                rcpFile.resize(0);
                rcpFile.write(QJsonDocument(rcpJson).toJson());
            }
        }
        rcpFile.close();
    }
    return -1;
}


//"RecipeItems": [
//{
//   "ID": "virtual.pid_map.pid1.cfg_speed",
//   "Value": "100"
//},
//{
//   "ID": "virtual.pid_map.pid1.Kd",
//   "Value": "3"
//}]
QString CMDRecipeFileData::getContent(const QString& fileName)
{
    QFile loadFile(fileName);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData));

    QJsonObject obj = loadDoc.object();
    QJsonArray arr = obj["RecipeItems"].toArray();
    QJsonDocument doc;
    doc.setArray(arr);

    QString dataToString(doc.toJson(QJsonDocument::JsonFormat::Compact).constData());
    return dataToString;
}
