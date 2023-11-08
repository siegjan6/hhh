/*
#ifndef GLOBALINTS_H
#define GLOBALINTS_H
#include <QString>
#include <QtCore/qglobal.h>

#if defined(FRAMEWORK_TESTOR)
#  define FRAMEWORK_TESTOR_EXPORT Q_DECL_EXPORT
#else
#  define FRAMEWORK_TESTOR_EXPORT Q_DECL_IMPORT
#endif

class IMDDataArchives;
class FRAMEWORK_TESTOR_EXPORT GlobalInst
{
public:
    static bool Init();
    static bool Reload(QString dirFullName);
    static IMDDataArchives * GetDataArchives();
};

#endif // GLOBALINT_H
*/
