#ifndef UTILS_2016_11_09_H
#define UTILS_2016_11_09_H
#include <QJsonObject>

namespace DBQ
{
void SaveJosnFileHead(QJsonObject & jsonObj, int version, const QString & type);
bool WriteJsonObjectToFile(const QString & fullFileName, QJsonObject & root);
bool GetFileContentToByteArray(const QString & fullFileName, QByteArray & fileContentOut);
void WriteInfoLog(const QString& msg);
void WriteErrorLog(const QString& msg);
}

#endif // UTILS_2016_11_09_H
