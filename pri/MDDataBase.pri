include(MDUtility.pri)
include(MDRTDBManager.pri)
#include(MDProject.pri)

include(MDDBConnect.pri)
include(MDLog.pri)

INCLUDEPATH += ../../include/MDDataBase \

LibraryName = Mind.MDDataBase.MDDBService

!contains(DEFINES, MDDATABASE_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
