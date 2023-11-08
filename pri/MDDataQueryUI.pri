include(MDFilterTreePlugin.pri)
include(MDMultiLanguage.pri)

INCLUDEPATH += ../../include/MDDataQueryUI/

LibraryName = Mind.Common.MDDataQueryUI

!contains(DEFINES, MDDATAQUERYUI_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
