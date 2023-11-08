#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T09:41:10
#
#-------------------------------------------------

QT       -= gui
QT += core
CONFIG += c++11
TEMPLATE = lib

DEFINES += MDEXPRESSION_LIBRARY

include(../../../pri/MDExpression.pri)

ProjectName = $${LibraryName}

include(../../../pri/common3.pri)

INCLUDEPATH += ../../../include/MDExpression \

SOURCES += \
    Core/ExpCompile.cpp \
    Core/FunctionBase.cpp \
    Core/Variant.cpp \
    Operator/AAnd.cpp \
    Operator/Add.cpp \
    Operator/AOr.cpp \
    Operator/Division.cpp \
    Operator/Multiplication.cpp \
    Operator/Not.cpp \
    Operator/StringConstantFun.cpp \
    Operator/Subtract.cpp \
    MDExpression.cpp \
    Function/Abs.cpp \
    Core/ExpUtil.cpp \
    Function/VariableFun.cpp \
    Core/ExpCompileMgr.cpp \
    Operator/LAnd.cpp \
    Operator/LOr.cpp \
    Operator/Eq.cpp \
    Operator/NEq.cpp \
    Operator/Greater.cpp \
    Operator/GTE.cpp \
    Operator/Less.cpp \
    Operator/LTE.cpp \
    Function/Format.cpp

HEADERS += \
    Interface/IEmpty.h \
    Interface/IFrequency.h \
    Interface/IOperator.h \
    Tool/CParameterCount.h \
    Core/ExpCompile.h \
    Core/FunctionBase.h \
    Core/Variant.h \
    Function/Abs.h \
    Interface/ParameterCount.h \
    Operator/AAnd.h \
    Operator/Add.h \
    Operator/AOr.h \
    Operator/Division.h \
    Operator/Multiplication.h \
    Operator/Not.h \
    Operator/StringConstantFun.h \
    Operator/Subtract.h \
    Interface/ExpVariable.h \
    Core/ExpUtil.h \
    Function/VariableFun.h \
    Core/ExpCompileMgr.h \
    Operator/LAnd.h \
    Operator/LOr.h \
    Operator/Eq.h \
    Operator/NEq.h \
    Operator/Greater.h \
    Operator/GTE.h \
    Operator/Less.h \
    Operator/LTE.h \
    ../../../include/MDExpression/MDExpression.h \
    Function/Format.h
