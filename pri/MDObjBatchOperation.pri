
INCLUDEPATH += ../../include/MDObjBatchOperation \

LibraryName = Mind.Common.MDObjBatchOperation

!contains(DEFINES, MDOBJBATCHOPERATIONEXPERT_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
