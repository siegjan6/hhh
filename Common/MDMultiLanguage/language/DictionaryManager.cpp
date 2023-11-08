#include <QFile>
#include <QObject>
#include <QDir>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QVariant>
#include "DictionaryManager.h"
#include "MDMultiLanguage.h"

//////////////////////////////////////////////////////////////
CDictionaryManager::CDictionaryManager(ILanguageObject *object, const QString &projectPath)
{
    sOrig = tr("原始字符串");
    LanguageCount = 26;

    m_project = object;
    initPath(projectPath);

    m_defaultSystemIndex = 20;
    m_defaultProjectIndex = 20;
    m_currentSystemIndex = 20;
    m_currentProjectIndex = 20;

    m_enable = false;

    m_languages.reserve(LanguageCount);
    m_languages << new CLanguageId(0x3801, tr("阿拉伯语"));
    m_languages << new CLanguageId(0x0415, tr("波兰语"));
    m_languages << new CLanguageId(0x0406, tr("丹麦语"));
    m_languages << new CLanguageId(0x0407, tr("德语"));
    m_languages << new CLanguageId(0x0419, tr("俄语"));
    m_languages << new CLanguageId(0x040C, tr("法语"));
    m_languages << new CLanguageId(0x040B, tr("芬兰语"));
    m_languages << new CLanguageId(0x0412, tr("朝鲜语"));
    m_languages << new CLanguageId(0x0413, tr("荷兰语"));
    m_languages << new CLanguageId(0x0405, tr("捷克语"));
    m_languages << new CLanguageId(0x0414, tr("挪威语"));
    m_languages << new CLanguageId(0x0816, tr("葡萄牙语(标准)"));
    m_languages << new CLanguageId(0x0416, tr("葡萄牙语(巴西)"));
    m_languages << new CLanguageId(0x0411, tr("日语"));
    m_languages << new CLanguageId(0x041F, tr("土耳其语"));
    m_languages << new CLanguageId(0x040A, tr("西班牙语"));
    m_languages << new CLanguageId(0x1037, tr("希伯来语"));
    m_languages << new CLanguageId(0x040E, tr("匈牙利语"));
    m_languages << new CLanguageId(0x0410, tr("意大利语"));
    m_languages << new CLanguageId(0x0809, tr("英语"));
    m_languages << new CLanguageId(0x0804, tr("中文(简体)"));
    m_languages << new CLanguageId(0x0404, tr("中文(繁体)"));
    m_languages << new CLanguageId(0x043E, tr("马来语"));
    m_languages << new CLanguageId(0x042A, tr("越南语"));
    m_languages << new CLanguageId(0x041E, tr("泰语"));
    m_languages << new CLanguageId(0xFFFF, tr("其他"));
    m_languages.at(20)->setIsSelect(true);

    load();
    init();
}
CDictionaryManager::~CDictionaryManager()
{
    qDeleteAll(m_languages.begin(), m_languages.end());
    m_languages.clear();
}
//////////////////////////////////////////////////////////////
void CDictionaryManager::setEnable(bool value)
{
    m_enable = value;
}
//////////////////////////////////////////////////////////////
bool CDictionaryManager::addWord(const QString &value, bool isSystem)
{
    return dictionary(isSystem)->addKey(value);
}
void CDictionaryManager::deleteWord(const QString &value, bool isSystem)
{
    dictionary(isSystem)->deleteKey(value);
}
void CDictionaryManager::setCurrentLanguage(int index, bool isSystem)
{
    if (isSystem)
        m_currentSystemIndex = index;
    else
        m_currentProjectIndex = index;
    dictionary(isSystem)->setCurrentDict(index);

    m_project->changeLanguage();
}
int CDictionaryManager::getCurrentLanguage(bool isSystem)
{
    if (isSystem)
        return m_currentSystemIndex;
    else
        return m_currentProjectIndex;
}
QString CDictionaryManager::value(const QString &key, bool isSystem)
{
    if (!enable())
        return key;

    return dictionary(isSystem)->value(key);
}
void CDictionaryManager::collectLanguageWords()
{
    m_project->collectWords();
}
void CDictionaryManager::resetCurrentIndex()
{
    QList<int> list = selectIndexs();
    if(list.count() == 0)
    {
        m_currentSystemIndex = -1;
        m_currentProjectIndex = -1;
    }
    else
    {
        if(list.indexOf(m_currentSystemIndex) == -1)
            m_currentSystemIndex = list.at(0);
        if(list.indexOf(m_currentProjectIndex) == -1)
            m_currentProjectIndex = list.at(0);
    }
    dictionary(true)->setCurrentDict(m_currentSystemIndex);
    dictionary(false)->setCurrentDict(m_currentProjectIndex);
}
void CDictionaryManager::innerChangeLanuage()
{
    m_project->changeLanguage();
}
//////////////////////////////////////////////////////////////
void CDictionaryManager::init()
{
    dictionary(true)->setCurrentDict(m_defaultSystemIndex);
    dictionary(false)->setCurrentDict(m_defaultProjectIndex);

    m_currentSystemIndex = m_defaultSystemIndex;
    m_currentProjectIndex = m_defaultProjectIndex;
}
void CDictionaryManager::initPath(const QString &projectPath)
{
    m_systemPath = QApplication::applicationDirPath();
    if (m_systemPath.at(m_systemPath.length() - 1) != '/')
        m_systemPath += "/";
    m_projectPath = projectPath;
    if (m_projectPath.at(m_projectPath.length() - 1) != '/')
        m_projectPath += "/";

    QString p = m_projectPath + "dict/";
    m_configName = p + "languageConfig";
    m_systemDictName = p + "system.dict";
    m_projectDictName = p + "project.dict";

    //dir & file
    QDir dir(p);
    if (!dir.exists())
        dir.mkdir(p);
    QFileInfo info(m_systemDictName);
    if(!info.exists())
        QFile::copy(m_systemPath+"dict/system.dict", m_systemDictName);
}
void CDictionaryManager::insert(const QStringList &keys, const QStringList &values, int index, bool isSystem)
{
    dictionary(isSystem)->reset(keys, values, index);
}
CDictionary *CDictionaryManager::dictionary(bool isSystem)
{
    if (isSystem)
        return &m_systemDict;

    return &m_projectDict;
}
//////////////////////////////////////////////////////////////
void CDictionaryManager::fillModel(QStandardItemModel &model, bool isSystem)
{
    model.clear();
    QList<int> indexs = selectIndexs();
    dictionary(isSystem)->fillModel(model, indexs);

    model.setHorizontalHeaderItem(0, new QStandardItem(CMDMultiLanguage::instance()->value(tr("添加时间"))));
    model.setHorizontalHeaderItem(1, new QStandardItem(CMDMultiLanguage::instance()->value(sOrig)));
    for(int i = 0; i < indexs.count(); i++)
    {
        QString name = CMDMultiLanguage::instance()->value(m_languages.at(indexs.at(i))->name());
        QStandardItem* item = new QStandardItem(name);
        item->setData((qulonglong)indexs.at(i));
        model.setHorizontalHeaderItem(i+2, item);
    }
}
void CDictionaryManager::appendModel(QStandardItemModel &model, const QString &key, bool isSystem)
{
    dictionary(isSystem)->appendModel(model, key);
}
void CDictionaryManager::saveDataFromModel(QStandardItemModel &model, bool isSystem)
{
    dictionary(isSystem)->saveDataFromModel(model);
}
QList<int> CDictionaryManager::selectIndexs()
{
    QList<int> selectIndexs;
    int count = m_languages.count();
    for(int i = 0; i < count; i++)
    {
        if (m_languages.at(i)->isSelect())
            selectIndexs.append(i);
    }

    return selectIndexs;
}
QStringList CDictionaryManager::selectLanguages()
{
    QStringList list;
    int count = m_languages.count();
    for(int i = 0; i < count; i++)
    {
        if (m_languages.at(i)->isSelect())
            list.append(m_languages.at(i)->name());
    }

    return list;
}
//////////////////////////////////////////////////////////////
void CDictionaryManager::save()
{
    saveSystemDict();
    savePorjectDict();
    saveConfig();
}
void CDictionaryManager::load()
{
    loadConfig();
    loadProjectDict();
    loadSystemDict();
}
void CDictionaryManager::saveConfig()
{
    QJsonObject json;
    serializeHead(json);
    serialize(json);
    QJsonDocument doc;
    doc.setObject(json);
    QByteArray ba = doc.toJson();

    QFile file(m_configName);
    if (!file.open(QIODevice::WriteOnly))
        return;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ba;
    //QDataStream stream(&file);
    //serialize(stream);

    file.close();
}
void CDictionaryManager::saveSystemDict()
{
    QJsonObject json;
    serializeHead(json);
    m_systemDict.serialize(json);
    QJsonDocument doc;
    doc.setObject(json);
    QJsonDocument::JsonFormat format = QJsonDocument::Compact;
#ifdef QT_DEBUG
    format = QJsonDocument::Indented;
#endif
    QByteArray ba = doc.toJson(/*format*/);

    QFile file(m_systemDictName);
    if (!file.open(QIODevice::WriteOnly))
        return;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ba;
    //QDataStream stream(&file);
    //m_systemDict.serialize(stream);
    file.close();
}
void CDictionaryManager::savePorjectDict()
{
    QJsonObject json;
    serializeHead(json);
    m_projectDict.serialize(json);
    QJsonDocument doc;
    doc.setObject(json);
    QJsonDocument::JsonFormat format = QJsonDocument::Compact;
#ifdef QT_DEBUG
    format = QJsonDocument::Indented;
#endif
    QByteArray ba = doc.toJson(/*format*/);

    QFile file(m_projectDictName);
    if (!file.open(QIODevice::WriteOnly))
        return;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ba;
    //QDataStream stream(&file);
    //m_projectDict.serialize(stream);
    file.close();
}
void CDictionaryManager::loadConfig()
{
    QFile file(m_configName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QByteArray ba;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    ba = stream.readAll().toUtf8();
    //QDataStream stream(&file);
    //m_systemDict.deserialize(stream);
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return;

    deserialize(doc.object());
}
void CDictionaryManager::loadSystemDict()
{
    QFile file(m_systemDictName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QByteArray ba;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    ba = stream.readAll().toUtf8();
    //QDataStream stream(&file);
    //m_systemDict.deserialize(stream);
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return;

    m_systemDict.deserialize(doc.object());
}
void CDictionaryManager::loadProjectDict()
{
    QFile file(m_projectDictName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QByteArray ba;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    ba = stream.readAll().toUtf8();
    //QDataStream stream(&file);
    //m_systemDict.deserialize(stream);
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return;

    m_projectDict.deserialize(doc.object());
}
void CDictionaryManager::serialize(QDataStream &stream)
{
    qint32 version = 1;
    stream << version;

    stream << m_enable;
    stream << m_defaultSystemIndex;
    stream << m_defaultProjectIndex;

    qint32 count = m_languages.count();
    stream << count;
    for(int i = 0; i < count; i++)
        stream << m_languages[i]->isSelect();
}
void CDictionaryManager::deserialize(QDataStream &stream)
{
    qint32 version = 0;
    stream >> version;

    stream >> m_enable;
    stream >> m_defaultSystemIndex;
    stream >> m_defaultProjectIndex;

    qint32 count = 0;
    stream >> count;
    for(int i = 0; i < count; i++)
    {
        bool isSelect = false;
        stream >> isSelect;

        if (i < m_languages.count())
            m_languages[i]->setIsSelect(isSelect);
    }
}
void CDictionaryManager::serialize(QJsonObject &json)
{
    json.insert("Enable", m_enable);
    json.insert("DefaultSystemIndex", m_defaultSystemIndex);
    json.insert("DefaultProjectIndex", m_defaultSystemIndex);
    QJsonArray array;
    foreach(CLanguageId* l, m_languages)
        array.append(l->isSelect());
    json.insert("SelectLanguages", array);
}
void CDictionaryManager::deserialize(const QJsonObject &json)
{
    m_enable = json.value("Enable").toBool();
    m_defaultSystemIndex = json.value("DefaultSystemIndex").toInt();
    m_defaultProjectIndex = json.value("DefaultProjectIndex").toInt();
    QJsonArray array = json.value("SelectLanguages").toArray();
    int count = array.count();
    if(count > m_languages.size())
        count = m_languages.size();
    for(int i = 0; i < count; i++)
        m_languages[i]->setIsSelect(array.at(i).toBool());
}
void CDictionaryManager::serializeHead(QJsonObject &json)
{
    QJsonObject js;
    js.insert("Version", 1);
    js.insert("Type", QString("Mind.Common.MDMultiLanguage"));

    json.insert("Head", js);
}
//////////////////////////////////////////////////////////////
void CDictionaryManager::exportToFile(QStandardItemModel &model, const QString &fileName)
{
    QStringList ls = selectLanguages();
    int colCount = ls.count();
    int rowCount = model.rowCount();

    QVariantList list;
    for(int row = 0; row < rowCount; row++)
    {
        QVariantMap map;
        map[sOrig] = model.item(row, 1)->text();
        for(int col = 0; col < colCount; col++)
        {
            QString value = model.item(row, col+2)->text();
            if(!value.isEmpty())
                map[ls.at(col)] = value;
        }

        list << map;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromVariant(list);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream stream(&file);
    stream << jsonDocument.toJson();
    file.close();
}
void CDictionaryManager::importFromFile(const QString &fileName, bool isSystem)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QTextStream stream(&file);
    QByteArray ba = stream.readAll().toUtf8();
    file.close();

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &error);
    if (error.error != QJsonParseError::NoError)
        return;

    QStringList languages;
    for(int i = 0; i < m_languages.count(); i++)
        languages.append(m_languages.at(i)->name());
    CDictionary* dict = dictionary(isSystem);

    QVariantList list = jsonDocument.toVariant().toList();
    int count = list.count();
    for(int i = 0; i < count; i++)
    {
        QVariantMap map = list.at(i).toMap();
        QString keyValue = map[sOrig].toString();
        if(!keyValue.isEmpty())
        {
            dict->addKey(keyValue);
            QStringList keys = map.keys();
            for(int j = 0; j < keys.count(); j++)
            {
                QString key = keys.at(j);
                int index = languages.indexOf(key);
                if(index == -1)
                    continue;
                dict->insert(keyValue, map[key].toString(), index);
            }
        }
    }
}
void CDictionaryManager::updateSystemDict()
{
    CDictionary dict;
    QFile file(m_systemPath+"dict/system.dict");
    if (!file.open(QIODevice::ReadOnly))
        return;
    //QDataStream stream(&file);
    //dict.deserialize(stream);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    QByteArray ba = stream.readAll().toUtf8();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return;
    dict.deserialize(doc.object());

    m_systemDict.updateFromDict(&dict);
}
//////////////////////////////////////////////////////////////


