#include "MDHMICreator.h"
#include "HMIStudio.h"
#include "LaunchConfigWidget.h"

IHMIStudio *CMDHMICreator::createHMIStudio()
{
    return new CHMIStudio();
}
IHMIFramework *CMDHMICreator::createHMIFramework()
{
    return new CHMIFramework();
}
IHMIWidget *CMDHMICreator::createLaunchConfigWidget()
{
    return new CLaunchConfigWidget;
}
