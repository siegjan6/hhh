#ifndef UTILITY_H
#define UTILITY_H

#include <QtCore/qglobal.h>
#include <QVariant>

#if defined(UTILITY_LIBRARY)
#  define UTILITYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UTILITYSHARED_EXPORT Q_DECL_IMPORT
#endif

class UTILITYSHARED_EXPORT CUtility
{

private:
    CUtility();

public:
    /*================================================================*
    【函數】:	getCPUID()
    【功能】:	获取CPUID
    【参数】:
    【返回】:	返回CPUID字符串
    *================================================================*/
    static QString getCPUID();

    /*================================================================*
    【函數】:	getMD5()
    【功能】:	算strText的MD5值
    【参数】:	strText : 待算MD5的字符串
    【返回】:	返回MD5值
    *================================================================*/
    static QString getMD5(const QString &strText);

    /*================================================================*
    【函數】:	CreateGuidByID()
    【功能】:	根据一个ID生成一个GUID号
    【参数】:	1. dwID : ID值
    【返回】:	返回GUID
    *================================================================*/
    static const char* CreateGuidByID(unsigned long dwID);

    /*================================================================*
    【函數】:	CreateGuid()
    【功能】:	随机生成一个GUID号
    【参数】:	无
    【返回】:	返回GUID
    *================================================================*/
    static const char* CreateGuid();

    /*================================================================*
    【函數】:	getSysGuid()
    【功能】:	获取系统GUID
    【参数】:	无
    【返回】:	返回GUID
    *================================================================*/
    static const char* getSysGuid();

    static void StringFromVartype (QVariant::Type vtType, QString &strType);

    static bool saveToLog(const QString &strFile,const QString &strMsg,bool bClean = false);

    /*================================================================*
     * 將依賴路徑dependPath加入系統環境變量path前
     * *================================================================*/
    static void helper_Enum(const QString& absoluteDestPath, QStringList& myDirs);

    static void putDependPathToEnv(const QString& dependPath);

};

#endif // UTILITY_H
