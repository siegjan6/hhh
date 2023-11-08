#include "MDUiWrappers.h"
#include "WrapperHelper.h"
#include "MDWindow.h"
#include "MDUser.h"

CMDWindowWrapper::CMDWindowWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

Handle<Value> CMDWindowWrapper::showMsg(const Arguments& args)
{
    int paraCount = args.Length();
    if(paraCount < 1)
        return v8::Undefined();

    QString msg;
    if(!CWrapperHelper::toQString(args[0], msg))
         return v8::Undefined();

    //调用类方法
    CMDWindow* obj = myPtr(args.Holder());
    obj->showMsg(msg);
    return v8::Undefined();
}

CMDWindow* CMDWindowWrapper::myPtr(Local<Object> self)
{
    return static_cast<CMDWindow*>(CWrapperHelper::objectPtr(self));
}

void CMDWindowWrapper::registerPropertyAndMethod(void* templ)
{
    if(!templ)
        return;

    Handle<ObjectTemplate>& templHandle = *((Handle<ObjectTemplate>*)templ);

    templHandle->Set("showMsg",FunctionTemplate::New(showMsg));
}

///
///
///
CMDUserWrapper::CMDUserWrapper(void* object, const QString& name)
                               : CMDObjectWrapper(object, name)
{

}

Handle<Value> CMDUserWrapper::GetUID(Local<String> property,
    const AccessorInfo &info)
{
    CMDUser* obj = myPtr(info.Holder());
    return Integer::New(obj->m_uid);
}

void CMDUserWrapper::SetUID(Local<String> property, Local<Value> value,
    const AccessorInfo& info)
{
    CMDUser* obj = myPtr(info.Holder());
    obj->m_uid = (value->Int32Value());
}

Handle<Value> CMDUserWrapper::doWork(const Arguments& args)
{
    int paraCount = args.Length();
    if(paraCount < 2)
        return v8::Undefined();

    QString name;
    int repeat;
    if(!CWrapperHelper::toQString(args[0], name))
        return v8::Undefined();
    if(!CWrapperHelper::toInt(args[1], repeat))
         return v8::Undefined();

    //调用类方法
    CMDUser* obj = myPtr(args.Holder());
    QString result = obj->doWork(name, repeat);
    return String::New(CWrapperHelper::toChar(result));
}

Handle<Value> CMDUserWrapper::showMsg(const Arguments& args)
{
    int paraCount = args.Length();
    if(paraCount < 1)
        return v8::Undefined();

    QString msg;
    if(!CWrapperHelper::toQString(args[0], msg))
         return v8::Undefined();

    //调用类方法
    CMDUser* obj = myPtr(args.Holder());
    //obj->showMsg(msg);
    return v8::Undefined();
}

CMDUser* CMDUserWrapper::myPtr(Local<Object> self)
{
   return static_cast<CMDUser*>(CWrapperHelper::objectPtr(self));
}

void CMDUserWrapper::registerPropertyAndMethod(void* templ)
{
    if(!templ)
        return;

    Handle<ObjectTemplate>& templHandle = *((Handle<ObjectTemplate>*)templ);

    templHandle->SetAccessor(String::New("uid"), GetUID, SetUID);

    templHandle->Set("doWork",FunctionTemplate::New(doWork));
    templHandle->Set("showMsg",FunctionTemplate::New(showMsg));
}

///
///
/// /

CMDUserMgrWrapper::CMDUserMgrWrapper(void* object, const QString& name)
                                      :CMDObjectWrapper(object, name)
{

}

Handle<Value> CMDUserMgrWrapper::login(const Arguments& args)
{
    int paraCount = args.Length();
    if(paraCount < 1)
        return v8::Undefined();

    QString name;
    if(!CWrapperHelper::toQString(args[0], name))
         return v8::Undefined();

    //调用类方法
    CMDUserMgr* obj = myPtr(args.Holder());
    obj->login(name);
    return v8::Undefined();
}

Handle<Value> CMDUserMgrWrapper::logout(const Arguments& args)
{
    //调用类方法
    CMDUserMgr* obj = myPtr(args.Holder());
    obj->logout();
    return v8::Undefined();
}

CMDUserMgr* CMDUserMgrWrapper::myPtr(Local<Object> self)
{
    return static_cast<CMDUserMgr*>(CWrapperHelper::objectPtr(self));
}

void CMDUserMgrWrapper::registerPropertyAndMethod(void* templ)
{
    if(!templ)
        return;

    Handle<ObjectTemplate>& templHandle = *((Handle<ObjectTemplate>*)templ);

    templHandle->Set("login",FunctionTemplate::New(login));
    templHandle->Set("logout",FunctionTemplate::New(logout));
}
