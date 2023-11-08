#ifndef UASINGLETON_H
#define UASINGLETON_H

#include "CSigleton.h"
#include "../../include/MDUaSingleton/MDUaSingleton_Global.h"
#include <QUaServer>

#define GetUaInstance() \
    SingletonBaseLazy::Instance<QUaServer>()

//class MDUASINGLETONSHARED_EXPORT QUaSingleton : public SingletonBaseLazy <QUaServer>
//{

//};

#endif // UASINGLETON_H
