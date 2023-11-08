#ifndef NSCOMMUNICATIONCOMPONENTFACTORY_H
#define NSCOMMUNICATIONCOMPONENTFACTORY_H
#include "communicationlibs_global.h"

#include "ICommunicationComponent.h"
#include "NSCommunicationComponent.h"

class COMMUNICATIONLIBSSHARED_EXPORT NSCommunicationComponentFactory
   {
   public:
       static ICommunicationComponent *CreateCommunicationComponent();
   private:
       static NSCommunicationComponent *m_pCommunicationComponent;

   };
#endif // NSCOMMUNICATIONCOMPONENTFACTORY_H
