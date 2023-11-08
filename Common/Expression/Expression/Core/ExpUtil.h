#ifndef EXPUTIL_H
#define EXPUTIL_H
#include <qstring.h>
class CExpUtil
{
public:
    CExpUtil();
    static bool IsVarNameValidate(const QString& str);
};

#endif // EXPUTIL_H
