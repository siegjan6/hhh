#ifndef MDUASERVER_GLOBAL_H
#define MDUASERVER_GLOBAL_H

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef MDUASERVER_STATIC
#    if defined(MDUASERVER_LIBRARY)
#      define MDUASERVER_EXPORT Q_DECL_EXPORT
#    else
#      define MDUASERVER_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define MDUASERVER_EXPORT
#endif

QT_END_NAMESPACE

#endif // MDUASERVER_GLOBAL_H
