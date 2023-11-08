include(MDDataArchives.pri)
include(MDSwitchFactory.pri)
INCLUDEPATH +=../../include/MDDataArchivesEngine/
INCLUDEPATH +=../../include/MDDataArchivesInterface/
INCLUDEPATH +=../../include/interface/Common/

LibraryName =  Mind.DataArchives.MDDataArchivesEngine

!contains(DEFINES, MDDATAARCHIVESENGINE_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
