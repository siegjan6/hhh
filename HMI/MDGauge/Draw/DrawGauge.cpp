#include <QList>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include "DrawGauge.h"
#include "DesignDialog.h"
#include "MDMultiLanguage.h"

CDrawGauge::CDrawGauge()
{
    m_redraw = true;
    m_proportion = 1;
    m_isLoad =true;
    m_count = 0;
    m_bhasNeedle = false;
    m_environment = NULL;
    m_initialIndex = 0;
    m_parent = NULL;
    initialize();
}
CDrawGauge::~CDrawGauge()
{
    if (!m_layer.isEmpty())
    {
        for (qint32 i = 0; i < m_layer.count(); i++)
            delete m_layer.at(i);
        m_layer.clear();
    }
    if (!m_valueList.isEmpty())
    {
        for (qint32 i = 0; i < m_valueList.count(); i++)
            delete m_valueList.at(i);
        m_valueList.clear();
    }
    if (!m_dataValueList.isEmpty())
    {
//        for (qint32 i = 0; i < m_dataValueList.count(); i++)
//            delete m_dataValueList.at(i);
        m_dataValueList.clear();
    }
}

CDrawGauge *CDrawGauge::clone() const
{
    CDrawGauge *gauge= new CDrawGauge();
    *gauge = *this;
    return gauge;
}

CDrawGauge &CDrawGauge::operator=(const CDrawGauge &other)
{
    if(this == &other)
    {
        return *this;
    }
    CDrawGauge* d = (CDrawGauge*)&other;
    this->m_environment = d->m_environment;
    this->m_proportion = d->m_proportion;
    this->m_redraw = d->m_redraw;
    this->m_bhasNeedle = d->m_bhasNeedle;
    this->m_count = d->m_count;
    this->m_drawItem = d->m_drawItem;
    this->m_initialIndex = d->m_initialIndex;
    this->m_isLoad = d->m_isLoad;
    this->m_moveRgn = d->m_moveRgn;
    this->m_rect = d->m_rect;
    this->m_rgn = d->m_rgn;

    if(!this->m_layer.isEmpty())
    {
        for(qint32 i = 0;i<m_layer.count();i++)
            delete m_layer.at(i);
        m_layer.clear();
    }

    CDataLayer* pLayer = NULL;
    CDataLayer* pNewLayer = NULL;
    qint32 count = d->m_layer.count();
    for(qint32 i = count-1;i>=0;i--)
    {
          pLayer = d->m_layer.at(i);
          pNewLayer = new CDataLayer();
          *pNewLayer = *pLayer;
          this->m_layer.prepend(pNewLayer);
      }

    return *this;
}

void CDrawGauge::serialize(QJsonObject &json)
{
    QJsonObject drawGaugeJson;
    if(m_layer.count() == 0)
        return;
    int count = m_layer.count();
    QJsonArray array;
    for(int i = 0; i < count; i++)
    {
        QJsonObject jsonLayer;
        m_layer.at(i)->serialize(jsonLayer);
        array.append(jsonLayer);
    }
    drawGaugeJson.insert("Layers", array);
    json.insert("DrawGauge",drawGaugeJson);
}

void CDrawGauge::deserialize(const QJsonObject &json)
{
    while(!m_layer.isEmpty())
    {
        for (qint32 i = 0; i < m_layer.count(); i++)
            delete m_layer.at(i);
        m_layer.clear();
    }
    QJsonObject drawGaugeJson = json.value("DrawGauge").toObject();
    QJsonArray array = drawGaugeJson.value("Layers").toArray();
    int count = array.count();
    CDataLayer* pLayer = NULL;
    for(int i = 0; i < count; i++)
    {
        pLayer = new CDataLayer();
        QJsonObject jsonLayer = array.at(i).toObject();
        pLayer->deserialize(jsonLayer);
        m_layer.append(pLayer);
    }
}

void CDrawGauge::draw(QPainter *paint)
{
    paint->setClipRect(m_rect, Qt::IntersectClip);
    draw(paint, m_rect);
}
void CDrawGauge::draw(QPainter *paint, const QRect &rect, QList<CDataLayer *> *pLayerList, bool isSquare)
{
    QRect r = rect;
    if (pLayerList == NULL)		//!   运行界面
    {
        if (paint->isActive())
        {
            CDataLayer* pLayer = NULL;
            qint32 count = m_layer.count();
            for (qint32 i = count -1; i >= 0; i--)
            {
                pLayer = m_layer.at(i);
                drawLayer(paint, r, pLayer, NULL, 4);
            }
            m_redraw = true;
        }
        else
        {
            if (m_redraw)
            {

                CDataLayer* pLayer = NULL;
                qint32 count = m_layer.count();
                for(qint32 i = count-1;i>=0;i--)
                {
                    pLayer = m_layer.at(i);
                    drawLayer(paint, r, pLayer, NULL, 1);
                }

                m_redraw = false;
            }

            //!   绘制指针
            paint->setClipRect(rect);
            //!  获取指针绘制区域
            CDataLayer* pLayer = NULL;
            qint32 count1 = m_layer.count();
            for(qint32 i = count1-1;i>=0;i--)
            {
                pLayer = m_layer.at(i);
                drawLayer(paint, r, pLayer, NULL, 2);
            }
            m_moveRgn.intersected(rect);
            paint->setClipRegion(m_moveRgn);

            m_bhasNeedle = false;
            //!  绘制指针
            for(qint32 i = count1-1;i>=0;i--)
            {
                pLayer = m_layer.at(i);
                drawLayer(paint, r, pLayer, NULL, 3);
            }
        }
        m_isLoad = false;
    }else		//!   配置界面
    {

        paint->setClipRect(rect);
        QRect rp = rect;
        if (!isSquare)
        {
            rp = getProportionRect(rect);
            paint->setClipRect(rp);
        }

        CDataLayer* pLayer = NULL;
        qint32 count = pLayerList->count();
        for(qint32 i = count-1;i>=0;i--)
        {
            pLayer = pLayerList->at(i);
            drawLayer(paint, rp, pLayer,NULL);
        }
    }
}

void CDrawGauge::draw(QPainter *paint, const QRect &rect, CData *data, QList<CDataLayer *> *pLayerList)
{
    QPainter* g;
    g = paint;
    g->setClipRect(rect);

    QRect rp = getProportionRect(rect);
    g->setClipRect(rp);

    QList<CDataLayer*>* pList = (pLayerList != NULL) ? pLayerList : &m_layer;
    CDataLayer* pLayer = NULL;
    qint32 count = pList->count();

    if (data->getDrawType() == dtLayer)
        drawLayer(g, rp, (CDataLayer*)data, NULL);
    else
    {
        for(qint32 i = count-1;i>=0;i--)
        {
            pLayer = pList->at(i);
            if (pLayer->m_item.contains(data))
            {
                drawLayer(g, rp,pLayer,data);
                return;
            }
        }
    }
}

bool CDrawGauge::openDesign()
{
    CDesignDialog design(dynamic_cast<QWidget*>(m_parent));
    design.m_gauge = this;
    design.onInitDialog();

    if (design.exec() == QDialog::Accepted)
        return true;
    return false;
}
void CDrawGauge::initialize()
{
    CDataCap* datacap = new CDataCap;
    CDataNeedle* dataneedle = new CDataNeedle;
    CDataRange* datarange = new CDataRange;
    CDataScale*  datascale = new CDataScale;
    CDataScale* datascaleSub = new CDataScale;
    CDataScaleBack* datascaleback = new CDataScaleBack;
    CDataBack*  databack = new CDataBack;
    CDataScaleText*  datascaletext = new CDataScaleText;

    datascaleSub->m_isSubScale = true;
    datascaleSub->setWidth(2);
    datascaleSub->setLen(4);
    CDataLayer* datalay = new CDataLayer();
    datalay->m_item.append(datacap);
    datalay->m_item.append(dataneedle);
    datalay->m_item.append(datarange);
    datalay->m_item.append(datascale);
    datalay->m_item.append(datascaleSub);
    datalay->m_item.append(datascaletext);
    datalay->m_item.append(datascaleback);
    datalay->m_item.append(databack);
    m_layer.append(datalay);

//        CRulerNeedle* rneedle = new CRulerNeedle;
//        CRulerMarker* rmarker = new CRulerMarker;
//        CRulerRange* rrange = new CRulerRange;
//        CRulerScaleText* rScaleText = new CRulerScaleText;
//        CRulerScale* rscale = new CRulerScale;
//        CRulerScaleBack* rscaleBack = new CRulerScaleBack;

//        CDataLayer* datalay1 = new CDataLayer();
//        datalay1->m_item.append(rScaleText);
//        datalay1->m_item.append(rneedle);
//        datalay1->m_item.append(rmarker);
//        datalay1->m_item.append(rscale);
//        datalay1->m_item.append(rrange);
//        datalay1->m_item.append(rscaleBack);
    //        m_layer.append(datalay1);
}

void CDrawGauge::setDrawEnvrionment(IDrawEnvrionment *value)
{
    m_environment = value;
}

QRect CDrawGauge::regionRect()
{
    return m_rect;
}

void CDrawGauge::setParent(IHMIForm *parent)
{
    m_parent = parent;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDrawGauge::drawLayer(QPainter* painter, const QRect& rect, CDataLayer* layer,CData* data, qint32 drawMode)
{
    QRectF r = getLayerRectF(rect, layer);
    m_drawItem.setLayer(layer);

    //!  获取非正方形坐标数据，主要用于直表组件
    QRectF rAct = getLayerRectF(rect, layer, false);
    m_drawItem.setRectAct(rAct);

    //!  设置中心点为原点
    qreal x = r.x() + r.width() / 2;
    qreal y = r.y()+ r.height() / 2;

    painter->translate(x, y);
    if (data != NULL)
    {
        m_drawItem.draw(painter, r, data);
    }
    else
    {
        CData* pData = NULL;
        qint32 count = layer->m_item.count();
        for(qint32 i = count-1;i>=0;i--)
        {
            pData = layer->m_item.at(i);
            drawType type = pData->getDrawType();
            bool bNeedle = (type == dtNeedle || type == dtRNeedle || type == dtRMarker || type == dtTemperature);

            switch(drawMode) {
            case 0:
            case 4:
                m_drawItem.draw(painter, r, pData);
                break;
            case 1:
                if (!bNeedle && type != dtLed)
                    m_drawItem.draw(painter, r, pData);
                break;
            case 2:
                if (bNeedle || type == dtLed)
                {
                    QTransform m;
                    m.translate(x, y);
                    m_drawItem.draw(painter, r, pData, &m_moveRgn, &m);
                }
                break;
            case 3:
                if (bNeedle)
                    m_bhasNeedle = true;

                if (m_bhasNeedle)
                    m_drawItem.draw(painter, r, pData);
                break;
            default:
                break;
            }
        }
    }

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::black,3));
    painter->translate(-x, -y);
}
QRect CDrawGauge::getRect(const QRect& rect)
{
    QRect r = rect;
    qint32 x = r.x();
    qint32 y = r.y();
    qint32 w = r.width();
    qint32 h = r.height();
    //! 非正方形的情况 rect
    if (w > h)
    {
        x += (w - h) / 2;
        w = h;
    }
    else if (w < h)
    {
        y += (h - w) / 2;
        h = w;
    }
    r = QRect(x,y,w,h);
    return r;
}

bool CDrawGauge::needRefresh()
{
    bool refresh = false;

    CDataValue* pValue = NULL;
    qint32 count = m_valueList.count();
    for(qint32 i = count-1;i>=0;i--)
    {
        pValue = m_valueList.at(i);
        drawType type = pValue->getDrawType();
        if (type == dtNeedle || type == dtRNeedle || type == dtRMarker || type == dtTemperature)
        {
            CDataValue* n = (CDataValue*)pValue;
            if (n->m_currentStep > 0)
            {
                n->m_currentStep--;
                refresh = true;
            }
            else
                n->m_currentStep = 0;
        }
    }

    return refresh;
}

void CDrawGauge::setClientRect(const QRect &rcClient)
{

    m_rect = rcClient;
    m_rgn.setRects(&rcClient,1);
    m_proportion = getProportion();
}

QRegion *CDrawGauge::getRegion()
{
    return &m_rgn;
}

QRectF CDrawGauge::getLayerRectF(const QRect& rect, CDataLayer* layer, bool isSquare)
{
    QRectF lr = layer->getRectF();

    //!  获取实际范围
    QRectF rf;
    qreal x = rf.x();
    qreal y = rf.y();
    qreal w = rf.width();
    qreal h = rf.height();
    x = rect.width() * lr.x() / 100 + rect.x();
    y = rect.height() * lr.y() / 100 + rect.y();
    w = rect.width() * lr.width() / 100;
    h = rect.height() * lr.height() / 100;
    if (isSquare)
    {
        //!  非正方形的情况
        if (w > h)
        {
            x += (w - h) / 2;
            w = h;
        }
        else if (w < h)
        {
            y += (h - w) / 2;
            h = w;
        }
    }
    rf = QRectF(x,y,w,h);

    return rf;
}
QRect CDrawGauge::getProportionRect(const QRect& rect)
{
    QRect r = rect;
    qint32 x = r.x();
    qint32 y = r.y();
    qint32 w = r.width();
    qint32 h = r.height();
    if (m_proportion > 1)		//! width > height
    {
        h = (qint32)(rect.height() / m_proportion);
        y += (rect.height() - h) / 2;
    }
    else						//!  width <= height
    {
        w = (qint32)(rect.width() * m_proportion);
        x += (rect.width() - w) / 2;
    }
    r = QRect(x,y,w,h);
    return r;
}

double CDrawGauge::getProportion()
{
    double i = 0;
    double max = 10;
    double min = 1 / max;
    double width = m_rect.width();
    double height = m_rect.height();

    if (m_rect.height() > 0)
        i = width / height;
    i = (i < max) ? i : max;
    i = (i > min) ? i : min;

    return i;
}

void CDrawGauge::setValue(qint32 index, qreal value)
{
    for (qint32 i = 0; i < m_dataValueList.count(); i++)
    {
        CDataValue* v = m_dataValueList.at(i);
        if (v->m_index == index)
        {
            if (v->m_value != value)
            {
                v->m_value = value;
                m_environment->update(m_rect);
            }
            return;
        }
    }
}

void CDrawGauge::setInitialIndex(qint32 value)
{
    m_initialIndex = value;
}
void CDrawGauge::getExpressionList(QList<qint32> &indexs, QList<QString> &expressions)
{
    qint32 index = m_initialIndex;

    for (qint32 i = 0; i < m_layer.count(); i++)
    {
        CDataLayer* layer = m_layer.at(i);

        for (qint32 j = 0; j < layer->m_item.count(); j++)
        {
            CData* d = layer->m_item.at(j);
            drawType type = d->getDrawType();

            if (type == dtNeedle ||type == dtLed ||type == dtRNeedle||type == dtRMarker||type == dtTemperature)
            {
                CDataValue* v = static_cast<CDataValue*>(d);
                if (v->m_expression.isEmpty())
                    continue;
                indexs.append(index);
                expressions.append(v->m_expression);
                v->m_index = index;
                index++;
                m_dataValueList.append(v);
            }
        }
    }
}

void CDrawGauge::changeLanguage()
{
    for(int i = 0;i<m_layer.count();i++)
    {
        for(int j = 0;j<m_layer.at(i)->m_item.count();j++)
        {
            if(m_layer.at(i)->m_item.at(j)->getDrawType() == dtLabel )
            {
                CDataLabel* label = (CDataLabel*)m_layer.at(i)->m_item.at(j);
                label->m_showText = CMDMultiLanguage::instance()->value(label->m_text, false);
            }
        }
    }
}

void CDrawGauge::collectWords()
{
    for(int i = 0;i<m_layer.count();i++)
    {
        for(int j = 0;j<m_layer.at(i)->m_item.count();j++)
        {
            if(m_layer.at(i)->m_item.at(j)->getDrawType() == dtLabel )
            {
                CDataLabel* label = (CDataLabel*)m_layer.at(i)->m_item.at(j);
                 CMDMultiLanguage::instance()->addWord(label->m_text, false);
            }
        }
    }
}


