INCLUDEPATH += ../../include/MDProjectExpert
INCLUDEPATH += ../../include/MDAlarmExpert
INCLUDEPATH += ../../include/MDRTDBManager/
INCLUDEPATH +=../../include/MDRTDBInterfaces/
INCLUDEPATH +=../../include/MDDriverInterfaces/
INCLUDEPATH +=../../include/interface/Common/

LibraryName = Mind.MDUaExpert

!contains(DEFINES, MDUAEXPERT_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
