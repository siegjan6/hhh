#include "WrapperHelper.h"

#ifdef Q_OS_LINUX

CWrapperHelper::CWrapperHelper()
{
}

char* CWrapperHelper::toChar(const QString& str)
{
    QByteArray ba = str.toLatin1();
    return ba.data();
}

/*!
\section V8 --> Qt
*/
bool CWrapperHelper::toQString(Local<Value> value, QString& str)
{
    if(!value->IsString())
        return false;

    Local<String> lstr = value->ToString();
    int length = lstr->Utf8Length();
    char* chValue = new char[length+1];
    lstr->WriteUtf8(chValue, length+1);
    str = QString(chValue);

    delete []chValue;
    return true;
}

bool CWrapperHelper::toInt(Local<Value> value, int& i)
{
    if(!value->IsNumber())
        return false;

    i = value->Int32Value();
    return true;
}

bool CWrapperHelper::toDouble(Local<Value> value, double& df)
{
    if(!value->IsNumber())
        return false;

    df = value->ToNumber()->Value();
    return true;
}

bool CWrapperHelper::toQDateTime(Local<Value> value, QDateTime& dt)
{
    if(!value->IsDate())
        return false;

    double df = value->ToNumber()->Value();
    dt = QDateTime::fromMSecsSinceEpoch(df);
    return true;
}

bool CWrapperHelper::toQVariant(Local<Value> value, QVariant& qv)
{
    bool result = true;
    if(value->IsNumber())
    {
        qv = QVariant(value->ToNumber()->Value());
    }
    else if(value->IsString())
    {
        QString str;
        toQString(value, str);
        qv = QVariant(str);
    }
    else if(value->IsDate())
    {
        QDateTime dt;
        toQDateTime(value, dt);
        qv = QVariant(dt);
    }
    else result = false;

    return result;
}

/*!
\section 其他
*/
Local<Value> CWrapperHelper::toValue(const QVariant& v)
{
    HandleScope handle_scope;
    Local<Value> p;
    switch (v.type())
    {
        case QMetaType::Bool :
        {
            p = BooleanObject::New(v.toBool());
            return p;
        }break;
        case QMetaType::Char :
        case QMetaType::UChar :
        case QMetaType::QChar:
        {
           p = Int32::New((int32_t)v.toInt());
            return p;
       }break;
        case QMetaType::Short :
        case QMetaType::UShort :
        case QMetaType::Int :
        case QMetaType::UInt :
        case QMetaType::Long :
        case QMetaType::ULong :
         {
            p = Int32::New((int32_t)v.toInt());
            return p;
        }break;
        case QMetaType::LongLong :
        case QMetaType::ULongLong :
        {
           p = Number::New((int64_t)v.toLongLong());
            return p;
       }break;
        case QMetaType::Float :
        case QMetaType::Double :
        {
           p = Number::New((int64_t)v.toDouble());
            return p;
       }break;
        case QMetaType::QString:
        {
           p = String::New(v.toString().toLatin1().data());
            return p;
       }break;
        case QMetaType::QDate:
        {
            p = Date::New(v.toDouble());
            return p;
        }break;
        default:
            break;
    }

    return Local<Value>();
}

void* CWrapperHelper::objectPtr(Local<Object> self)
{
    Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
    return wrap->Value();
}

#endif
