#include "ExpUtil.h"
#include "QRegExp"
CExpUtil::CExpUtil()
{
}

bool CExpUtil::IsVarNameValidate(const QString &str)
{
        try
        {
           // 保留字均返回false
           if (str == "true" || str == "false")
               return false;
           QRegExp regex("^[_a-zA-Z][_a-zA-Z0-9.]*$");
           return regex.exactMatch(str);
        }
        catch(...)
        {
           return false;
        }
}
