
#include "SortJsonObject.h"

CSortJsonObject::CSortJsonObject()
{
    keysInOrder.clear();
}

CSortJsonObject::~CSortJsonObject()
{
    //!arrmap
    QMap<QString,CSortJsonArray*>::iterator it;
    CSortJsonArray* pArr = NULL;
    for ( it = arrMap.begin(); it != arrMap.end(); ++it )
    {
        pArr = it.value();
        if(pArr != NULL)
        {
            delete pArr;
            pArr = NULL;
        }
    }
    arrMap.clear();

    //!objectMap
    QMap<QString,CSortJsonObject*>::iterator ito;
    CSortJsonObject* pObj = NULL;
    for ( ito = objectMap.begin(); ito != objectMap.end(); ++ito )
    {
        pObj = ito.value();
        if(pObj != NULL)
        {
            delete pObj;
            pObj = NULL;
        }
    }
    objectMap.clear();

    keysInOrder.clear();
}
///////////////////////////////////////////////////////////
/// \brief CSortJsonObject::convertQJsonObject2String
/// \转换QJsonObj->QString
/// \param object
/// \return
QString CSortJsonObject::convertQJsonObject2String(QJsonObject& object)
{
    QString str = "";
    QJsonObject obj = object;
    CSortJsonObject jsonObj;
    CSortJsonObject* jsonObjpointer = jsonObj.convertQJsonObject2JsonObject(obj);
    if(jsonObjpointer != NULL)
    {
        str = jsonObjpointer->convertJsonObject2String(1);
        delete jsonObjpointer;
        jsonObjpointer = NULL;
    }
    return str;
}

CSortJsonObject* CSortJsonObject::convertQJsonObject2JsonObject(QJsonObject& object)
{
    QStringList keys = object.keys();
    int nCnt = keys.count();
    CSortJsonObject* jsObj = new CSortJsonObject();
    for(int i = 0; i<nCnt; i++)
    {
        QJsonValue val = object[keys.at(i)];

        switch(val.type())
        {
        case QJsonValue::Bool:
        case QJsonValue::Double:
        case QJsonValue::String:
            jsObj->insertSimpleValue(keys.at(i),val);
            break;
        case QJsonValue::Object:
        {
            QJsonObject obj = val.toObject();
            jsObj->insertObjectPointer(keys.at(i),convertQJsonObject2JsonObject(obj));
        }
            break;
        case QJsonValue::Array:
        {
            QJsonArray qjsonarr = val.toArray();
            CSortJsonArray* josnArr = converetQJsonArray2JsonArray(qjsonarr);
            if(josnArr)
                jsObj->insertArrayPointer(keys.at(i),josnArr);
        }
            break;
        case QJsonValue::Undefined:
            break;
        }
    }
    return jsObj;
}
CSortJsonArray* CSortJsonObject::converetQJsonArray2JsonArray(QJsonArray& qJsonArray)
{
    CSortJsonArray* jsonArr = NULL;
    int nArrCnt = qJsonArray.size();
    if(nArrCnt > 0)
    {
        jsonArr = new CSortJsonArray();
        for(int nArrIndex = 0; nArrIndex<nArrCnt; nArrIndex++)
        {
            switch(qJsonArray[nArrIndex].type())
            {
            case QJsonValue::Bool:
            case QJsonValue::Double:
            case QJsonValue::String:
                jsonArr->insertSimpleValue(qJsonArray[nArrIndex]);
                break;
            case QJsonValue::Object:
                {
                    QJsonObject qjsonObj = qJsonArray[nArrIndex].toObject();
                    CSortJsonObject* jsonobj = convertQJsonObject2JsonObject(qjsonObj);
                    if(jsonobj != NULL)
                        jsonArr->insertJsonObjPointer(jsonobj);
                }
                break;
            }//end switch
        }
    }
    return jsonArr;
}

void CSortJsonObject::insertObjectPointer(const QString& key, CSortJsonObject* jsonObj)
{
    keysInOrder.append(key);
    objectMap.insert(key,jsonObj);
}

void CSortJsonObject::insertArrayPointer(const QString& key, CSortJsonArray* jsonArr)
{
    keysInOrder.append(key);
    arrMap.insert(key,jsonArr);
}

void CSortJsonObject::insertSimpleValue(QString key,QJsonValue value)
{
    if(!keysInOrder.contains(key))
        keysInOrder.append(key);
    if(value.isString())
        qJsonObject.insert(key,value.toString());
    else
        qJsonObject.insert(key,value);
}

void CSortJsonObject::insertArray(QString key,CSortJsonArray& value)
{
    if(!keysInOrder.contains(key))
        keysInOrder.append(key);

    CSortJsonArray* arr = new CSortJsonArray();
    arr->copy(value);
    arrMap.insert(key,arr);

}
void CSortJsonObject::insertObject(QString key,CSortJsonObject& value)
{
    if(!keysInOrder.contains(key))
        keysInOrder.append(key);

    CSortJsonObject* pObj = new CSortJsonObject();
    pObj->copy(value);
    objectMap.insert(key,pObj);
}

void CSortJsonObject::copy(CSortJsonObject& object)
{
    //keys
    keysInOrder.clear();
    QStringList keys = object.getKeysInOrder();
    keysInOrder.append(keys);
    //qobject
    QStringList keysInJsonObject = object.getQjsonObject().keys();
    int nSimpleObjCnt = keysInJsonObject.size();
    for(int i = 0; i<nSimpleObjCnt; i++)
    {
        qJsonObject.insert(keysInJsonObject[i],object.getQjsonObject().value(keysInJsonObject[i]));
    }
    //arrMap
    QStringList keysInArrMap = object.getArrKeys();
    int nArrCnt = keysInArrMap.size();
    CSortJsonArray* pFromArr = NULL;
    for(int i = 0; i<nArrCnt; i++)
    {
        CSortJsonArray* pArr = new CSortJsonArray();
        pFromArr = object.getArrValue(keysInArrMap.at(i));
        if(pFromArr != NULL)
            pArr->copy(*pFromArr);
        arrMap.insert(keysInArrMap.at(i),pArr);
    }
    //objectMap
    QStringList keysInObjMap = object.getObjKeys();
    int nObjCnt = keysInObjMap.size();
    CSortJsonObject* pFromObj = NULL;
    for(int i = 0; i<nObjCnt; i++)
    {
        CSortJsonObject* pObj = new CSortJsonObject();
        pFromObj = object.getObjValue(keysInObjMap.at(i));
        if(pFromObj != NULL)
            pObj->copy(*pFromObj);
        objectMap.insert(keysInObjMap.at(i),pObj);
    }
}

//!get

QJsonObject CSortJsonObject::getQjsonObject()
{
    return qJsonObject;
}
QStringList CSortJsonObject::getKeysInOrder()
{
    return keysInOrder;
}
//arr
QStringList CSortJsonObject::getArrKeys()
{
    return arrMap.keys();
}
CSortJsonArray* CSortJsonObject::getArrValue(const QString& key)
{
    return arrMap[key];
}
//object
QStringList CSortJsonObject::getObjKeys()
{
    return objectMap.keys();
}
CSortJsonObject* CSortJsonObject::getObjValue(const QString& key)
{
    return objectMap[key];
}

void CSortJsonObject::appendKey(QString key, QString *out)
{
    out->append("\"");
    out->append(key);
    out->append("\"");
    out->append(":");
}/*

QString CSortJsonObject::convertQJsonObject2String(QJsonObject& object,int nLayer)
{
    //QStringList keys = object.keys();
    QString out;

    CSortJsonObject* theob = (CSortJsonObject*)&object;
    QStringList keys = theob->keysInOrder;

    for(int nTabIndex = 0; nTabIndex < nLayer; nTabIndex++)
    {
        out.append("\t");
    }
    for(int i = 0 ; i < keys.size() ; i++)
    {
        appendKey(keys[i],&out);

        out.append(convertQJsonValue2String(theob->qJsonObject[keys[i]],nLayer));
        if(i != keys.size() - 1)
            out.append(",");
        out.append("\n");

        //add layer
        if(i != keys.size() - 1)
        {
            for(int nTabIndex = 0; nTabIndex < nLayer; nTabIndex++)
            {
                out.append("\t");
            }
        }
    }
    for(int nTabIndex = 0; nTabIndex < nLayer-1; nTabIndex++)
    {
        out.prepend("\t");
        out.append("\t");
    }
    out.prepend("{\n");
    out.append("}");
    return out;
}*/

QString CSortJsonObject::convertJsonObject2String(int nLayer,bool bHasDoor)
{
    if(nLayer < 1)
        nLayer = 1;

    QString out;
    for(int nTabIndex = 0; nTabIndex < nLayer; nTabIndex++)
    {
        out.append("\t");
    }
    QString key;
    for(int i = 0 ; i < keysInOrder.size() ; i++)
    {
        key = keysInOrder[i];
        appendKey(key,&out);
        //检查是哪种值类型
        if(arrMap.contains(key))
        {
            out.append(convertJsonArray2String(arrMap[key],nLayer+1));
        }
        else if(objectMap.contains(key))
        {
            out.append("\n");
            out.append(objectMap[key]->convertJsonObject2String(nLayer+1));
        }
        else if(qJsonObject.contains(key))
        {
            out.append(convertQJsonValue2String(qJsonObject[key],nLayer+1));
        }

        if(i != keysInOrder.size() - 1)
            out.append(",");
        out.append("\n");

        //add layer
        if(i != keysInOrder.size() - 1)
        {
            for(int nTabIndex = 0; nTabIndex < nLayer; nTabIndex++)
            {
                out.append("\t");
            }
        }
    }

    for(int nTabIndex = 0; nTabIndex < nLayer-1; nTabIndex++)
    {
        //out.prepend("\t");
        out.append("\t");
    }
    if(bHasDoor)
    {
//        for(int nTabIndex = 0; nTabIndex < nLayer-1; nTabIndex++)
//        {
//            //out.prepend("\t");
//            out.append("\t");
//        }
        out.prepend("{\n");
        for(int nTabIndex = 0; nTabIndex < nLayer-1; nTabIndex++)
        {
            out.prepend("\t");
            //out.append("\t");
        }
        out.append("}");
    }
    return out;
}

QString CSortJsonObject::convertQJsonValue2String(QJsonValue value,int nLayer)
{
    switch(value.type())
    {
    case QJsonValue::Null:
        return QString("null");
        break;
    case QJsonValue::Bool:
        return QString("%1").arg(value.toBool());
        break;
    case QJsonValue::Double:
        return QString("%1").arg(value.toDouble());
        break;
    case QJsonValue::String:
    {
        QString retStr = value.toString();
        retStr = retStr.replace("\\","\\\\");
        retStr = retStr.replace("\"","\\\"");
        retStr = retStr.replace("\r\n","\\r\\n");
        retStr = retStr.replace("\n","\\n");
        retStr = retStr.replace("\t","\\t");
        return retStr.prepend("\"").append("\"");
    }
        break;
//    case QJsonValue::Object:
//        return convertQJsonObject2String(value.toObject(),nLayer);
//        break;
//    case QJsonValue::Array:
//        return convertQJsonArray2String(value.toArray(),nLayer+1);
//        break;
    case QJsonValue::Undefined:
        return QString();
        break;
    }
    return QString();
}

QString CSortJsonObject::convertArrayJsonObject2String(CSortJsonObject* object,int nLayer)
{
    QStringList keys = object->getKeysInOrder();
    QString out;
    out.append(object->convertJsonObject2String(nLayer+1,false));
    out.prepend("{\n");
    out.append("}");
    return out;
}

QString CSortJsonObject::convertJsonArray2String(CSortJsonArray* array,int nLayer)
{
    QString out;
    //add the [
    for(int nTabIndex = 0; nTabIndex < nLayer; nTabIndex++)
    {
        out.append("\t");
    }
    out.prepend("[\n");
    CSortJsonObject* pObj = NULL;
    for(int i = 0 ; i < array->getArraySize(true) ; i++)
    {
        pObj = array->getAt(i);
        if(pObj == NULL)
            continue;

        out.append(convertArrayJsonObject2String(pObj,nLayer));
        if(i != (array->getArraySize() - 1))
        {
            out.append(",");
            out.append("\n");
            //add layer
            for(int nTabIndex = 0; nTabIndex < nLayer; nTabIndex++)
            {
                out.append("\t");
            }
        }
    }

    for(int j = 0 ; j < array->getArraySize(false) ; j++)
    {
        out.append(convertQJsonValue2String(array->getSimpleArrayAt(j), nLayer));
        if(j != (array->getArraySize(false) - 1))
        {
            out.append(",");
            out.append("\n");
            //add layer
            for(int nTabIndex = 0; nTabIndex < nLayer; nTabIndex++)
            {
                out.append("\t");
            }
        }
    }
    out.append("\n");
    //add layer
    for(int nTabIndex = 0; nTabIndex < nLayer-1; nTabIndex++)
    {
        out.append("\t");
    }
    //add the ]

    out.append("]");
    //return out.prepend("[\n").append("]\n");
    return out;
}

//--------------------------------JSONArray----------------------------------
CSortJsonArray::CSortJsonArray()
{
    arrList.clear();
}
CSortJsonArray::~CSortJsonArray()
{
    clearList();
}

void CSortJsonArray::clearList()
{
    CSortJsonObject* pObj = NULL;
    for(int i = 0; i<arrList.count(); i++)
    {
        pObj = arrList[i];
        if(pObj != NULL)
        {
            delete pObj;
            pObj = NULL;
        }
    }
    arrList.clear();
}
void CSortJsonArray::insertSimpleValue(QJsonValue value)
{
    if(arrList.size() == 0)
        simpleArray.append(value);
}

void CSortJsonArray::insertObject(CSortJsonObject& value)
{
    if(simpleArray.size() == 0)
    {
        CSortJsonObject* pObj = new CSortJsonObject();
        pObj->copy(value);
        arrList.append(pObj);
    }
}

void CSortJsonArray::insertJsonObjPointer(CSortJsonObject* jsonObj)
{
    arrList.append(jsonObj);
}

int  CSortJsonArray::getArraySize(bool bObject)
{
    if(bObject)
        return arrList.count();
    else
        return simpleArray.count();
}

CSortJsonObject* CSortJsonArray::getAt(int nIndex)
{
    CSortJsonObject* pObj = NULL;
    if(nIndex < getArraySize())
    {
        pObj = arrList[nIndex];
    }
    return pObj;
}

QJsonValue CSortJsonArray::getSimpleArrayAt(int nIndex)
{
    return simpleArray[nIndex];
}

void CSortJsonArray::copy(CSortJsonArray& arr)
{
    CSortJsonObject* pFromObj = NULL;
    //!copy object array
    int nSize = arr.getArraySize();
    for(int i = 0; i<nSize; i++)
    {
        pFromObj = arr.getAt(i);
        CSortJsonObject* pObj = new CSortJsonObject();
        pObj->copy(*pFromObj);
        arrList.append(pObj);
    }

    //!copy simple array
    nSize = arr.getArraySize(false);
    for(int i = 0; i<nSize; i++)
    {
        QJsonValue val = arr.getSimpleArrayAt(i);
        simpleArray.append(val);
    }
}
