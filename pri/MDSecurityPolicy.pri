include(MDUtility.pri)

INCLUDEPATH += ../../include/MDSecurityPolicy \

LibraryName = Mind.Common.MDSecurityPolicy

!contains(DEFINES, SECURITYPOLICY_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
