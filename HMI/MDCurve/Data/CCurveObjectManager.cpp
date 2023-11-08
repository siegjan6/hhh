#include "CCurveObjectManager.h"
#include "CCurveObject.h"
#include "CXAxisManager.h"
#include "CYAxisManager.h"
#include "CLabelManager.h"
#include "MDJsonConvert.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
//#include <sys/time.h>
#include <time.h>

CCurveObjectManager::CCurveObjectManager():
    m_xs(NULL),
    m_ys(NULL)
{
}

CCurveObjectManager::~CCurveObjectManager()
{
    clearItemList();
}

void CCurveObjectManager::appendCurve(CCurveObject *curve)
{
    m_list.append(curve);
    if(m_labels){
        m_labels->addCurve(curve);
    }
}

QList<CCurveObject *> &CCurveObjectManager::list()
{
    return m_list;
}

void CCurveObjectManager::setList(const QList<CCurveObject *> &list)
{
    m_list = list;

    if(m_labels)
    {
        m_labels->setCurves(list);
    }
}

CYAxisManager *CCurveObjectManager::ys() const
{
    return m_ys;
}

void CCurveObjectManager::setYs(CYAxisManager *ys)
{
    m_ys = ys;
}

CXAxisManager *CCurveObjectManager::xs() const
{
    return m_xs;
}

void CCurveObjectManager::setXs(CXAxisManager *xs)
{
    m_xs = xs;
}

QRectF CCurveObjectManager::curveArea() const
{
    return  m_curveArea;
}

void CCurveObjectManager::setCurveArea(const QRectF &rect)
{
    m_curveArea = rect;
}

void CCurveObjectManager::setType(bool isReal)
{
    for(int i = 0; i < m_list.count();i++){
        CCurveObject *curve = m_list.at(i);
        curve->setType(isReal);
    }
}

void CCurveObjectManager::setFresh(qint32 ms)
{
    for(int i = 0; i < m_list.count();i++){
        CCurveObject *curve = m_list.at(i);
        curve->setFresh(ms);
    }
}

CLabelManager *CCurveObjectManager::labels() const
{
    return m_labels;
}

void CCurveObjectManager::setLabels(CLabelManager *labels)
{
    m_labels = labels;
}


/*!
\section data source
*/
CDataSource CCurveObjectManager::dataSource() const
{
    return m_dataSource;
}

void CCurveObjectManager::setDataSource(const CDataSource &dataSource)
{
    m_dataSource = dataSource;
}

QString CCurveObjectManager::sourceName() const
{
    return m_dataSource.source();
}
void CCurveObjectManager::setSourceName(const QString &sourceName)
{
    m_dataSource.setSource(sourceName);
}

QString CCurveObjectManager::tableName() const
{
    return m_dataSource.table();
}
void CCurveObjectManager::setTabelName(const QString &tableName)
{
    m_dataSource.setTable(tableName);
}

QStringList CCurveObjectManager::fields() const
{
    return m_dataSource.fields();
}

void CCurveObjectManager::setFields(const QStringList &fields)
{
    m_dataSource.setFields(fields);
}

void CCurveObjectManager::calculate()
{
    QStringList fields;
    for(int i = 0; i < m_list.count();i++)
    {
        CCurveObject *curve = m_list.at(i);
        if(m_xs && m_ys)
        {
            curve->setXAxis(m_xs->findAxis(curve->xName()));
            curve->setYAxis(m_ys->findAxis(curve->yName()));
            curve->setCurveArea(m_curveArea);
            curve->calculate();
            fields.append(curve->variantDepict());
        }
    }
    m_dataSource.setFields(fields);
}

void CCurveObjectManager::draw(QPainter *p)
{
    for(int i = 0; i < m_list.count();i++)
    {
        CCurveObject *curve = m_list[i];
        curve->draw(p);
    }
}

void CCurveObjectManager::serialize(QJsonObject &json)
{
    json.insert("CurveArea",CMDJsonConvert::toJson(m_curveArea));
    QJsonArray array;
    int count = m_list.count();
    for(int i =0; i< count; i++)
    {
        QJsonObject itemJson;
        CCurveObject *curve = m_list.at(i);
        curve->serialize(itemJson);
        array.append(itemJson);
    }
    json.insert("CurveArray",array);
    QJsonObject dataJson;
    m_dataSource.serialize(dataJson);
    json.insert("DataSource",dataJson);
}

void CCurveObjectManager::deserialize(const QJsonObject &json)
{
    clearItemList();
    m_curveArea = CMDJsonConvert::toQRectF(json.value("CurveArea"));
    QJsonArray array = json.value("CurveArray").toArray();
    int count = array.count();
    for(int i = 0; i< count; i++)
    {
        QJsonObject itemJson = array.at(i).toObject();
        CCurveObject *curve = new CCurveObject;
        curve->deserialize(itemJson);
        m_list.append(curve);

    }
    QJsonObject dataJson = json.value("DataSource").toObject();
    m_dataSource.deserialize(dataJson);
}


void CCurveObjectManager::setXY()
{
    for(int i = 0; i < m_list.count();i++){
        CCurveObject *curve = m_list.at(i);
        if(m_xs && m_ys) {
            curve->setXAxis(m_xs->findAxis(curve->xName()));
            curve->setYAxis(m_ys->findAxis(curve->yName()));
        }
    }
}

CCurveObjectManager &CCurveObjectManager::operator =(const CCurveObjectManager &other)
{
    if(&other == this)
        return *this;
    clearItemList();//clear the list, free the memory

    for(int i = 0; i < other.m_list.count();i++){
        CCurveObject *curve = other.m_list.at(i);
        CCurveObject *tCurve = new CCurveObject;
        *tCurve = *curve;
        appendCurve(tCurve);
    }

    this->m_dataSource = other.m_dataSource;
    this->m_curveArea = other.m_curveArea;

    return *this;
}

void CCurveObjectManager::setProjectPath(const QString &path)
{
    m_projectPath = path;
}

QString CCurveObjectManager::projectPath() const
{
    return m_projectPath;
}

void CCurveObjectManager::clearItemList()
{
    if(!m_list.isEmpty())
        qDeleteAll(m_list.begin(),m_list.end());
    m_list.clear();
}

