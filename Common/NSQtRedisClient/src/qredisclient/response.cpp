#include "response.h"
#include <QDebug>
#include <QVector>
#include <QObject>
#include <QVariantList>
#include "qredisclient/utils/text.h"
#include "qredisclient/utils/compat.h"

const redisReplyObjectFunctions RedisClient::Response::defaultFunctions = {
    RedisClient::Response::createStringObject,
    RedisClient::Response::createArrayObject,
    RedisClient::Response::createIntegerObject,
    RedisClient::Response::createNilObject,
    RedisClient::Response::freeObject
};

RedisClient::Response::Response()
    : m_responseSource(""), 
      m_redisReader(QSharedPointer<redisReader>(redisReaderCreate(), redisReaderFree)),
      m_result(nullptr)
{

}

RedisClient::Response::Response(const QByteArray & src)
    : m_responseSource(""),
      m_redisReader(QSharedPointer<redisReader>(redisReaderCreate(), redisReaderFree)),
      m_result(nullptr)
{
    feed(src);
}

RedisClient::Response::~Response(void)
{
}

void RedisClient::Response::setSource(const QByteArray& src)
{
    reset();
    feed(src);
}

void RedisClient::Response::reset()
{
    m_responseSource.clear();
    m_redisReader = QSharedPointer<redisReader>(redisReaderCreate(), redisReaderFree);
    m_result.clear();
}

QByteArray RedisClient::Response::source()
{
    return m_responseSource;
}

void RedisClient::Response::appendToSource(const QByteArray& src)
{
    feed(src);
}

QByteArray RedisClient::Response::getUnusedBuffer()
{
    if (!hasUnusedBuffer())
        return QByteArray{};

    return m_responseSource.mid(m_redisReader->pos);
}

QString RedisClient::Response::toRawString() const
{
    return m_responseSource.left(1500);
}

bool RedisClient::Response::isEmpty() const
{
    return m_responseSource.isEmpty();
}

RedisClient::Response::Type RedisClient::Response::getType() const
{
    return getResponseType(m_responseSource);
}

QVariant RedisClient::Response::getValue()
{
    if (!m_result.isNull() || parse()) // Return cached result
        return *m_result;
       
    return QVariant();
}   

QVariant* convertUnsafeArray(QVariant* arr)
{
    QVariantList result;
    auto val = arr->value<QVector<QVariant*>>();

    for (int index=0; index < val.size(); ++index) {
        if (val[index]->canConvert<QVector<QVariant*>>()) {
            result.append(*convertUnsafeArray(val[index]));
        } else {
            result.append(*val[index]);
            delete val[index];
        }
    }
    delete arr;
    return (new QVariant(result));
}

bool RedisClient::Response::parse()
{
    if (m_responseSource.isEmpty())
        return false;

    QVariant* reply = nullptr;

    if (redisReaderGetReply(m_redisReader.data(), (void **)&reply) == REDIS_ERR) {
        qDebug() << "hiredis cannot parse buffer";
        return false;
    }

    if (!reply)
        return false;

    if (getType() == MultiBulk) {
        reply = convertUnsafeArray(reply);
    }

    m_result = QSharedPointer<QVariant>(reply);

    return true;
}

void RedisClient::Response::feed(const QByteArray& buffer)
{
    m_responseSource.append(buffer);

    if (redisReaderFeed(m_redisReader.data(), buffer.constData(), buffer.size()) == REDIS_ERR) {
        qDebug() << "hiredis cannot feed buffer";
    }
}

void setParent(const redisReadTask * task, QVariant* r)
{
    QVariant* parent = (QVariant*)task->parent->obj;
    Q_ASSERT(parent->isValid());

    auto value = parent->value<QVector<QVariant*>>();
    value[task->idx] = r;
    parent->setValue(value);
}

void *RedisClient::Response::createStringObject(const redisReadTask * task, char *str, size_t len)
{
    QVariant* s = new QVariant(QByteArray(str, len));

    if (task->parent)
        setParent(task, s);

    return s;
}

void *RedisClient::Response::createArrayObject(const redisReadTask * task, int elements)
{
    QVariant* arr = new QVariant();
    arr->setValue(QVector<QVariant*>(elements));

    if (task->parent)
       setParent(task, arr);

    return arr;
}

void *RedisClient::Response::createIntegerObject(const redisReadTask *task, long long value)
{
    QVariant* val = new QVariant(value);

    if (task->parent)
       setParent(task, val);

    return val;
}

void *RedisClient::Response::createNilObject(const redisReadTask *task)
{
    QVariant* nil = new QVariant();

    if (task->parent)
       setParent(task, nil);

    return nil;
}

void RedisClient::Response::freeObject(void *obj)
{
    if (obj != nullptr)
        delete (QVariant*)obj;
}

RedisClient::Response::Type RedisClient::Response::getResponseType(const QByteArray & r) const
{    
    const char typeChar = (r.length() == 0)? ' ' : r.at(0);
    return getResponseType(typeChar);
}

RedisClient::Response::Type RedisClient::Response::getResponseType(const char typeChar) const
{    
    if (typeChar == '+') return Status; 
    if (typeChar == '-') return Error;
    if (typeChar == ':') return Integer;
    if (typeChar == '$') return Bulk;
    if (typeChar == '*') return MultiBulk;
    return Unknown;
}

bool RedisClient::Response::isValid()
{
    return m_result || (m_responseSource.endsWith("\r\n") && parse());
}

bool RedisClient::Response::isMessage() const
{
    if (!isArray())
        return false;

    QVariantList result = m_result->toList();

    return result.size() == 3 && result[0] == "message";
}

bool RedisClient::Response::isArray() const
{
    return m_result && m_result->canConvert(QMetaType::QVariantList);
}

bool RedisClient::Response::hasUnusedBuffer() const
{
    return m_result
            && m_redisReader->pos > 0
            && m_redisReader->pos < m_responseSource.size();
}

bool RedisClient::Response::isAskRedirect() const
{
    return getResponseType(m_responseSource) == Error
            && m_responseSource.startsWith("-ASK");
}

bool RedisClient::Response::isMovedRedirect() const
{
    return getResponseType(m_responseSource) == Error
            && m_responseSource.startsWith("-MOVED");
}

QByteArray RedisClient::Response::getRedirectionHost() const
{
    if (!isMovedRedirect() && !isAskRedirect())
        return QByteArray();

    QByteArray hostAndPort = m_responseSource.split(' ')[2];

    return hostAndPort.split(':')[0];
}

uint RedisClient::Response::getRedirectionPort() const
{
    if (!isMovedRedirect() && !isAskRedirect())
        return 0;

    QByteArray hostAndPort = m_responseSource.split(' ')[2];

    return QString(hostAndPort.split(':')[1]).toUInt();
}

QByteArray RedisClient::Response::getChannel() const
{
    if (!isMessage())
        return QByteArray{};

    QVariantList result = m_result->toList();

    return result[1].toByteArray();
}

// TBD: add pretty printing
QString RedisClient::Response::valueToHumanReadString(const QVariant& value)
{
    if (value.isNull()) 
    {
        return "NULL";
    }
    else if (value.type() == QVariant::StringList)
    {
        //return printableString(convertQVariantList(value.toList()).join("\r\n"));//chp
        QList<QByteArray> datalist = convertQVariantList(value.toList());
        datalist.append("\r\n");
        QByteArray strtemp;
        for(int i =0; i<datalist.count(); i++)
        {
            strtemp.append(datalist.at(i));
        }
        return printableString(strtemp);
    }
    else if (value.type() == QVariant::Type::List)
    {
        QVariantList val = value.toList();
        QByteArray result;
        for (int i = 0; i < val.size(); i++)
        {
            result.append(QString("%1) ").arg(QString::number(i+1)).toUtf8());
            if (val.at(i).type() == QVariant::Type::List)
            {
                //result.append(convertQVariantList(val.at(i).toList()).join("\r\n"));//chp
                result.append(val.at(i).toByteArray());
                result.append("\r\n");
            }
            else
            {
                result.append(printableString(val.at(i).toByteArray()));
            }
            result.append("\r\n");
        }
        return result;
    }

    return value.toString();
}

bool RedisClient::Response::isErrorMessage() const
{
    return getResponseType(m_responseSource) == Error
            && m_responseSource.startsWith("-ERR");
}

bool RedisClient::Response::isDisabledCommandErrorMessage() const
{
    return isErrorMessage() && m_responseSource.contains("unknown command");
}

bool RedisClient::Response::isOkMessage() const
{
    return getResponseType(m_responseSource) == Status
            && m_responseSource.startsWith("+OK");
}
