#include "CReferenceCurveManager.h"
#include "CReferenceCurve.h"
#include "CXAxisManager.h"
#include "CYAxisManager.h"
#include "CLabelManager.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonArray>
#include "MDJsonConvert.h"

CReferenceCurveManager::CReferenceCurveManager():
    m_xs(NULL),
    m_ys(NULL)
{
}

CReferenceCurveManager::~CReferenceCurveManager()
{
    clearItemList();
}

void CReferenceCurveManager::appendReferenceCurve(CReferenceCurve *reCurve)
{
    m_referenceList.append(reCurve);

}

QList<CReferenceCurve *> &CReferenceCurveManager::list()
{
    return m_referenceList;
}

void CReferenceCurveManager::setList(const QList<CReferenceCurve *> &list)
{
    m_referenceList = list;
    if(m_labels)
    {
       m_labels->setReCurves(list);
    }
}

CXAxisManager *CReferenceCurveManager::xs() const
{
    return m_xs;
}

void CReferenceCurveManager::setXs(CXAxisManager *xs)
{
    m_xs = xs;
}

CYAxisManager *CReferenceCurveManager::ys() const
{
    return m_ys;
}

void CReferenceCurveManager::setYs(CYAxisManager *ys)
{
    m_ys = ys;
}

CLabelManager *CReferenceCurveManager::labels() const
{
    return m_labels;
}

void CReferenceCurveManager::setLabels(CLabelManager *labels)
{
    m_labels = labels;
}

QRectF CReferenceCurveManager::referenceCurveArea() const
{
    return m_referenceCurveArea;
}

void CReferenceCurveManager::setReferenceCurveArea(const QRectF &rect)
{
    m_referenceCurveArea = rect;
}

void CReferenceCurveManager::calculate()
{
    for(int i = 0; i < m_referenceList.count();i++){
        CReferenceCurve *curve = m_referenceList.at(i);
        if(m_xs && m_ys){
            curve->setXAxis(m_xs->findAxis(curve->xName()));
            curve->setYAxis(m_ys->findAxis(curve->yName()));
            curve->setCurveArea(m_referenceCurveArea);
            curve->calculate();
        }
    }
}

void CReferenceCurveManager::draw(QPainter *p)
{
    for(int i = 0; i < m_referenceList.count();i++)
    {
        CReferenceCurve *curve = m_referenceList.at(i);
        curve->draw(p);
    }
}

void CReferenceCurveManager::setXY()
{
    for(int i = 0; i < m_referenceList.count();i++){
        CReferenceCurve *curve = m_referenceList.at(i);
        if(m_xs && m_ys) {
            curve->setXAxis(m_xs->findAxis(curve->xName()));
            curve->setYAxis(m_ys->findAxis(curve->yName()));
        }
    }
}

void CReferenceCurveManager::serialize(QJsonObject &json)
{
    json.insert("ReferenceCurveArea",CMDJsonConvert::toJson(m_referenceCurveArea));
    int count = m_referenceList.count();
    QJsonArray array;
    for(int i =0; i<count; i++)
    {
        QJsonObject itemJson;
        m_referenceList.at(i)->serialize(itemJson);
        array.append(itemJson);
    }
    json.insert("ReferenceList",array);
}

void CReferenceCurveManager::deserialize(const QJsonObject &json)
{
    clearItemList();
    m_referenceCurveArea = CMDJsonConvert::toQRectF(json.value("ReferenceCurveArea"));
    QJsonArray array = json.value("ReferenceList").toArray();
    int count = array.count();
    for(int i = 0; i <count; i++)
    {
        QJsonObject itemJson = array.at(i).toObject();
        CReferenceCurve *curve = new CReferenceCurve;
        curve->deserialize(itemJson);
        appendReferenceCurve(curve);
   }
}

CReferenceCurveManager &CReferenceCurveManager::operator =(const CReferenceCurveManager &other)
{
    if(&other == this)
        return *this;
    clearItemList();//clear the list, free the memory

    for(int i = 0; i < other.m_referenceList.count();i++){
        CReferenceCurve *curve = other.m_referenceList.at(i);
        CReferenceCurve *tCurve = new CReferenceCurve;
        *tCurve = *curve;
        appendReferenceCurve(tCurve);
    }

    this->m_referenceCurveArea = other.m_referenceCurveArea;

    return *this;
}

void CReferenceCurveManager::clearItemList()
{
    if(!m_referenceList.isEmpty())
        qDeleteAll(m_referenceList.begin(),m_referenceList.end());
    m_referenceList.clear();
}

void CReferenceCurveManager::setValue(int index, QVariant value)
{
    for (int i = 0; i < m_referenceList.count(); i++)
    {
        CReferenceCurve* reference = m_referenceList.at(i);
        if (i == index)
        {
            reference->setStartValue(value);
        }
    }
}

void CReferenceCurveManager::setStartTime(const QString &curveName, const QVariant &startTime)
{
    for (int i = 0; i < m_referenceList.count(); i++)
    {
        CReferenceCurve* reference = m_referenceList.at(i);
        if (reference->name() == curveName)
        {
            reference->setStartTime(startTime);
        }
    }
}

void CReferenceCurveManager::getExpressionList(QList<int> &indexs, QList<QString> &expressions)
{
    for (int i = 0; i < m_referenceList.count(); i++)
    {
        CReferenceCurve* re = m_referenceList.at(i);
        if (re->variantDepict().isEmpty())
            continue;
        indexs.append(i);
        expressions.append(re->variantDepict());
    }
}
