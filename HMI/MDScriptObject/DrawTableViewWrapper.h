//by aGRw 2015.12.20
#ifndef MDDRAWTABLEVIEWWRAPPER_H
#define MDDRAWTABLEVIEWWRAPPER_H

#include "MDDrawObjWrapper.h"

class CMDDrawTableView;

class CDrawTableViewWrapper : public CMDObjectWrapper
{
    Q_OBJECT
public:
    CDrawTableViewWrapper(void* object, const QString& name);
    virtual ~CDrawTableViewWrapper(){}
//////////////////////////////////////////////////////////////////////////
public:
//    static Handle<Value> rowCount(Local<String> property, const AccessorInfo &info);
//    static void setRowCount(Local<String> property, Local<Value> value, const AccessorInfo& info);
//    static Handle<Value> colCount(Local<String> property, const AccessorInfo &info);
//    static void setColCount(Local<String> property, Local<Value> value, const AccessorInfo& info);

//    static Handle<Value> setItemTypeRange(const Arguments& args);
//    static Handle<Value> setItemType(const Arguments& args);
//    static Handle<Value> itemType(const Arguments& args);
//    static Handle<Value> setItemValueRange(const Arguments& args);
//    static Handle<Value> setItemValue(const Arguments& args);
//    static Handle<Value> itemValue(const Arguments& args);
//    static Handle<Value> setRowHeaderText(const Arguments& args);
//    static Handle<Value> setColHeaderText(const Arguments& args);
//    static Handle<Value> setSingleRowHeight(const Arguments& args);
//    static Handle<Value> setSingleColWidth(const Arguments& args);
//    static Handle<Value> setReadOnlyRowRange(const Arguments& args);
//    static Handle<Value> setReadOnlyColRange(const Arguments& args);
//    static Handle<Value> setDecimalRowRange(const Arguments& args);
//    static Handle<Value> setDecimalColRange(const Arguments& args);

};

#endif // MDDRAWTABLEVIEWWRAPPER_H








