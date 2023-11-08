#ifndef UASINGLETON_H
#define UASINGLETON_H

#include "CSigleton.h"
#include "../../include/MDUaSingleton/MDUaSingleton_Global.h"
#include <QUaServer>

#define GetUaInstance() \
    SingletonBaseLazy<QUaServer>::Instance()

//class MDUASINGLETONSHARED_EXPORT QUaSingleton : SingletonBaseLazy<QUaServer>
//{

//};

#endif // UASINGLETON_H
