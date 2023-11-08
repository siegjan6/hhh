#include "CXAxisManager.h"
#include "CFind.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

const int Interval = 5;
CXAxisManager::CXAxisManager():
    m_orientation(CDrawMode::Orientation::RigthToLeft),
    m_startScale(0),
    m_endScale(1),
    m_autoHideCurve(false),
    m_divNum(3)
{
    CXAxis *x = new CXAxis;
    x->setName(("X1"));
    m_list.append(x);
    //    setOrientation(CDrawMode::Orientation::RigthToLeft);
}
CXAxisManager::~CXAxisManager()
{
    clearItemList();
}

QList<CXAxis *> &CXAxisManager::list()
{
    return m_list;
}

void CXAxisManager::setList(const QList<CXAxis *> &list)
{
    m_list = list;
}


CDrawMode::Orientation CXAxisManager::orientation() const
{
    return m_orientation;
}

void CXAxisManager::setOrientation(const CDrawMode::Orientation &orientation)
{    

    if(m_orientation == orientation)
        return;
    m_orientation = orientation;

    for(int i= 0; i < m_list.count();i++){
        CXAxis *x = m_list.at(i);
        setXOrientation(x);
    }
}
double CXAxisManager::startScale() const
{
    return m_startScale;
}

double CXAxisManager::endScale() const
{
    return m_endScale;
}

void CXAxisManager::setBeginEnd(double startScale, double endScale)
{
    if(startScale > endScale || startScale < 0 || endScale > 1)
        return;
    m_startScale = startScale;
    m_endScale = endScale;

    for(int i= 0; i < m_list.count();i++){
        CXAxis *x = m_list.at(i);

        x->setStartScale(m_startScale);
        x->setEndScale(m_endScale);
    }

}
bool CXAxisManager::autoHideCurve() const
{
    return m_autoHideCurve;
}

void CXAxisManager::setAutoHideCurve(bool autoHideCurve)
{
    m_autoHideCurve = autoHideCurve;
}

void CXAxisManager::setXBoundVisible(QList<bool> boundVisibleList)
{
    m_xBoundVisbleList = boundVisibleList;
}

QList<bool> CXAxisManager::xBoundVisble()
{
    return m_xBoundVisbleList;
}

/*!
\section
*/

double CXAxisManager::height()
{
    double total = 0;
    for(int i = 0; i < m_list.count();i++)
    {
        CXAxis *x = m_list.at(i);
        if(m_autoHideCurve && m_xBoundVisbleList.at(i))
        {
            if(m_list.count() == 1)
            {
                // jump the first
            }
            else
            {
                continue;
            }
        }
        total += x->xHeight();
    }
    return total;
}

void CXAxisManager::setEndTime(const QDateTime &time,bool isStudioMode)
{
    for(int i= 0; i < m_list.count();i++){
        CXAxis *x = m_list.at(i);
        if(CXAxis::ValueType::Time == x->valueType())
            x->setFinalEnd(time,isStudioMode);
    }
}

void CXAxisManager::appendAxis(CXAxis *x)
{
    m_list.append(x);

    if(CDrawMode::Orientation::LeftToRight == m_orientation){
        if(CXAxis::ValueType::Time == x->valueType()){
            QDateTime oldTime = x->endTime();
            qint64 timeLength = x->totalLength();
            x->setFinalEnd(oldTime.addSecs(timeLength));
        }
    }

    setBeginEnd(m_startScale,m_endScale);
}
CXAxis *CXAxisManager::findAxis(const QString &name)
{
    if(!name.isEmpty()){
        for(int i= 0; i < m_list.count();i++){
            CXAxis *axis = m_list.at(i);
            if(axis->name() == name)
                return axis;
        }
    }
    return NULL;
}

void CXAxisManager::setOrientationOnRun()
{
    for(int i= 0; i < m_list.count();i++){
        CXAxis *x = m_list.at(i);
         setXOrientation(x,false);
    }
}
/*!
\section
*/
void CXAxisManager::clearItemList()
{
    if(!m_list.isEmpty())
        qDeleteAll(m_list.begin(),m_list.end());
    m_list.clear();
    m_xBoundVisbleList.clear();
}

void CXAxisManager::setXOrientation(CXAxis *x,bool isStudioMode)
{
    if(CXAxis::ValueType::Time == x->valueType()){
        QDateTime oldTime = x->endTime();
        qint64 timeLength = x->totalLength();
        if(CDrawMode::Orientation::LeftToRight == m_orientation)
        {
             x->setFinalEnd(oldTime.addSecs(timeLength),isStudioMode);

        }
    }
}
/*!
\section
*/
QRectF CXAxisManager::drawArea() const
{
    return m_drawArea;
}

void CXAxisManager::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
}
int CXAxisManager::divNum() const
{
    return m_divNum;
}

void CXAxisManager::setDivNum(int divNum)
{
    m_divNum = divNum;
}
void CXAxisManager::draw(QPainter *p)
{
    for(int i = 0; i < m_list.count();i++)
    {
        CXAxis *x = m_list[i];
        if(m_autoHideCurve && m_xBoundVisbleList.at(i))
        {
            if(m_list.count() == 1)
            {
                // jump the first
            }
            else
            {
                continue;
            }
        }
        x->draw(p);
    }
}

void CXAxisManager::calculate()
{
    int top = m_drawArea.top();
    for(int i = 0; i < m_list.count();i++)
    {
        CXAxis *x = m_list.at(i);
        if(m_autoHideCurve && m_xBoundVisbleList.at(i))
        {
            if(m_list.count() == 1)
            {
                // jump the first
            }
            else
            {
                continue;
            }
        }
        if(!x->axisVisible())
            continue;

        QRect r = QRect(m_drawArea.left(), top, m_drawArea.width(), x->xHeight() - Interval);
        x->setDrawArea(r);
        top += x->xHeight();
        x->setDivNum(m_divNum);
        x->calculate();
    }
}

/*!
\section
*/
void CXAxisManager::serialize(QJsonObject &json)
{
    QJsonObject xAxisJson;
    xAxisJson.insert("DivNum",m_divNum);
    xAxisJson.insert("Orientation",(int)m_orientation);
    xAxisJson.insert("StartScale",m_startScale);
    xAxisJson.insert("EndScale",m_endScale);
    xAxisJson.insert("AutoHideCurve",m_autoHideCurve);
    QJsonArray array;
    int count = m_list.count();
    for(int i = 0; i <count; i++)
    {
        QJsonObject object;
        m_list.at(i)->serialize(object);
        array.append(object);
    }
    json.insert("List",array);
    QJsonArray boundArray;
    int boundCount = m_xBoundVisbleList.count();
    for(int i = 0; i< boundCount; i++)
    {
        boundArray.append(m_xBoundVisbleList.at(i));
    }
    json.insert("BoundList",boundArray);
    json.insert("XAxisManager",xAxisJson);

}

void CXAxisManager::deserialize(const QJsonObject &json)
{
    clearItemList();
    QJsonObject jsonObject = json.value("XAxisManager").toObject();
    m_divNum = jsonObject.value("DivNum").toInt();
    int orientation = jsonObject.value("Orientation").toInt();
    m_orientation = (CDrawMode::Orientation)orientation;
    m_startScale = jsonObject.value("StartScale").toDouble();
    m_endScale = jsonObject.value("EndScale").toDouble();
    m_autoHideCurve = jsonObject.value("AutoHideCurve").toBool();
    QJsonArray array = json.value("List").toArray();
    int count = array.count();
    for(int i =0; i<count; i++)
    {
        CXAxis *x = new CXAxis;
        QJsonObject itemJson;
        itemJson = array.at(i).toObject();
        x->deserialize(itemJson);
        m_list.append(x);
    }
    QJsonArray boundArray = json.value("BoundList").toArray();
    int boundCount = boundArray.count();
    for(int i = 0; i<boundCount; i++)
    {
        bool isBound = boundArray.at(i).toBool();
        m_xBoundVisbleList.append(isBound);
    }
}

CXAxisManager &CXAxisManager::operator =(const CXAxisManager &other)
{
    if(&other == this)
        return *this;
    this->m_drawArea = other.m_drawArea;
    this->m_divNum = other.m_divNum;
    this->m_orientation = other.m_orientation;
    this->m_startScale = other.m_startScale;
    this->m_endScale = other.m_endScale;
    this->m_autoHideCurve = other.m_autoHideCurve;

    clearItemList();
    for(int i= 0; i < other.m_list.count();i++){
        CXAxis *x = other.m_list.at(i);
        CXAxis *nX = new CXAxis;
        *nX = *x;
        m_list.append(nX);
    }
    for(int i=0; i< other.m_xBoundVisbleList.count();i++)
    {
        bool bound = other.m_xBoundVisbleList.at(i);
        m_xBoundVisbleList.append(bound);
    }
    return *this;
}


