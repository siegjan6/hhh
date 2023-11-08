#include "MDDBQueryConfigure.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <MDSwitchFactory/MDLogSwitchFactory.h>
#include <QCoreApplication>

namespace DBQ
{
void SaveJosnFileHead(QJsonObject & jsonObj, int version, const QString & type)
{
    QJsonObject headObj;
    headObj.insert("Version", version);
    headObj.insert("Type", type);
    jsonObj.insert("Head", headObj);
}

bool GetFileContentToByteArray(const QString & fullFileName, QByteArray & fileContentOut)
{
    QFile file(fullFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        file.close();
        return false;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    fileContentOut = stream.readAll().toUtf8();
    file.close();

    return true;
}

bool WriteJsonObjectToFile(const QString & fullFileName, QJsonObject & root)
{
    QFile file(fullFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        file.close();
        return false;
    }

    QJsonDocument doc;
    doc.setObject(root);
    QTextStream stream(&file);
    stream.setCodec("UTF-8"); //读入的文件需为UTF-8编码
    stream<<doc.toJson();
    file.close();

    return true;
}

static CMDLogSwitch * pLogSwitch = 0;

void WriteInfoLog(const QString& msg)
{
    if(!pLogSwitch)
    {
        QString runPath = QCoreApplication::applicationDirPath();
        //pLogSwitch = CMDLogSwitchFactory::createSwitch(runPath, "DBQureyConfigure", "");
        pLogSwitch = CMDLogSwitchFactory::createSwitchWithSubFileDir(runPath, "DBQureyConfigure", "", "");
        pLogSwitch->turnOn();
    }

    pLogSwitch->LOG_INFO(msg);
}

void WriteErrorLog(const QString& msg)
{
    if(!pLogSwitch)
    {
        QString runPath = QCoreApplication::applicationDirPath();
        pLogSwitch = CMDLogSwitchFactory::createSwitchWithSubFileDir(runPath, "DBQureyConfigure", "", "");
        //pLogSwitch = CMDLogSwitchFactory::createSwitch(runPath, "DBQureyConfigure", "");
        pLogSwitch->turnOn();
    }

    pLogSwitch->LOG_ERROR(msg);
}

#define STATUS_EMPTY                        0
#define STATUS_START_EXTRACT      1  //{ or [%
#define STATUS_END_EXTRACT          2  //} or %]

//可重用函数 {}, [%%]
void ExtractTokensWithBound1(QString & str, char leftBound, char rightBound, QStringList & tokens)
{
    int status = STATUS_EMPTY;
    QString token;
    int len = str.length();

    for(int i=0; i<len; i++)
    {
        //<状态分割代码>
        if(str[i] == leftBound)
        {
            status = STATUS_START_EXTRACT;
            i++;
            if(i>=len)
                return ;

            token.clear();
        }
        else if(str[i] == rightBound)
            status = STATUS_END_EXTRACT;
        //</状态分割代码>

        if(STATUS_START_EXTRACT == status) //提取状态
            token.push_back(str[i]);
        else if(STATUS_END_EXTRACT == status) //提取完成
        {
            if(!token.isEmpty())
                tokens.push_back(token);

            token.clear();
            status = STATUS_EMPTY;

        }
        //else //不需要此分支
    }//end for
}

DBQUERYCONFIGURE_EXPORT bool ExtractTokensWithBound2(QString & str, const char * leftBound, const char * rightBound, QStringList & tokens)
{
    if(!leftBound || !rightBound)
        return false;

    if(!(strlen(leftBound) == 2 && strlen(rightBound) == 2))
        return false;

    int status = STATUS_EMPTY;
    QString token;
    int len = str.length();
    len--;//str[i]要向后访问一个字符 防止越界

    for(int i=0, j=1; i<len; i++, j++)
    {
        //<状态分割代码>
        if(str[i] == leftBound[0] && str[j] == leftBound[1])
        {
            status = STATUS_START_EXTRACT;
            i++;i++;j++;j++; //跳过边界字符串
            if(j>=len) //字符串结束了 j代表最后一个字符的索引
                return true;
            token.clear();
        }
        else if(str[i] == rightBound[0] && str[j] == rightBound[1])
        {
            status = STATUS_END_EXTRACT;
        }
        //</状态分割代码>

        if(STATUS_START_EXTRACT == status) //提取状态
            token.push_back(str[i]);
        else if(STATUS_END_EXTRACT == status) //提取完成
        {
            if(!token.isEmpty()) //可能str只有rightBound 例如 [%%].只有%],此时token为空
            {
                tokens.push_back(token);
            }
            token.clear();
            i++;j++; //跳过rightBound
            status = STATUS_EMPTY;
        }
        //else //不需要此分支
    }//end for

    return true;
}
}
