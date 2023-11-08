include(MDMultiLanguage.pri)

INCLUDEPATH += ../../include/MDDataArchivesUI/ \

LibraryName =  Mind.DataArchives.MDDataArchivesUI

!contains(DEFINES, MDDATAARCHIVESUI_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
