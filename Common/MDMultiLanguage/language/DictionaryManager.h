/*!
\author aGRw 2015.12.28
\section 字典管理类
*/
#ifndef DICTIONARYMANAGER_H
#define DICTIONARYMANAGER_H

#include <QObject>
#include <QVector>
#include <QStandardItemModel>
#include <QJsonObject>
#include "LanguageId.h"
#include "Dictionary.h"
#include "ILanguageObject.h"
#include "IDictionaryManager.h"

class CDictionaryManager : public QObject, public IDictionaryManager
{
    Q_OBJECT
public:
    CDictionaryManager(ILanguageObject* object, const QString& projectPath);
    virtual ~CDictionaryManager();
//!property
public:
    void setEnable(bool value);
    bool enable(){return m_enable;}
    void setDefaultSystemIndex(int value){m_defaultSystemIndex = value;}
    int defaultSystemIndex(){return m_defaultSystemIndex;}
    void setDefaultProjectIndex(int value){m_defaultProjectIndex = value;}
    int defaultProjectIndex(){return m_defaultProjectIndex;}
    int currentSystemIndex(){return m_currentSystemIndex;}
    int currentProjectIndex(){return m_currentProjectIndex;}
    QString systemName(){return m_systemDictName;}
    QString projectName(){return m_projectDictName;}
    QString systemPath(){return m_systemPath;}
    QVector<CLanguageId*>* languages(){return &m_languages;}
private:
    bool m_enable;//!是否启用
    qint32 m_defaultSystemIndex;//!默认系统语言
    qint32 m_defaultProjectIndex;//!默认项目语言
    qint32 m_currentSystemIndex;
    qint32 m_currentProjectIndex;
    QVector<CLanguageId*> m_languages;
    ILanguageObject* m_project;//!项目接口
/*!
\section const
*/
private:
    QString sOrig;
    int LanguageCount;

//!field
private:
    CDictionary m_systemDict;
    CDictionary m_projectDict;
    QString m_configName;//!配置文件路径
    QString m_systemDictName;//!系统字典全路径
    QString m_projectDictName;//!项目字典全路径
    QString m_projectPath;
    QString m_systemPath;
//!public function
public:
    bool addWord(const QString& value, bool isSystem = true);
    void deleteWord(const QString& value, bool isSystem = true);
    //!选择当前语言
    void setCurrentLanguage(int index, bool isSystem = true);
    int getCurrentLanguage(bool isSystem);
    QString value(const QString& key, bool isSystem = true);
    //!收集项目字典
    void collectLanguageWords();
    void resetCurrentIndex();
    void innerChangeLanuage();
private:
    void init();
    void initPath(const QString& projectPath);
    void insert(const QStringList& keys, const QStringList& values, int index, bool isSystem = true);
    CDictionary* dictionary(bool isSystem = true);
/*!
\section model
*/
public:
    void fillModel(QStandardItemModel& model, bool isSystem = true);
    void appendModel(QStandardItemModel& model, const QString& key, bool isSystem = true);
    void saveDataFromModel(QStandardItemModel& model, bool isSystem = true);
    QList<int> selectIndexs();
    QStringList selectLanguages();
/*!
\section serialize
*/
public:
    void save();
    void load();
    void saveConfig();
    void saveSystemDict();
    void savePorjectDict();
private:
    void loadConfig();
    void loadSystemDict();
    void loadProjectDict();
    void serialize(QDataStream& stream);
    void deserialize(QDataStream& stream);
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    void serializeHead(QJsonObject& json);
/*!
\section export & import
*/
public:
    void exportToFile(QStandardItemModel &model, const QString& fileName);
    void importFromFile(const QString& fileName, bool isSystem = true);
    void updateSystemDict();

};


#endif // DICTIONARYMANAGER_H



