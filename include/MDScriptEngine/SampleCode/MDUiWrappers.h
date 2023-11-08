#ifndef MDUIWRAPPERS_H
#define MDUIWRAPPERS_H

#include "../../include/V8/include/v8.h"
using namespace v8;
#include <QString>
#include <QVariant>

#include "../../include/MDScriptEngine/MDObjectWrapper.h"

class CMDWindow;
class CMDWindowWrapper : public CMDObjectWrapper
{
public:
    CMDWindowWrapper(void* object, const QString& name);
public:
    static Handle<Value> showMsg(const Arguments& args);
public:
    static CMDWindow* myPtr(Local<Object> self);
    void registerPropertyAndMethod(void* templ);
};

class CMDUser;
class CMDUserWrapper : public CMDObjectWrapper
{
public:
    CMDUserWrapper(void* object, const QString& name);
public:
    static Handle<Value> GetUID(Local<String> property, const AccessorInfo &info);
    static void SetUID(Local<String> property, Local<Value> value, const AccessorInfo& info);
public:
    static Handle<Value> doWork(const Arguments& args);
    static Handle<Value> showMsg(const Arguments& args);
public:
    static CMDUser* myPtr(Local<Object> self);
    void registerPropertyAndMethod(void* templ);
};

class CMDUserMgr;
class CMDUserMgrWrapper : public CMDObjectWrapper
{
public:
    CMDUserMgrWrapper(void* object, const QString& name);
public:
    static Handle<Value> login(const Arguments& args);
    static Handle<Value> logout(const Arguments& args);
public:
    static CMDUserMgr* myPtr(Local<Object> self);
    void registerPropertyAndMethod(void* templ);
};

#endif // MDUIWRAPPERS_H
