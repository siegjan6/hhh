
INCLUDEPATH += ../../include/MDDataGroup/
INCLUDEPATH +=../../include/MDDataArchivesInterface/
INCLUDEPATH +=../../include/interface/Common/

LibraryName =  Mind.DataArchives.MDDataGroup

!contains(DEFINES, MDDATAGROUP_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
