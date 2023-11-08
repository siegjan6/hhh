

INCLUDEPATH += ../../include/MDDrawSpecial \

LibraryName = Mind.HMI.MDDrawSpecial

!contains(DEFINES, MDDRAWSPECIAL_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug -l$${LibraryName}d

}else{
LIBS += -L../../bin/release -l$${LibraryName}
}
}

win32{
    LIBS += -lUser32
}

unix{
    LIBS += -L/usr/local/lib -lxdo
#    PRE_TARGETDEPS += $$PWD/../libs/x32/libxdo.a
}
