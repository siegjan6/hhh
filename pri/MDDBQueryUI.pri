INCLUDEPATH += ../../include/interface/Studio /
INCLUDEPATH += ../../include/MDMultiLanguage /
INCLUDEPATH += ../../include/MDDBQueryUI/

LibraryName = Mind.MDDataBase.MDDBQueryUI

!contains(DEFINES, DBQUERYUI_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
