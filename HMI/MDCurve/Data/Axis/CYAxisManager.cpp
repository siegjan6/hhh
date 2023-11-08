#include "CYAxisManager.h"
#include "MDJsonConvert.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

CYAxisManager::CYAxisManager():
    m_scale(1),
    m_autoHideCurve(false),
    m_labelHeight(40),
    m_divNum(3)
{
    CYAxis *y = new CYAxis;
    y->setName("Y1");
    m_list.append(y);
}
CYAxisManager::~CYAxisManager()
{
    clearItemList();
}

/*!
\section Base
*/
QList<CYAxis *> &CYAxisManager::list()
{
    return m_list;
}

void CYAxisManager::setList(const QList<CYAxis *> &list)
{
    m_list = list;
}

int CYAxisManager::scale() const
{
    return m_scale;
}

void CYAxisManager::setScale(int scale)
{
    m_scale = scale;
}

bool CYAxisManager::autoHideCurve() const
{
    return m_autoHideCurve;
}

void CYAxisManager::setAutoHideCurve(bool autoHideCurve)
{
    m_autoHideCurve = autoHideCurve;
}

void CYAxisManager::setYBoundVisible(QList<bool> boundVisibleList)
{
    m_yBoundVisbleList = boundVisibleList;
}

QList<bool> CYAxisManager::yBoundVisble()
{
    return m_yBoundVisbleList;
}

/*!
\section
*/
void CYAxisManager::appendCurve(CYAxis *y)
{
    m_list.append(y);
}
CYAxis *CYAxisManager::findAxis(const QString &name)
{
    if(!name.isEmpty())
    {
        for(int i = 0; i < m_list.count();i++){
            CYAxis *axis = m_list.at(i);
            if(name == axis->name())
                return axis;
        }
    }
    return NULL;
}

/*!
\section calculate and draw
*/
void CYAxisManager::setRect(const QRectF &left, const QRectF &right)
{
    m_leftRect = left;
    m_rightRect = right;
}

double CYAxisManager::width(bool isLeft)
{
    double width = 0;

    for(int i = 0; i < m_list.count();i++)
    {
        CYAxis *y = m_list.at(i);
        if(m_autoHideCurve && m_yBoundVisbleList.at(i)) {
            if(m_list.count()==1) {
                // jump the first
            } else {
                continue;
            }
        }
        if(!y->axisVisible())
            continue;

        if(isLeft){
            if(y->isLeft())
                width += y->yWidth();
        } else {
            if(!y->isLeft())
                width += y->yWidth();
        }
    }
    return width;
}
double CYAxisManager::labelHeight() const
{
    return m_labelHeight;
}

void CYAxisManager::setLabelHeight(double labelHeight)
{
    m_labelHeight = labelHeight;
}


int CYAxisManager::divNum() const
{
    return m_divNum;
}

void CYAxisManager::setDivNum(int divNum)
{
    m_divNum = divNum;
}
void  CYAxisManager::calculate()
{
    double leftX = m_leftRect.right();
    double rightX = m_rightRect.left();
    for(int i = 0; i < m_list.count();i++)
    {
        CYAxis *y = m_list.at(i);
        if(m_autoHideCurve && m_yBoundVisbleList.at(i)){
            if(m_list.count() == 1) {
                // jump the first
            } else {
                continue;
            }
        }
        if(!y->axisVisible())
            continue;
        QRectF r;
        if (y->isLeft()) {
            leftX -= y->yWidth();
            r = QRectF(leftX, m_leftRect.top(), y->yWidth(), m_leftRect.height());
        } else {
            r = QRect(rightX, m_rightRect.top(), y->yWidth(), m_rightRect.height());
            rightX += y->yWidth();
        }
        y->setDrawArea(r);
        y->calculate(m_labelHeight,m_divNum);
    }
}

void  CYAxisManager::draw(QPainter *p)
{
    for(int i = 0; i < m_list.count();i++)
    {
        CYAxis *y = m_list[i];
        if(m_autoHideCurve && m_yBoundVisbleList.at(i)){
            if(m_list.count() ==1) {
                // jump the first
            } else {
                continue;
            }
        }
        y->draw(p);
    }
}

/*!
\section serialize
*/
void CYAxisManager::serialize(QJsonObject &json)
{
    QJsonObject yAxisJson;
    yAxisJson.insert("LeftRect",CMDJsonConvert::toJson(m_leftRect));
    yAxisJson.insert("RightLeft",CMDJsonConvert::toJson(m_rightRect));
    yAxisJson.insert("LabelHeight",m_labelHeight);
    yAxisJson.insert("DivNum",m_divNum);
    yAxisJson.insert("AutoHideCurve",m_autoHideCurve);
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
    int boundCount = m_yBoundVisbleList.count();
    for(int i = 0; i< boundCount; i++)
    {
        boundArray.append(m_yBoundVisbleList.at(i));
    }
    json.insert("BoundList",boundArray);
    json.insert("YAxisManager",yAxisJson);


}

void CYAxisManager::deserialize(const QJsonObject &json)
{
    clearItemList();
    QJsonObject jsonObject = json.value("YAxisManager").toObject();
    m_leftRect =CMDJsonConvert::toQRectF(jsonObject.value("LeftRect"));
    m_rightRect =CMDJsonConvert::toQRectF(jsonObject.value("RightLeft"));
    m_labelHeight = jsonObject.value("LabelHeight").toDouble();
    m_divNum = jsonObject.value("DivNum").toInt();
    m_autoHideCurve = jsonObject.value("AutoHideCurve").toBool();
    QJsonArray array = json.value("List").toArray();
    int count = array.count();
    for(int i =0; i<count; i++)
    {
        CYAxis *y = new CYAxis;
        QJsonObject itemJson;
        itemJson = array.at(i).toObject();
        y->deserialize(itemJson);
        m_list.append(y);
    }
    QJsonArray boundArray = json.value("BoundList").toArray();
    int boundCount = boundArray.count();
    for(int i = 0; i<boundCount; i++)
    {

        bool isBound = boundArray.at(i).toBool();
        m_yBoundVisbleList.append(isBound);
    }
}


CYAxisManager &CYAxisManager::operator =(const CYAxisManager &other)
{
    if(&other == this)
        return *this;
    this->m_leftRect = other.m_leftRect;
    this->m_rightRect = other.m_rightRect;
    this->m_labelHeight = other.m_labelHeight;
    this->m_divNum = other.m_divNum;
    this->m_autoHideCurve = other.m_autoHideCurve;

    clearItemList();

    for(int i = 0; i < other.m_list.count();i++){
        CYAxis *oY = other.m_list.at(i);
        CYAxis *tY = new CYAxis;
        *tY = *oY;
        m_list.append(tY);
    }
    for(int i = 0; i < other.m_yBoundVisbleList.count();i++){
        bool bound = other.m_yBoundVisbleList.at(i);
        m_yBoundVisbleList.append(bound);
    }

    return *this;
}


void CYAxisManager::clearItemList()
{
    if(!m_list.isEmpty())
        qDeleteAll(m_list.begin(),m_list.end());
    m_list.clear();

    m_yBoundVisbleList.clear();
}


