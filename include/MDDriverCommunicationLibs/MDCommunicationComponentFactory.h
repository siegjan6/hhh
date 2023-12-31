﻿#ifndef MDCOMMUNICATIONCOMPONENTFACTORY_H
#define MDCOMMUNICATIONCOMPONENTFACTORY_H


#include "ICommunicationComponent.h"

#if defined(MDDRIVERCOMMUNICATIONLIBS_LIBRARY)
#  define MDDRIVERCOMMUNICATIONLIBSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRIVERCOMMUNICATIONLIBSSHARED_EXPORT Q_DECL_IMPORT
#endif

class MDDRIVERCOMMUNICATIONLIBSSHARED_EXPORT CMDCommunicationComponentFactory
{
   public:
   static ICommunicationComponent *CreateCommunicationComponent();
};
#endif // MDCOMMUNICATIONCOMPONENTFACTORY_H
