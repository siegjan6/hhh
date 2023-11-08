/*!
\author aGRw 2015.04.07
\section 用于创建接口类
*/
#ifndef MDHMICREATORH
#define MDHMICREATORH

#include <QtCore/qglobal.h>

#if defined(MDHMIFRAMEWORK_LIBRARY)
#  define MDHMIFRAMEWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFRAMEWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "IHMIStudio.h"
#include "IHMIWidget.h"

class MDHMIFRAMEWORKSHARED_EXPORT CMDHMICreator
{
public:
    static IHMIStudio *createHMIStudio();
    static IHMIFramework* createHMIFramework();
    static IHMIWidget* createLaunchConfigWidget();
};

#endif // MDHMICREATORH
