TARGET = visiscript
TEMPLATE = lib
QT += gui script
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
#win32:RC_FILE = visiscript.rc

!android {
    QT += scripttools
}


FORMS = ReplaceDlgQt4.ui \
    FindDlgQt4.ui \
    ConfigureDlgQt4.ui \
    AboutDlgQt4.ui \
    mobilefileselection.ui \
    extensionsdialog.ui \
    selectfromlistdlg.ui
HEADERS = visiscript.h \
    configuredlgimpl.h \
    outputarea.h \
    codeedit.h \
    finddlgimpl.h \
    replacedlgimpl.h \
    aboutdlgimpl.h \
    qscintillamock.h \
    extensionsdialog.h \
    images/editdelete_png.h \
    images/play_png.h \
    images/new_png.h \
    images/forward_png.h \
    images/back_png.h \
    images/next_png.h \
    images/filesave_png.h \
    images/find_png.h \
    images/fileopen_png.h \
    ../include/inline_code.h \
    ../include/configure.h \
    ../include/extensions.h \
    qextscintillalexerlisp.h \
    selectfromlistdlg.h \
    qscilexerhaskell.h \
    MDVisiScript.h \
    ../../include/MDJavaScriptCodeEditor/MDJavaScriptCodeEditor.h \
    MDVisiScriptBaseForm.h
SOURCES = main.cpp \
    visiscript.cpp \
    configuredlgimpl.cpp \
    outputarea.cpp \
    codeedit.cpp \
    finddlgimpl.cpp \
    replacedlgimpl.cpp \
    aboutdlgimpl.cpp \
    qscintillamock.cpp \
    extensionsdialog.cpp \
    qextscintillalexerlisp.cpp \
    selectfromlistdlg.cpp \
    qscilexerhaskell.cpp \
    MDJavaScriptCodeEditor.cpp \
    MDVisiScript.cpp \
    MDJavaScriptCodeEditor2.cpp


TRANSLATIONS = visiscript_de.ts \
               visiscript_en.ts

INCLUDEPATH += ../include
INCLUDEPATH += ../QScintilla-gpl-2.8.4/qscintilla2/Qsci
INCLUDEPATH += ../QScintilla-gpl-2.8.4/qscintilla2


CONFIG(debug, debug|release){
    unix:LIBS += -L../../bin/debug/ -lqscintilla2d
    win32:LIBS += -L../../bin/debug/ -lqscintilla2d
}else{
    unix:LIBS += -L../../bin/release/ -lqscintilla2
    win32:LIBS += -L../../bin/release/ -lqscintilla2
}

#debug {
#     unix:LIBS += ../../bin/debug/libqscintilla2d.a
#    win32:LIBS += ../../bin/debug/libqscintilla2d.a
#    #CONFIG -= release
#}
#release {
#     unix:LIBS += ../../bin/release/libqscintilla2.a
#    win32:LIBS += ../../bin/release/libqscintilla2.a
#}

unix:!macx { 
    isEmpty( PREFIX ):PREFIX = /usr
    isEmpty( DESKTOPDIR ):DESKTOPDIR = /usr/share/applications
    isEmpty( ICONDIR ):ICONDIR = /usr/share/pixmaps
    DEFINES += PREFIX=\\\"$${PREFIX}\\\"
    target.path = $${PREFIX}/bin
    INSTALLS += target
    language.path = $${PREFIX}/share/visiscript
    language.files += visiscript_de.qm \
        visiscript_en.qm \
        qt_de.qm
    INSTALLS += language
    desktop.path = $${DESKTOPDIR}
    desktop.files = visiscript.desktop
    INSTALLS += desktop
    icon.path = $${ICONDIR}
    icon.files = visiscript_64x64.png
    INSTALLS += icon
}

RESOURCES += visiscript.qrc

CONFIG(debug, debug|release){
    SUBDIRTARGET = debug
}else{
    SUBDIRTARGET = release
}

#Release {
#    SUBDIRTARGET = release
#}
#Debug {
#    SUBDIRTARGET = debug
#}

############################################################
INCLUDEPATH += . ../include ../lexlib ../src\

DEFINES += MDJAVASCRIPTCODEEDITOR_LIBRARY

include(../../pri/MDJavaScriptCodeEditor.pri)

ProjectName = $${LibraryName}

include(../../pri/common.pri)
############################################################
