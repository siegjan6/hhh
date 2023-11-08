#include <QDir>
#include "MDProjectConfigerImp.h"
#include "IMDDataArchives.h"
CMDProjectConfigerImp::CMDProjectConfigerImp()
{
    m_mdRTDBManager = CMDRTDBManagerFactory::createRTDBManager();
    m_mdDataArchives = CMDDataArchivesFactory::createDataArchives();

#   ifdef Q_OS_WIN
    m_projectPath = "c:/";
#   endif
#   ifdef Q_OS_LINUX
    m_projectPath = "/";
#   endif
}
CMDProjectConfigerImp::~CMDProjectConfigerImp()
{
    if(m_mdRTDBManager)
    {
        delete m_mdRTDBManager;
        m_mdRTDBManager = NULL;
    }
    if(m_mdDataArchives)
    {
         delete m_mdDataArchives;
         m_mdDataArchives = NULL;
    }
}

bool CMDProjectConfigerImp::parseProjectName(const QString& projectName, QString& path, QString& nameOnly)
{
    int pos = projectName.lastIndexOf("/");
    if(pos<=0)
    {
        return false;
    }
    path = projectName.left(pos);
    nameOnly = projectName.mid(pos+1);

    return true;
}

QString CMDProjectConfigerImp::fullFileName()
{
     return m_projectFullName;
}

// 名称
QString CMDProjectConfigerImp::name()
{
     return m_name;
}
void CMDProjectConfigerImp::setName(const QString &name)
{
    m_name= name ;
    if(m_name.right(5) ==".devx")
    {
        m_name = m_name.left(m_name.length()-5);
    }
}
// 目录
 QString CMDProjectConfigerImp::path()
 {
     return m_projectPath;
 }
 void CMDProjectConfigerImp::setPath(const QString &projectPath)
 {
      m_projectPath = projectPath ;
      if(m_projectPath.right(1)=="/")
      {
          m_projectPath = m_projectPath.left(m_projectPath.length()-1);
      }
      QDir *dir = new QDir;
      if(false == dir->exists(m_projectPath))
      {
          dir->mkdir(m_projectPath);
      }
      delete dir;

 }
// 描述
QString  CMDProjectConfigerImp::description()
{
    return m_description;
}
void CMDProjectConfigerImp:: setDescription(const QString &description)
{
     m_description = description ;
}

/*!
\section 管理项目总体参数
*/
bool CMDProjectConfigerImp::load(const QString& projectName)
{
    if(projectName.isEmpty()||projectName.isNull())
    {
        return false;
    }

    try
    {
        QFile file(projectName);
        if (file.open(QIODevice::ReadWrite))
        {
            QDataStream ar(&file);
            int uVersion = 1;
            ar >>uVersion;
            if(uVersion >= 1)
            {
                ar >> m_name;
                ar >> m_projectPath;
                ar >> m_description;
            }
            file.close();
        }
    }
    catch(...)
    {
    }

    QString pathOnly;
    QString nameOnly;
    if(!parseProjectName(projectName, pathOnly, nameOnly))
    {
        return false;
    }
    m_projectFullName = projectName;
    setPath(pathOnly);
    setName(nameOnly);

    m_paraFileName = m_projectPath + "/ProjectPara.dat";
    loadFromParaFile();

    m_eventConfiger.loadConfig(m_projectPath);
    m_actionConfiger.loadConfig(m_projectPath);
    m_securityConfiger.loadConfig(m_projectPath);
    m_alarmConfiger.loadConfig(m_projectPath);
    m_recipeConfiger.load(m_projectPath);

    if(NULL == m_mdDataArchives)
         m_mdDataArchives = CMDDataArchivesFactory::createDataArchives();
    if(NULL != m_mdDataArchives)
        m_mdDataArchives->load(m_projectPath);

    return true;
}

bool CMDProjectConfigerImp::unLoad()
{
    m_para = CMDProjectPara();

    m_eventConfiger.unloadConfig();
    m_actionConfiger.unloadConfig();
    m_securityConfiger.unloadConfig();
    m_alarmConfiger.unloadConfig();
    m_recipeConfiger.unload();

    if(NULL != m_mdDataArchives)
    {
         m_mdDataArchives->unLoad();
         delete m_mdDataArchives;
         m_mdDataArchives = NULL;
    }
    return true;
}
bool CMDProjectConfigerImp::save(const QString& projectName)
{
    bool bSeccess = false;
    try
    {
        QFile file(projectName);
        if (file.open(QIODevice::ReadWrite))
        {
            QDataStream ar(&file);
            int uVersion = 1;
            ar << uVersion;
            ar << m_name;
            ar << m_projectPath;
            ar << m_description;

            file.close();
            bSeccess = true;
        }
    }
    catch(...)
    {
        bSeccess = false;
    }

    return bSeccess;
}

bool CMDProjectConfigerImp::save()
{
    QString  projectName =m_projectPath+"/"+m_name+".devx";
    return save(projectName);
}

bool CMDProjectConfigerImp::loadFromParaFile()
{
    QFile configFile(m_paraFileName);
    if(!configFile.exists())
        return false;

    configFile.open(QIODevice::ReadOnly);
    QDataStream readDS(&configFile);
    m_para.deserialize(readDS);

    return true;
}

bool CMDProjectConfigerImp::saveToParaFile()
{
    QFile configFile(m_paraFileName);
    if(!configFile.exists())
    {
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    if(!configFile.exists())
        return false;

    configFile.open(QIODevice::WriteOnly);
    QDataStream writeDS(&configFile);
    m_para.serialize(writeDS);

    return true;
}

QString CMDProjectConfigerImp::paraFileName()
{
    return m_paraFileName;
}

CMDProjectPara* CMDProjectConfigerImp::projectPara()
{
    return &m_para;
}

void CMDProjectConfigerImp::setProjectPara(const CMDProjectPara* destConfig)
{
    m_para = *destConfig;
}

/*!
\section 实时数据库模块的初始化与释放
*/
void CMDProjectConfigerImp::loadRTDB()
{
    if(NULL == m_mdRTDBManager)
         m_mdRTDBManager = CMDRTDBManagerFactory::createRTDBManager();
    if(NULL != m_mdRTDBManager)
        m_mdRTDBManager->loadProject(m_projectPath);
}

void CMDProjectConfigerImp::unloadRTDB()
{
    if(NULL != m_mdRTDBManager)
    {
        m_mdRTDBManager->stop();
        m_mdRTDBManager->unLoadProject();
        delete m_mdRTDBManager;
        m_mdRTDBManager = NULL;
    }
}
