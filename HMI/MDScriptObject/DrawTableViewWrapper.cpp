#include "DrawTableViewWrapper.h"
#include "MDDrawTableView.h"

//////////////////////////////////////////////////////////////////////////
CDrawTableViewWrapper::CDrawTableViewWrapper(void *object, const QString &name)
    :CMDObjectWrapper(object, name)
{
}
//////////////////////////////////////////////////////////////////////////
//Handle<Value> CDrawTableViewWrapper::rowCount(Local<String> property, const AccessorInfo &info)
//{
//    CMDDrawTableView* obj = myPtr(info.Holder());
//    return Number::New(obj->rowCount());
//}
//void CDrawTableViewWrapper::setRowCount(Local<String> property, Local<Value> value, const AccessorInfo &info)
//{
//    CMDDrawTableView* obj = myPtr(info.Holder());
//    obj->setRowCount(value->NumberValue());
//}
//Handle<Value> CDrawTableViewWrapper::colCount(Local<String> property, const AccessorInfo &info)
//{
//    CMDDrawTableView* obj = myPtr(info.Holder());
//    return Number::New(obj->colCount());
//}
//void CDrawTableViewWrapper::setColCount(Local<String> property, Local<Value> value, const AccessorInfo &info)
//{
//    CMDDrawTableView* obj = myPtr(info.Holder());
//    obj->setColCount(value->NumberValue());
//}
////////////////////////////////////////////////////////////////////////////
//Handle<Value> CDrawTableViewWrapper::setItemTypeRange(const Arguments &args)
//{
//    if (args.Length() != 5)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    int arg2 = 0;
//    int arg3 = 0;
//    int arg4 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toInt(args[2], arg2);
//    CWrapperHelper::toInt(args[3], arg3);
//    CWrapperHelper::toInt(args[4], arg4);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setItemTypeRange(arg0, arg1, arg2, arg3, arg4);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setItemType(const Arguments &args)
//{
//    if (args.Length() != 3)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    int arg2 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toInt(args[2], arg2);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setItemType(arg0, arg1, arg2);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::itemType(const Arguments &args)
//{
//    if (args.Length() != 2)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    return Integer::New(obj->itemType(arg0, arg1));
//}
//Handle<Value> CDrawTableViewWrapper::setItemValueRange(const Arguments &args)
//{
//    if (args.Length() != 5)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    int arg2 = 0;
//    int arg3 = 0;
//    QVariant arg4;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toInt(args[2], arg2);
//    CWrapperHelper::toInt(args[3], arg3);
//    CWrapperHelper::toQVariant(args[4], arg4);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setItemValueRange(arg0, arg1, arg2, arg3, arg4);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setItemValue(const Arguments &args)
//{
//    if (args.Length() != 3)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    QVariant arg2;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toQVariant(args[2], arg2);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setItemValue(arg0, arg1, arg2);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::itemValue(const Arguments &args)
//{
//    if (args.Length() != 2)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    QVariant v = obj->itemValue(arg0, arg1);
//    if (obj->itemType(arg0, arg1) == 0)//string
//        return String::New(v.toString().utf16());
//    else//double
//        return Number::New(v.toDouble());
//}
//Handle<Value> CDrawTableViewWrapper::setRowHeaderText(const Arguments &args)
//{
//    if (args.Length() != 2)
//        return v8::Undefined();

//    int arg0 = 0;
//    QString arg1 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toQString(args[1], arg1);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setRowHeaderText(arg0, arg1);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setColHeaderText(const Arguments &args)
//{
//    if (args.Length() != 2)
//        return v8::Undefined();

//    int arg0 = 0;
//    QString arg1 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toQString(args[1], arg1);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setColHeaderText(arg0, arg1);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setSingleRowHeight(const Arguments &args)
//{
//    if (args.Length() != 2)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setSingleRowHeight(arg0, arg1);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setSingleColWidth(const Arguments &args)
//{
//    if (args.Length() != 2)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setSingleColWidth(arg0, arg1);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setReadOnlyRowRange(const Arguments &args)
//{
//    if (args.Length() != 3)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    int arg2 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toInt(args[2], arg2);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setReadOnlyRowRange(arg0, arg1, arg2);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setReadOnlyColRange(const Arguments &args)
//{
//    if (args.Length() != 3)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    int arg2 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toInt(args[2], arg2);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setReadOnlyColRange(arg0, arg1, arg2);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setDecimalRowRange(const Arguments &args)
//{
//    if (args.Length() != 3)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    int arg2 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toInt(args[2], arg2);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setDecimalRowRange(arg0, arg1, arg2);
//    return v8::Undefined();
//}
//Handle<Value> CDrawTableViewWrapper::setDecimalColRange(const Arguments &args)
//{
//    if (args.Length() != 3)
//        return v8::Undefined();

//    int arg0 = 0;
//    int arg1 = 0;
//    int arg2 = 0;
//    CWrapperHelper::toInt(args[0], arg0);
//    CWrapperHelper::toInt(args[1], arg1);
//    CWrapperHelper::toInt(args[2], arg2);

//    CMDDrawTableView* obj = myPtr(args.Holder());
//    obj->setDecimalColRange(arg0, arg1, arg2);
//    return v8::Undefined();
//}






