#include "MDDrawCurve.h"
#include "DrawCurveData.h"
#include "PropertyDrawCurve.h"

#include <QDebug>
#include <QVariant>

CMDDrawCurve::CMDDrawCurve():
    m_data(new CDrawCurveData)
{
    QWidget *widget = m_data->m_curve.widget();
    setWidget(widget);
    setIsRealTime(true);
}
CMDDrawCurve::~CMDDrawCurve()
{
    delete m_data;
}
CDrawCurveData *CMDDrawCurve::data() const
{
    return m_data;
}

void CMDDrawCurve::setData(CDrawCurveData *data)
{
    m_data = data;
}

void CMDDrawCurve::setDataSourceParameter( int type, const QString &parameter)
{
    m_data->setDataSourceParameter(type,parameter);
}

void CMDDrawCurve::changeLanguage()
{
    m_data->changeLanguage();
}

void CMDDrawCurve::collectWords()
{
    m_data->collectWords();
}
void CMDDrawCurve::getBrushes(QList<CMDBrushManager *> &list)
{
    CMDDrawObj::getBrushes(list);
    m_data->m_curve.getBrushes(list);
}
void CMDDrawCurve::addAddtionalExpression()
{
    QList<int> indexs;
    QList<QString> exps;
    m_data->m_curve.getExpressionList(indexs, exps);

    addExpressionToManager(exps);
}
void CMDDrawCurve::onDataChanged(CMDExpInfo *exp)
{
    m_data->m_curve.setValue(exp->index(), exp->decimalValue());
}

void CMDDrawCurve::setStartTime(const QString &curveName, const QVariant &startTime)
{
    m_data->m_curve.setStartTime(curveName,startTime);
}

void CMDDrawCurve::setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue)
{
    m_data->m_curve.setReferenceCurveData(curveName,xValue,yValue);
}

int CMDDrawCurve::setProperty()
{
    return m_data->m_curve.setProperty();
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawCurve::type()
{
    return DrawType::Curve;
}
CMDPropertyDrawObj *CMDDrawCurve::createPropertyDrawObj()
{
    return new CPropertyDrawCurve(this);
}
void CMDDrawCurve::onInitialization()
{
    CMDDrawWidget::onInitialization();
    m_data->m_curve.setDrawEnvrionment(environment());
}
void CMDDrawCurve::initContent()
{
    CMDDrawWidget::initContent();
    resetImagePath(getImagePath());
}
CMDDrawObj *CMDDrawCurve::createSelf()
{
    return new CMDDrawCurve();
}

void CMDDrawCurve::onDoubleClick()
{  
    //1 - QDialog::Accepted
    //0 - QDialog::Rejected
    if(setProperty() == QDialog::Accepted)
        parent()->setModify(true);
}

CMDDrawObj *CMDDrawCurve::clone()
{
    CMDDrawCurve* obj = static_cast<CMDDrawCurve*>(CMDDrawWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawCurve::setIsRealTime(bool isTrue)
{
    m_data->setIsRealTime(isTrue);
}

void CMDDrawCurve::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);
    QJsonObject myJson;
    m_data->serialize(myJson);
    json.insert("MDDrawCurve",myJson);
}

void CMDDrawCurve::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);
    QJsonObject myJson = json.value("MDDrawCurve").toObject();
    m_data->deserialize(myJson);
}

DrawType CMDDrawHistory::type()
{
    return DrawType::HistoryCurve;
}

void CMDDrawHistory::setFilter(const QString &filter)
{
    data()->setFilter(filter);
}

void CMDDrawHistory::setTagFilter(const QString &filter)
{
    data()->setTagFilter(filter);
}

void CMDDrawHistory::query()
{
    data()->query();
}

void CMDDrawHistory::setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue)
{
    data()->m_curve.setReferenceCurveData(curveName,xValue,yValue);
}

CMDDrawObj *CMDDrawHistory::createSelf()
{
    return new CMDDrawHistory();
}
