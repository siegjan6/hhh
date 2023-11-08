/*!

\author enli

\section 功能

\通信对象创建类，由channel对象调用
*/

#include "MDCommunicationComponentFactory.h"
#include "MDCommunicationComponent.h"


ICommunicationComponent *CMDCommunicationComponentFactory::CreateCommunicationComponent()
{
    CMDCommunicationComponent *pCommunicationComponent=new CMDCommunicationComponent();
    return (ICommunicationComponent*)pCommunicationComponent;
}
