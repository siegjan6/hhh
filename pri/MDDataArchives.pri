
INCLUDEPATH += ../../include/MDDataArchives/
INCLUDEPATH +=../../include/MDDataArchivesInterface/
INCLUDEPATH +=../../include/interface/Common/

LibraryName =  Mind.DataArchives.MDDataArchives

!contains(DEFINES, MDDATAARCHIVES_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
