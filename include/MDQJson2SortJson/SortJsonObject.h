#ifndef JSONOBJECT_H
#define JSONOBJECT_H
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QJsonArray>
#include <QMap>
class CSortJsonArray;
#if defined(MDQJSON2SORTJSON_LIBRARY)
#  define MDQJSON2SORTJSON_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDQJSON2SORTJSON_SHARED_EXPORT Q_DECL_IMPORT
#endif

class MDQJSON2SORTJSON_SHARED_EXPORT CSortJsonObject
{
    friend class CSortJsonArray;
public:
    CSortJsonObject();
    virtual ~CSortJsonObject();
    /*!
    \section 转换QJson->Json
    */
    static QString convertQJsonObject2String(QJsonObject& object);
    //!对象用完，需调用方析构
    static CSortJsonObject* convertQJsonObject2JsonObject(QJsonObject& object);
    static CSortJsonArray* converetQJsonArray2JsonArray(QJsonArray& qJsonArray);

    /*!
    \section 插入对象到QObject
    */
    void insertSimpleValue(QString key,QJsonValue value);
    void insertArray(QString key,CSortJsonArray& value);
    void insertObject(QString key,CSortJsonObject& value);

    /*!
    \section 序列化
    \bHasDoor:数组元素不需要
    */
    QString convertJsonObject2String(int nLayer,bool bHasDoor = true);//bHasDoor = {}

private:
    /*!
    \section 获取对象属性，为copy服务
    */
    QJsonObject getQjsonObject();
    QStringList getKeysInOrder();
    //arr
    QStringList getArrKeys();
    CSortJsonArray* getArrValue(const QString& key);
    //object
    QStringList getObjKeys();
    CSortJsonObject* getObjValue(const QString& key);

    //!复制
    void copy(CSortJsonObject& object);

    //!序列化解析
    void appendKey(QString key,QString * out);
    QString convertQJsonValue2String(QJsonValue value,int nLayer);
    QString convertJsonArray2String(CSortJsonArray* array,int nLayer);
    //!转换数组中的object->QString
    QString convertArrayJsonObject2String(CSortJsonObject* object,int nLayer);

    //!call by QJsonObject->CSortJsonObject
    //only for QString convertQJsonObject2String(QJsonObject& object);
    void insertObjectPointer(const QString& key, CSortJsonObject* jsonObj);
    void insertArrayPointer(const QString& key, CSortJsonArray* jsonArr);


    //!按照插入顺序的key链表
    QStringList keysInOrder;

    //!每个JsonObject包含3种值
    QJsonObject qJsonObject;//duble,bool,string
    QMap<QString,CSortJsonArray*> arrMap;
    QMap<QString,CSortJsonObject*>objectMap;
};
///////////////////////////////////////////////////////////
/// \brief The JsonArray class
/// all simple value or all CSortJsonObject
///
class CSortJsonArray
{
    friend class CSortJsonObject;
public:
    CSortJsonArray();
    virtual ~CSortJsonArray();

public:

    //!CSortJsonObject
    void insertObject(CSortJsonObject& value);
    void insertJsonObjPointer(CSortJsonObject* jsonObj);
    CSortJsonObject* getAt(int nIndex);

    //!simple QJsonValue
    void insertSimpleValue(QJsonValue value);
    QJsonValue getSimpleArrayAt(int nIndex);

    int  getArraySize(bool bObject = true);
    void copy(CSortJsonArray& arr);

private:
    //!清除链表
    void clearList();
    QList<CSortJsonObject*> arrList;
    QJsonArray simpleArray;
};

#endif // JSONOBJECT_H
