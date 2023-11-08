#include "Utility.h"

#include <QtCore/QCoreApplication>
#include <QCryptographicHash>
#include <iostream>
#include <QTime>
#include <QFile>
#include <QDir>
#include <QProcessEnvironment>
CUtility::CUtility()
{
}
QString CUtility::getCPUID()
{
    unsigned long s1,s2,s3,s4;
    char string[128];
    char szCpuId[1024];
    char p1[128], p2[128];
    unsigned int eax = 0;
    unsigned int ebx,ecx,edx;

    QString strCPUID;
    /*================================================*
    #if 1
    asm volatile
    (
    "cpuid"
    : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
    : "0"(0)
    );
    snprintf(szCpuId, 5, "%s", (char *)&ebx);
    //printf((char*)szCpuId);
    //printf("11\n");
    snprintf(szCpuId+4, 5, "%s", (char *)&edx);
    //printf((char*)szCpuId);
    //printf("22\n");
    snprintf(szCpuId+8, 5, "%s", (char *)&ecx);
    //printf((char*)szCpuId);
    //printf("33\n");
    #endif

    asm volatile
    (
    "movl $0x01 , %%eax ; \n\t"
    "xorl %%edx , %%edx ;\n\t"
    "cpuid ;\n\t"
    "movl %%edx ,%0 ;\n\t"
    "movl %%eax ,%1 ; \n\t"
    :"=m"(s1),"=m"(s2)
    );

    char cpuID[20];
    memset(cpuID,0,20);
    sprintf((char *)p1, "%08X%08X", s1, s2);
    snprintf(szCpuId+12, 20, "%s", (char *)p1);
    memcpy(cpuID,p1,17);
    strCPUID = strCPUID.fromLocal8Bit(cpuID);
    //printf("=========\n");
    //printf((char*)cpuID);
    //printf("=========\n");
    //printf((char*)szCpuId);
    //printf("44\n");
    asm volatile
    (
    "movl $0x03,%%eax ;\n\t"
    "xorl %%ecx,%%ecx ;\n\t"
    "xorl %%edx,%%edx ;\n\t"
    "cpuid ;\n\t"
    "movl %%edx,%0 ;\n\t"
    "movl %%ecx,%1 ;\n\t"
    :"=m"(s3),"=m"(s4)
    );

    sprintf((char *)p2, "%08X-%08X\n", s3, s4);
    snprintf(szCpuId+31, 19, "%s", (char *)p2);
    //printf((char*)szCpuId);
    //printf("55\n");
    //printf((char*)szCpuId);
    /*================================================*/
    return strCPUID;
}
QString CUtility::getMD5(const QString &strText)
{
    QString md5 = "";
    QByteArray bb;
    bb = QCryptographicHash::hash(strText.toLocal8Bit(),QCryptographicHash::Md5 );
    md5.append(bb.toHex());

    return md5;
}
/*================================================================*
【函數】:	CreateGuidByID()
【功能】:	根据一个ID生成一个GUID号
【参数】:	1. dwID : ID值
【返回】:	返回GUID
*================================================================*/
const char* CUtility::CreateGuidByID(unsigned long dwID)
{
    typedef struct _GUID
    {
        unsigned long Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char Data4[8];
    } GUID;

    GUID guid;
    unsigned long id = dwID;

    guid.Data1 = (unsigned long)(id << 8);
    guid.Data2 = (unsigned long)(id << 4);
    guid.Data3 = (unsigned long)id << 2;
    guid.Data4[0] = (unsigned char)(id << 8);
    guid.Data4[1] = (unsigned char)(id << 4);
    guid.Data4[2] = (unsigned char)(id >> 2);
    guid.Data4[3] = (unsigned char)(id << 1);
    guid.Data4[4] = (unsigned char)(id >> 8);
    guid.Data4[5] = (unsigned char)(id >> 4);
    guid.Data4[6] = (unsigned char)(id >> 2);
    guid.Data4[7] = id >> 1;

    static char buf[64] = {0};

    snprintf(buf, sizeof(buf)
        , "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"
        , guid.Data1
        , guid.Data2
        , guid.Data3
        , guid.Data4[0], guid.Data4[1]
        , guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
        , guid.Data4[6], guid.Data4[7]
        );

    return (const char*)buf;
}
const char* CUtility::CreateGuid()
{
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    int n = qrand();

    return CreateGuidByID(n);
}

const char* CUtility::getSysGuid()
{
    unsigned long dwData = 0;
    QString strMD5 = getMD5(getCPUID());
    for(int i = 0; i < strMD5.length();i ++)
    {
        char ch = strMD5.at(i).toLatin1();
        dwData += ch;
    }
    return CreateGuidByID(dwData);
}
void CUtility::StringFromVartype (QVariant::Type vtType, QString &strType)
{
    switch (vtType)
    {
    case QMetaType::Bool:           strType = "bool";					break;
    case QMetaType::UChar:          strType = "unsigned char";			break;
    case QMetaType::Char:           strType = "signed char";			break;
    case QMetaType::UShort:         strType = "unsigned short";			break;
    case QMetaType::Short:          strType = "2 byte signed int";		break;
    case QMetaType::ULong:          strType = "4 byte unsigned short";	break;
    case QMetaType::UInt:           strType = "4 byte unsigned short";	break;
    case QMetaType::Int:            strType = "4 byte signed int";		break;
    case QMetaType::Long:           strType = "4 byte signed int";		break;
    case QMetaType::LongLong:       strType = "8 byte signed int";      break;
    case QMetaType::ULongLong:      strType = "8 byte unsigned int";    break;
    case QMetaType::Float:          strType = "4 byte real";			break;
    case QMetaType::Double:         strType = "8 byte real";			break;
    case QMetaType::QString:        strType = "string";					break;
    case QMetaType::QDateTime:      strType = "date";					break;
    default:                        strType = "Native";					break;
    }
}

bool CUtility::saveToLog(const QString &strFile,const QString &strMsg,bool bClean)
{
    bool bSuccess = false;

    if(bClean)
    {
        QFile::remove(strFile);
    }
    QFile file(strFile);
    if (file.open(QIODevice::Append |QIODevice:: WriteOnly))
    {
        std::string str = strMsg.toStdString();
        file.write(str.c_str(),strlen(str.c_str()));
        file.close();
    }
    return bSuccess;
}

void CUtility::helper_Enum(const QString& absoluteDestPath, QStringList& myDirs)
{
    myDirs.append(absoluteDestPath);

    QDir dir(absoluteDestPath);
    foreach(QFileInfo mfi ,dir.entryInfoList())
    {
      if(mfi.isDir())
      {
          if(mfi.fileName()=="." || mfi.fileName() == "..")
              continue;

          myDirs.append(mfi.absoluteFilePath());
          helper_Enum(mfi.absoluteFilePath(), myDirs);
      }
    }
}

void CUtility::putDependPathToEnv(const QString& dependPath)
{
    QDir dir(dependPath);
    if(dir.exists())
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString curEnvPath = env.value("path");
        QStringList myDirs;

        QStringList ignorelist;
        QFile f(dir.absoluteFilePath(".pathignore"));
        if(f.open(QIODevice::ReadOnly))
        {
            QByteArray barr = f.readAll();
            ignorelist= QString::fromLocal8Bit(barr).split("\n", QString::SkipEmptyParts);
        }
        helper_Enum(dependPath, myDirs);
        foreach (QString d, myDirs)
        {
            //! 如果环境变量PATH中已经包含子路径，不重复添加
            if(curEnvPath.contains(d, Qt::CaseInsensitive) ||
                    curEnvPath.contains(QDir::toNativeSeparators(d), Qt::CaseInsensitive)
                    )
            {
                continue;
            }
            bool ignore = false;
            foreach (QString ignorepath, ignorelist) {
                if(QDir::toNativeSeparators(d).contains(QDir::toNativeSeparators(ignorepath.trimmed())))
                    ignore = true;
            }
            if(ignore)
                continue;
            curEnvPath +=QString(";%1").
                    arg(QDir::toNativeSeparators(d));
        }
        bool br = qputenv("path", curEnvPath.toLocal8Bit());
    }
}
