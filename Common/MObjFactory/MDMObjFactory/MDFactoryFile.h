#ifndef MDFACTORYFILE_H
#define MDFACTORYFILE_H

#include <QJsonObject>
#include <QMutex>
#include <QMap>

class CMDMObjFactory;
class CMDFactoryFile
{
public:
    CMDFactoryFile();
    ~CMDFactoryFile();
public:
    bool load(CMDMObjFactory* factory);
    bool save(CMDMObjFactory* factory);
    QString prepareFileName(CMDMObjFactory* factory);
    QString preparePath(CMDMObjFactory* factory);
    bool preCreatePath(const QString& path);
public:
    void read(const QJsonObject &json);
    void readFactory(QJsonObject &jsFactory);
    void writeFactory(QJsonObject &jsFactory, CMDMObjFactory* factory);
public:
    QString groupValue(const QString& group);
protected:
    QMap<QString, QString> m_groupMap;
    QMutex m_mutex;
};

#endif // MDFACTORYFILE_H
