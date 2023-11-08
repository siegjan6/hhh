
INCLUDEPATH +=../../include/MDSQLDataBaseProxy/

LibraryName = Mind.SQLDataBase.MDSQLDataBaseProxy
!contains(DEFINES, MDSQLDATABASEPROXY_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
