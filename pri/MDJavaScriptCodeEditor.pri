
include(MDBaseForm.pri)

INCLUDEPATH += ../../include/MDJavaScriptCodeEditor \
LibraryName = Mind.Script.MDJAVAScriptCodeEditord
LibraryName = Mind.Script.MDJAVAScriptCodeEditor

!contains(DEFINES, MDJAVASCRIPTCODEEDITOR_LIBRARY){
#加载动态库
CONFIG(debug, debug|release){
LIBS += -L../../bin/debug/ -lMind.Script.MDJAVAScriptCodeEditord

}else{
LIBS += -L../../bin/release/ -lMind.Script.MDJAVAScriptCodeEditor
}
}
