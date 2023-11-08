win32:!win32-g++ {
	INCLUDEPATH += C:/opensource/MMKV/Win32/x64/Release/include
}
unix{
	INCLUDEPATH += /media/pi/A31C-5DF8/MMKV/Core
}


!contains(DEFINES, MDKVCLIENT_LIBRARY){
DEFINES += MDKVCLIENT_LIBRARY
#load static library
CONFIG(debug, debug|release){

    win32:!win32-g++ {
        contains(DEFINES, MDKVCLIENT_LIBRARY_MT){
            LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MT \
                -lmmkvd

            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MT/mmkvd.lib
        }

        contains(DEFINES, MDKVCLIENT_LIBRARY_MD){
            LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MD \
                -lmmkvd

            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MD/mmkvd.lib
        }
    }

    unix {
        VSCMD_ARG_TGT_ARCH=x32
        LIBS += -L$$PWD/../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmmkvd

        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmmkvd.a
    }

}else{

    win32:!win32-g++ {
        contains(DEFINES, MDKVCLIENT_LIBRARY_MT){
            LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MT \
                -lmmkv

            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MT/mmkv.lib
        }

        contains(DEFINES, MDKVCLIENT_LIBRARY_MD){
            LIBS += -L$$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MD \
                -lmmkv

            PRE_TARGETDEPS += $$PWD/../libs/$$(VSCMD_ARG_TGT_ARCH)/MD/mmkv.lib
        }

    #    message(path=$${LibraryName})
    }

    unix{
        VSCMD_ARG_TGT_ARCH=x32

        LIBS += -L$$PWD/../libs/$$VSCMD_ARG_TGT_ARCH \
            -lmmkv

        PRE_TARGETDEPS += $$PWD/../libs/$$VSCMD_ARG_TGT_ARCH/libmmkv.a
    }

}
}
