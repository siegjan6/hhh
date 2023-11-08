#include "MDCurve.h"
#include "IDrawEnvironment.h"
#include "CCurveWidget.h"

CMDCurve::CMDCurve():
    m_widget(new CCurveWidget)
{

}

CMDCurve::~CMDCurve()
{
    if(m_widget){
        delete m_widget;
        m_widget = NULL;
    }
}

QWidget *CMDCurve::widget()
{
    return m_widget;
}

void CMDCurve::setDrawEnvrionment(IDrawEnvrionment *value)
{
    m_widget->setDrawEnvironment(value);
}

void CMDCurve::serialize(QJsonObject &json)
{
    m_widget->serialize(json);
}

void CMDCurve::deserialize(const QJsonObject &json)
{
   m_widget->deserialize(json);
}

void CMDCurve::changeLanguage()
{
    m_widget->changeLanguage();
}

void CMDCurve::collectWords()
{
    m_widget->collectWords();
}

void CMDCurve::setIsRealTime(bool isRealTime)
{
    m_widget->setIsRealTime(isRealTime);
}

int CMDCurve::setProperty()
{
    return m_widget->setDILProperty();
}

void CMDCurve::setHistoryDataSearchFilter(const QString &filter)
{
    m_widget->setHistoryDataSearchFilter(filter);
}

void CMDCurve::setHistoryTagSearchFilter(const QString &filter)
{
    m_widget->setHistoryTagSearchFilter(filter);
}

void CMDCurve::setDataSourceParameter(int type, const QString &parameter)
{
    m_widget->setDataSourceParameter(type,parameter);
}

void CMDCurve::setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue)
{
   m_widget->setReferenceCurveData(curveName,xValue,yValue);
}

void CMDCurve::query()
{
    m_widget->query();
}

CMDCurve &CMDCurve::operator =(const CMDCurve &other)
{
    if(&other == this)
        return *this;

//    delete m_widget;
//    *m_widget->m_curve = *other.m_widget->m_curve;
    m_widget->setCurve(other.m_widget->clone(m_widget));
//    m_widget->setCurveParentWidget(m_widget);
//    m_widget = other.m_widget->clone();
    return *this;
}
void CMDCurve::getBrushes(QList<CMDBrushManager*>& list)
{
    m_widget->getBrushes(list);
}
void CMDCurve::setValue(int index, QVariant value)
{
    m_widget->setValue(index,value);
}

void CMDCurve::getExpressionList(QList<int> &indexs, QList<QString> &expressions)
{
    m_widget->getExpressionList(indexs,expressions);
}

void CMDCurve::setStartTime(const QString &curveName, const QVariant &startTime)
{
    m_widget->setStartTime(curveName,startTime);
}

