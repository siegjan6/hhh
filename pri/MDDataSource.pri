INCLUDEPATH += ../../include/MDDataSource/
INCLUDEPATH +=../../include/MDDataArchivesInterface/
INCLUDEPATH +=../../include/interface/Common/

LibraryName =  Mind.DataArchives.MDDataSource

!contains(DEFINES, MDDATASOURCE_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
