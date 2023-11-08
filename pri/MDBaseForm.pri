INCLUDEPATH += ../../include/MDBaseForm/ \
../../include/interface/Common/ \
../../include/interface/HMI/ \
../../include/MDRTDBManagerUI \
../../include/interface/Studio \

LibraryName = Mind.Common.BaseForm

!contains(DEFINES, BASEFORM_LIBRARY){

CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}
