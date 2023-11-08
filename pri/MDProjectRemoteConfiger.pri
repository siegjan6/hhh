INCLUDEPATH +=../../include/MDProjectRemoteConfiger/

LibraryName = Mind.Project.MDProjectRemoteConfiger
!contains(DEFINES, MDPROJECTREMOTECONFIGER_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
