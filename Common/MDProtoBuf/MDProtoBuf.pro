#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T14:25:04
#
#-------------------------------------------------

CONFIG += c++11
QT       -= gui
QT       +=network
QT       += widgets
QT       += xml

TARGET = MDProtoBuf
TEMPLATE = lib

DEFINES += MDPROTOBUF_LIBRARY

include(../../pri/MDProtoBuf.pri)
ProjectName = $${LibraryName}
include(../../pri/common.pri)

#DEFINES +=_MSC_VER
DEFINES +=PROTOBUF_USE_DLLS
DEFINES +=LIBPROTOBUF_EXPORTS


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lProtoBuf
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lProtoBuf
#else:unix: LIBS += -L$$PWD/ -lProtoBuf

#INCLUDEPATH += $$PWD/ProtocBuf

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/libProtoBuf.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/libProtoBuf.a
#else:unix: PRE_TARGETDEPS += $$PWD/libProtoBuf.a

SOURCES += MDProtoBuf.cpp

HEADERS += MDProtoBuf.h\
        mdprotobuf_global.h

HEADERS += google/protobuf/map_field.h
HEADERS += google/protobuf/any.h
HEADERS += google/protobuf/config.h
HEADERS += google/protobuf/arena.h
HEADERS += google/protobuf/arenastring.h
HEADERS += google/protobuf/stubs/int128.h
HEADERS += google/protobuf/stubs/atomicops_internals_x86_gcc.h
HEADERS += google/protobuf/stubs/status.h
HEADERS += google/protobuf/stubs/atomicops.h
HEADERS += google/protobuf/stubs/atomicops_internals_x86_msvc.h
HEADERS += google/protobuf/io/strtod.h
HEADERS += google/protobuf/io/coded_stream.h
HEADERS += google/protobuf/io/coded_stream_inl.h
HEADERS += google/protobuf/stubs/Logging.h
HEADERS += google/protobuf/descriptor.h
HEADERS += google/protobuf/descriptor.pb.h
HEADERS += google/protobuf/descriptor_database.h
HEADERS += google/protobuf/dynamic_message.h
HEADERS += google/protobuf/extension_set.h
HEADERS += google/protobuf/generated_message_reflection.h
HEADERS += google/protobuf/generated_message_util.h
HEADERS += google/protobuf/io/gzip_stream.h
HEADERS += google/protobuf/stubs/hash.h
HEADERS += google/protobuf/compiler/importer.h
HEADERS += google/protobuf/stubs/map-util.h
HEADERS += google/protobuf/message.h
HEADERS += google/protobuf/message_lite.h
HEADERS += google/protobuf/stubs/once.h
HEADERS += google/protobuf/compiler/parser.h
HEADERS += google/protobuf/stubs/platform_macros.h
HEADERS += google/protobuf/io/printer.h
HEADERS += google/protobuf/reflection_ops.h
HEADERS += google/protobuf/repeated_field.h
HEADERS += google/protobuf/service.h
HEADERS += google/protobuf/stubs/stringpiece.h
HEADERS += google/protobuf/stubs/stl_util.h
HEADERS += google/protobuf/stubs/common.h
HEADERS += google/protobuf/stubs/stringprintf.h
HEADERS += google/protobuf/stubs/strutil.h
HEADERS += google/protobuf/stubs/substitute.h
HEADERS += google/protobuf/stubs/template_util.h
HEADERS += google/protobuf/text_format.h
HEADERS += google/protobuf/io/tokenizer.h
HEADERS += google/protobuf/stubs/type_traits.h
HEADERS += google/protobuf/unknown_field_set.h
HEADERS += google/protobuf/wire_format.h
HEADERS += google/protobuf/wire_format_lite.h
HEADERS += google/protobuf/wire_format_lite_inl.h
HEADERS += google/protobuf/io/zero_copy_stream.h
HEADERS += google/protobuf/io/zero_copy_stream_impl.h
HEADERS += google/protobuf/io/zero_copy_stream_impl_lite.h
SOURCES += google/protobuf/stubs/atomicops_internals_x86_msvc.cc
SOURCES += google/protobuf/io/coded_stream.cc
SOURCES += google/protobuf/stubs/common.cc
SOURCES += google/protobuf/descriptor.cc
SOURCES += google/protobuf/descriptor.pb.cc
SOURCES += google/protobuf/descriptor_database.cc
SOURCES += google/protobuf/dynamic_message.cc
SOURCES += google/protobuf/extension_set.cc
SOURCES += google/protobuf/extension_set_heavy.cc
SOURCES += google/protobuf/generated_message_reflection.cc
SOURCES += google/protobuf/generated_message_util.cc
SOURCES += google/protobuf/io/gzip_stream.cc
SOURCES += google/protobuf/compiler/importer.cc
SOURCES += google/protobuf/message.cc
SOURCES += google/protobuf/message_lite.cc
SOURCES += google/protobuf/stubs/atomicops_internals_x86_gcc.cc
SOURCES += google/protobuf/stubs/status.cc
SOURCES += google/protobuf/stubs/once.cc
SOURCES += google/protobuf/compiler/parser.cc
SOURCES += google/protobuf/io/printer.cc
SOURCES += google/protobuf/io/strtod.cc
SOURCES += google/protobuf/reflection_ops.cc
SOURCES += google/protobuf/repeated_field.cc
SOURCES += google/protobuf/service.cc
SOURCES += google/protobuf/stubs/int128.cc
SOURCES += google/protobuf/stubs/stringprintf.cc
SOURCES += google/protobuf/stubs/stringpiece.cc
SOURCES += google/protobuf/stubs/structurally_valid.cc
SOURCES += google/protobuf/stubs/strutil.cc
SOURCES += google/protobuf/stubs/substitute.cc
SOURCES += google/protobuf/text_format.cc
SOURCES += google/protobuf/io/tokenizer.cc
SOURCES += google/protobuf/unknown_field_set.cc
SOURCES += google/protobuf/wire_format.cc
SOURCES += google/protobuf/wire_format_lite.cc
SOURCES += google/protobuf/io/zero_copy_stream.cc
SOURCES += google/protobuf/io/zero_copy_stream_impl.cc
SOURCES += google/protobuf/io/zero_copy_stream_impl_lite.cc
SOURCES += google/protobuf/any.cc
SOURCES += google/protobuf/arena.cc
SOURCES += google/protobuf/map_field.cc
SOURCES += google/protobuf/arenastring.cc
unix {
    target.path = /usr/lib
    INSTALLS += target
}

