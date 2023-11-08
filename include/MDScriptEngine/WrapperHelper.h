/*!

\author tangzw

\section 功能

脚本辅助类，主要实现数据类型的双向转换【V8 <--> Qt】。

*/

#ifndef WRAPPERHELPER_H
#define WRAPPERHELPER_H

#ifdef Q_OS_LINUX

#include "../../include/V8/include/v8.h"
using namespace v8;
#include <QString>
#include <QVariant>
#include <QDateTime>

class CWrapperHelper
{
public:
    CWrapperHelper();

/*!
\section V8 --> Qt
*/
public:
    static bool toQString(Local<Value> value, QString& str);
    static bool toInt(Local<Value> value, int& i);
    static bool toDouble(Local<Value> value, double& df);
    static bool toQDateTime(Local<Value> value, QDateTime& dt);
    static bool toQVariant(Local<Value> value, QVariant& qv);

/*!
\section 其他
*/
public:
    static char* toChar(const QString& str);
    static Local<Value> toValue(const QVariant& v);
public:
    static void* objectPtr(Local<Object> self);
};
#endif

#endif // WRAPPERHELPER_H
