#include "MDFactoryFile.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include "MDMObjFactory.h"

CMDFactoryFile::CMDFactoryFile()
{
}

CMDFactoryFile::~CMDFactoryFile()
{
    QMutexLocker locker(&m_mutex);
    m_groupMap.clear();
}

bool CMDFactoryFile::load(CMDMObjFactory* factory)
{
    QString fileName = prepareFileName(factory);
    if(fileName.isEmpty())
        return false;

    QMutexLocker locker(&m_mutex);
    m_groupMap.clear();
    locker.unlock();

    QFile loadFile(fileName);
    loadFile.open(QIODevice::ReadOnly);
    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject jsFactory = loadDoc.object();
    readFactory(jsFactory);

    return true;
}

bool CMDFactoryFile::save(CMDMObjFactory* factory)
{
    QString fileName = prepareFileName(factory);
    if(fileName.isEmpty())
        return false;

    QString path = preparePath(factory);
    preCreatePath(path);

    QFile saveFile(fileName);
    saveFile.open(QIODevice::WriteOnly);

    QJsonObject jsFactory;
    writeFactory(jsFactory, factory);

    QJsonDocument saveDoc(jsFactory);
    saveFile.write(saveDoc.toJson());
    return true;
}

QString CMDFactoryFile::prepareFileName(CMDMObjFactory* factory)
{
    QString path = preparePath(factory);
    if(path.isEmpty())
        return "";

    QString fileName = path + factory->para().type() + ".json";
    return fileName;
}

QString CMDFactoryFile::preparePath(CMDMObjFactory* factory)
{
    QString path = factory->para().pidToPath();
    if(path.isEmpty())
        return "";

    QString pathSymbol;
    #ifdef Q_OS_LINUX
        pathSymbol = "/";
    #else
        pathSymbol = "\\";
    #endif

    QString filePath = path;
    QString subPath = factory->subPathForFactoryFile();
    if(!subPath.isEmpty())
        filePath = path + subPath + pathSymbol;

    return filePath;
}

bool CMDFactoryFile::preCreatePath(const QString& path)
{
    QDir dir;
    return dir.mkpath(path);
}

void CMDFactoryFile::readFactory(QJsonObject &jsFactory)
{
    QString name = jsFactory["FactoryName"].toString();
    QString pid = jsFactory["Pid"].toString();
    QJsonArray groupArray = jsFactory["Groups"].toArray();
    for (int index = 0; index < groupArray.size(); ++index)
    {
        QJsonObject jsMObj = groupArray[index].toObject();
        QString group = jsMObj["group"].toString();
        QString status = jsMObj["value"].toString();

        QMutexLocker locker(&m_mutex);
        m_groupMap.insert(group, status);
    }
}

QString CMDFactoryFile::groupValue(const QString& group)
{
    QMutexLocker locker(&m_mutex);

    QMap<QString, QString>::iterator it = m_groupMap.find(group);
    if(it != m_groupMap.end())
        return it.value();
    else return "";
}

void CMDFactoryFile::writeFactory(QJsonObject &jsFactory, CMDMObjFactory* factory)
{
    jsFactory["FactoryName"] = factory->para().name();
    jsFactory["Pid"] = factory->para().pid();

    QJsonArray groupArray;
    QList<CMDMObj*> sList;
    factory->enumAll(sList);
    foreach (CMDMObj* mobj, sList)
    {
        QJsonObject jsMObj;
        jsMObj["group"] = mobj->para().group();
        jsMObj["value"] = mobj->groupValue();
        groupArray.append(jsMObj);
    }
    jsFactory["Groups"] = groupArray;
}
