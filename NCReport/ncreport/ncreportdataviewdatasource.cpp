#include "ncreportdataviewdatasource.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
//#include "MDDBQueryConfigure\MDDBQueryConfigure.h"
#include "../../include/MDDBQueryConfigure/MDDBQueryConfigure.h"
//#include "D:\checkout\MindSCADA_CPP\include\MDDBQueryConfigure\MDDBQueryConfigure.h"
#include <assert.h>
#include "IMDDBQueryEngine.h"
//#include "D:\checkout\MindSCADA_CPP\include\MDDBQueryEngine\IMDDBQueryEngine.h"
#include "MDDatabaseRecordset.h"
//#include "D:\checkout\MindSCADA_CPP\include\MDSQLDataBaseProxy\MDDatabaseRecordset.h"
#include <QDebug>
#include <QThread>
#include <QMap>
#include <QMutex>

/*
//<临时>
static IMDDBQueryEngine * pEngine = 0;
static void InitDBQueryEngine() //TODO:临时
{
    if(pEngine)
        return ;

    pEngine = CMDDBQueryEngineFactory::createDBQueryEngine();
    bool b = pEngine->load("D:/svn/HaiDe/ncreport/ConfigureTestCase/case1");
    assert(b);
}
//</临时>
*/

typedef QMap<QString, CMDDatabaseRecordset *> RecordsetMap;
class RecordSetCache
{
public:
    CMDDatabaseRecordset * find(const QString & queryID);
    //函数功能:查询超时后,将超时的query的ID加入Cache
    //说明:.在每次查询时,都会遍历此列表,尝试删除对应的Recordset
    void addQueryIDTimeouted(const QString & queryID);
    void deleteRecordset(const QString & queryID);
    void addRecordset(const QString & queryID, CMDDatabaseRecordset * pRecordset);
 protected:
    CMDDatabaseRecordset * findNoLock(const QString & queryID);
 protected:
    RecordsetMap m_RecordsetMap;
    QSet<QString> m_KeysTimeout; //使用QSet 保证不插入重复元素
    QMutex m_Mutux;
};

CMDDatabaseRecordset * RecordSetCache::find(const QString & queryID)
{
    QMutexLocker locker(&m_Mutux);
    return findNoLock(queryID);
}

CMDDatabaseRecordset * RecordSetCache::findNoLock(const QString & queryID)
{
    RecordsetMap::iterator ite = m_RecordsetMap.find(queryID);
    if(ite == m_RecordsetMap.end())
    {
        return 0;
    }
    return ite.value();
}

//函数功能:查询超时后,将超时的query的ID加入Cache
//说明:.在每次查询时,都会遍历此列表,尝试删除对应的Recordset
void RecordSetCache::addQueryIDTimeouted(const QString & queryID)
{
    QMutexLocker locker(&m_Mutux);
    m_KeysTimeout.insert(queryID);
}

void RecordSetCache::deleteRecordset(const QString & queryID)
{
    QMutexLocker locker(&m_Mutux);
    CMDDatabaseRecordset * pRecord = findNoLock(queryID);
    if(pRecord)
        delete pRecord;
    m_RecordsetMap.remove(queryID);

    //删除超时的查询 //TODO：待测试
    QList<QString> keysRemoved; //TODO:会反复的new内存么
    foreach(const QString & key, m_KeysTimeout)
    {
        pRecord = findNoLock(key);
        if(pRecord)
        {
            delete pRecord;
            m_RecordsetMap.remove(key);
            keysRemoved.append(key);
        }
    }

    foreach(const QString & key, keysRemoved)
    {
        m_KeysTimeout.remove(key);
    }
}

void RecordSetCache::addRecordset(const QString & queryID, CMDDatabaseRecordset * pRecordset)
{
    QMutexLocker locker(&m_Mutux);
    if(queryID.isEmpty())
        return ;

    //id不可能重复 所以不会有内存泄漏
    CMDDatabaseRecordset * pRecord = findNoLock(queryID);
    if(pRecord)
        return ; //TODO:log

    m_RecordsetMap.insert(queryID, pRecordset);
}

static RecordSetCache rsCache;

class ReportNotify : public IMDDBRecordSetNotify
{
public:
    virtual void NotityDBQueryResult(const QString &strQueryID, CMDDatabaseRecordset *);
};

void ReportNotify::NotityDBQueryResult(const QString & strQueryID, CMDDatabaseRecordset * recordset)
{
    rsCache.addRecordset(strQueryID, recordset);
}

static ReportNotify localNotify;

//前提假设:NCReportDataViewDataSource单线程操作
NCReportDataViewDataSource::NCReportDataViewDataSource(QObject * parent)
    :NCReportDataSource(parent),
      m_pView(0),
      m_pRecordset(0),
      m_pDBQueryEngine(0)
{
    setDataSourceType(DataView);
    setLocationType(Static);
    //InitDBQueryEngine();//TODO:temp
}

NCReportDataViewDataSource::~NCReportDataViewDataSource()
{
    delete m_pView;
}

bool NCReportDataViewDataSource::open(OPEN_MODEL model)
{
    m_OpenModel = model;
    if(m_ViewName.isEmpty())
        return false;

    if(OPEN_MODEL_DESIGN == model)
    {
        delete m_pView;
        m_pView = 0;
        QString errStr;
        if(!DBQ::View_LoadFromDir(m_ViewPath, m_ViewName, &m_pView, errStr))
            return false; //TODO:log

        return true;
    }

    if(model != OPEN_MODEL_QUERY)
        return false; //TODO:LOG

    if(!m_pDBQueryEngine)
         return false; //TODO:LOG

    //为查询模式时 从引擎读出视图路径
    if(m_ViewPath.isEmpty())
        m_ViewPath = m_pDBQueryEngine->getViewPath();

    //TODO:要优化获得View的方式
    delete m_pView;
    m_pView = 0;
    QString errStr;
    if(!DBQ::View_LoadFromDir(m_ViewPath, m_ViewName, &m_pView, errStr))
        return false; //TODO:log

    QString queryID;
    if(!m_pDBQueryEngine)
         return false; //TODO:log

    bool b = m_pDBQueryEngine->queryView(m_ViewName, &localNotify, queryID);
    if(!b)
    {
        setOpened(false);
        return false;
    }

    rsCache.deleteRecordset(m_QueryID);
    m_QueryID = queryID; //为了在第一次查询的时候删除
    m_pRecordset = 0;

    for(int i=0;i<30*10;i++) //最多等待30秒
    {
        m_pRecordset = rsCache.find(m_QueryID);
        if(m_pRecordset)
        {
            setOpened(true); //同步查询操作 只有在查询操作执行完后 才能根据查询结果判断是否打开
            return true;
        }
        QThread::msleep(100);//睡100毫秒
        //查询m_QueryID标识的查询的状态。共有以下几种状态
        //1.没有此查询
        //2.正在查询
        //3.查询成功
        //4.查询失败

        if(DBQUERY_STATUS_FAIL == m_pDBQueryEngine->getQueryStatus(m_QueryID))
        {
            //如何查询失败 可通过m_QueryID获得此次查询的错误原因.
            //m_pDBQueryEngine->getQueryErrorCode(m_QueryID);
            break; //退出循环之后 会执行timeout, setOpened(false)操作
        }
    }

    //超时了 或查询失败
    rsCache.addQueryIDTimeouted(queryID);//TODO:测试
    setOpened(false);
    return false;
}

bool NCReportDataViewDataSource::close()
{
    setOpened(false);
    return true;
}

bool NCReportDataViewDataSource::first()
{
    if(!m_pRecordset)
        return false;

    m_pRecordset->moveFirst();
    return true;
}

bool NCReportDataViewDataSource::last()
{
    if(!m_pRecordset)
        return true; //是否是最后一条记录:因为没有查找到 所以只能认为是最后一条记录 所以返回true

    m_pRecordset->moveLast();
    return true;
}

bool NCReportDataViewDataSource::next(bool isSamplingPageBreak)
{
    Q_UNUSED(isSamplingPageBreak);

    if(!m_pRecordset)
        return false;

    if(m_pRecordset->isLast())
        return false;

    m_pRecordset->moveNext();
    return true;
}

bool NCReportDataViewDataSource::previous()
{
    if(!m_pRecordset)
        return false;

    m_pRecordset->movePrevious();
    return true;
}

bool NCReportDataViewDataSource::hasNext()
{
    if(!m_pRecordset)
        return false;

    return !m_pRecordset->isLast();//???
}

bool NCReportDataViewDataSource::seek( int index )
{
    Q_UNUSED(index)
    assert(false);//TODO:目前没有调到此函数
    return false;
}

int NCReportDataViewDataSource::size() const
{
    if(!m_pRecordset)
        return 0;

    return m_pRecordset->rowCount();
}

QVariant NCReportDataViewDataSource::value( const QString& columnname, bool* ok, int nQTDataRole) const
{
    Q_UNUSED(columnname)
    Q_UNUSED(ok)
    Q_UNUSED(nQTDataRole)
    assert(false);//TODO:目前没有调到此函数
    return QVariant();
}

//???ok, ???nQTDataRole
QVariant NCReportDataViewDataSource::value( int column, bool* ok, int nQTDataRole) const
{
    Q_UNUSED(ok)
    Q_UNUSED(nQTDataRole)
    if(!m_pRecordset)
        return QVariant();

    return m_pRecordset->value(column);
}

bool NCReportDataViewDataSource::isValid() const
{
    assert(false); //TODO:目前没有调到此函数
    return false;
}
//</实现纯虚接口>
//-------------------------------
bool NCReportDataViewDataSource::update()
{
    assert(false); //TODO:目前没有调到此函数
    return false;
}

int NCReportDataViewDataSource::rowCount() const
{
    if(!m_pRecordset)
        return 0;

    return m_pRecordset->rowCount();
}

int NCReportDataViewDataSource::columnCount() const
{
    if (!m_pView)
         return NCReportDataSource::columnCount();

    return m_pView->fields.size();
}

bool NCReportDataViewDataSource::read( NCReportXMLReader* reader )
{
    if(reader->isAttrib("dataView"))
        m_ViewName = reader->attribToString("dataView");
    return true;
}

bool NCReportDataViewDataSource::write( NCReportXMLWriter* writer )
{
    writer->writeAttribute("dataView", m_ViewName);
    return true;
}

void NCReportDataViewDataSource::evaluate( NCReportEvaluator* evaluator)
{
    Q_UNUSED(evaluator)
    //assert(false);
}

QStringList NCReportDataViewDataSource::keywordList()
{
    return NCReportDataSource::keywordList();
}

QStringList NCReportDataViewDataSource::columnNames()
{
    QStringList colNames;
    if (!TestAndLoadView())
        return colNames;

    QString fieldName;
    foreach (DBQ::ViewField * pField, m_pView->fields)
    {
        if(!pField)
            return QStringList(); //TODO:log

        //列的名称为"表名.字段名"
        fieldName = pField->tableName;
        fieldName.append(".").append(pField->fieldName);
        colNames.append(fieldName);
    }

    return colNames;
}

QString NCReportDataViewDataSource::columnName(int column) const
{
    if(!m_pView)
    {
        return QString();
    }

    DBQ::ViewField * pField = m_pView->fields.at(column); //TODO:测试越界情况
    if(!pField)
        return QString(); //TODO:log

    if(pField->alias.isEmpty())
    {
        //列的名称为"表名.字段名"
        QString fieldName(pField->tableName);
        fieldName.append(".").append(pField->fieldName);
        return fieldName;
    }

    return pField->alias;
}

bool NCReportDataViewDataSource::hasColumn(int column) const
{
    Q_UNUSED(column)
    assert(false); //TODO:目前没有调到此函数
    return false;
}

bool NCReportDataViewDataSource::hasColumn(const QString& columnname) const
{
    Q_UNUSED(columnname)
    assert(false); //TODO:目前没有调到此函数
    return false;
}

int NCReportDataViewDataSource::columnIndexByName(const QString& columnname) const
{
    if(!m_pRecordset) //TODO:待测试
        return -1;

    return m_pRecordset->getFiledIndex(columnname);
}

QString NCReportDataViewDataSource::getViewName()
{
    return m_ViewName;
}

void NCReportDataViewDataSource::setViewName(const QString & viewName)
{
    m_ViewName = viewName;
}

void NCReportDataViewDataSource::setViewPath(const QString & viewPath)
{
    m_ViewPath = viewPath;
}

void NCReportDataViewDataSource::setQueryEngine(IMDDBQueryEngine * pDBQueryEngine)
{
    m_pDBQueryEngine = pDBQueryEngine;
}

bool NCReportDataViewDataSource::TestAndLoadView()
{
    if(m_ViewName.isEmpty())
        return false;
    if(m_pView)
        return true;

    if(m_pDBQueryEngine)
    {
        if(m_ViewPath.isEmpty())
            m_ViewPath = m_pDBQueryEngine->getViewPath();
    }

    //m_pView为空
    QString errStr;
    if(!DBQ::View_LoadFromDir(m_ViewPath, m_ViewName, &m_pView, errStr))
        return false; //TODO:log

    return true;
}
