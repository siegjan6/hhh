#include "MDDrawGauge.h"
#include "DrawGaugeData.h"
#include "PropertyDrawGauge.h"

CMDDrawGauge::CMDDrawGauge()
{
    m_data = new CDrawGaugeData();
}
CMDDrawGauge::~CMDDrawGauge()
{
    delete m_data;
}

bool CMDDrawGauge::openDesign()
{
    return m_data->m_gauge.openDesign();
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawGauge::type()
{
    return DrawType::Gauge;
}
CMDPropertyDrawObj *CMDDrawGauge::createPropertyDrawObj()
{
    return new CPropertyDrawGauge(this);
}
void CMDDrawGauge::onInitialization()
{
    m_data->m_gauge.setDrawEnvrionment(environment());
    m_data->m_gauge.setParent(parent());
}
CMDDrawObj *CMDDrawGauge::createSelf()
{
    return new CMDDrawGauge();
}
void CMDDrawGauge::onGeneratePath()
{
    CMDDrawVector::onGeneratePath();

    m_data->m_gauge.setRect(rect().toRect());
}
void CMDDrawGauge::onPaint(QPainter *p)
{
    m_data->m_gauge.paint(p);
}

void CMDDrawGauge::onDoubleClick()
{
    m_data->m_gauge.onDoubleClick();
    parent()->setModify(true);
}

void CMDDrawGauge::changeLanguage()
{
    m_data->m_gauge.changeLanguage();
}

void CMDDrawGauge::collectWords()
{
    m_data->m_gauge.collectWords();
}
CMDDrawObj *CMDDrawGauge::clone()
{
    CMDDrawGauge* obj = static_cast<CMDDrawGauge*>(CMDDrawVector::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawGauge::serialize(QJsonObject &json)
{
    CMDDrawVector::serialize(json);
    QJsonObject myJson;
    m_data->serialize(myJson);
    json.insert("MDDrawGauge",myJson);
}

void CMDDrawGauge::deserialize(const QJsonObject &json)
{
    CMDDrawVector::deserialize(json);
    QJsonObject myJson = json.value("MDDrawGauge").toObject();
    m_data->deserialize(myJson);
}
//////////////////////////////////////////////////////////////////////////
QList<QStringList> CMDDrawGauge::expNames()
{
    QList<QStringList> list;
    list << QStringList({"$drawvector.fill"});
    list << QStringList({"$drawvector.fillBrush"});
    list << QStringList({"$drawvector.brush"});
    list << QStringList({"$drawvector.pen"});
    return list;
}
void CMDDrawGauge::addAddtionalExpression()
{
    QList<int> indexs;
    QList<QString> exps;
    m_data->m_gauge.getExpressionList(indexs, exps);

    addExpressionToManager(exps);
}
void CMDDrawGauge::onDataChanged(CMDExpInfo *exp)
{
    m_data->m_gauge.setValue(exp->index(), exp->decimalValue());
}
//////////////////////////////////////////////////////////////////////////
