#include "CCurveProperty.h"

#include "CXAxisManager.h"
#include "CYAxisManager.h"

#include "CGridRegion.h"
#include "CDrawMode.h"
#include "CAppearance.h"

#include "CCurveObject.h"
#include "CDataProperty.h"
#include "CCurveObjectManager.h"
#include "CReferenceCurve.h"
#include "CReferenceCurveManager.h"
#include "CPreviewCurve.h"
#include "CPreviewCurveData.h"
#include "CLabelManager.h"
#include "CVernier.h"
#include "CVernierWidget.h"
#include "CAuxiliary.h"
#include "CTools.h"
#include "CToolButton.h"

#include "CHistoryTagManager.h"
#include "CRealTimeTagManager.h"
#include "CFeatureTag.h"

#include "CFind.h"

#include "IDrawEnvironment.h"
#include "MDDatabaseRecordset.h"
#include "IMDProjectExpert.h"
#include <QTimer>
#include <QJsonObject>
#include <QDebug>

const int Interval = 10;
CCurveProperty::CCurveProperty(QObject *parent):
    QObject(parent),
    m_isRealTime(true),
    m_appearance(new CAppearance),
    m_gridRegion(new CGridRegion),
    m_curveObjects(new CCurveObjectManager),
    m_referenceCurves(new CReferenceCurveManager),
    m_previewCurve(new CPreviewCurve),
    m_xsManager(new CXAxisManager),
    m_ysManager(new CYAxisManager),
    m_drawMode(new CDrawMode),
    m_data(new CDataProperty),
    m_label(new CLabelManager),
    m_vernier(new CVernier),
    m_vernierForm(new CVernierWidget),
    m_auxiliary(new CAuxiliary),
    m_historyTag(new CHistoryTagManager),
    m_realTag(new CRealTimeTagManager),
    m_tools(new CTools(this)),
    m_isCalculated(false),
    m_isStudioMode(true)

{

    m_curveObjects->setXs(m_xsManager);
    m_curveObjects->setYs(m_ysManager);
    m_curveObjects->setLabels(m_label);

    m_realTag->setXs(m_xsManager);
    m_historyTag->setXs(m_xsManager);

    m_referenceCurves->setXs(m_xsManager);
    m_referenceCurves->setYs(m_ysManager);
    m_referenceCurves->setLabels(m_label);

    m_data->setCurves(m_curveObjects);
    m_data->setReferenceCurves(m_referenceCurves);
    m_data->setHistoryTags(m_historyTag);//Mark
    m_data->setRealTags(m_realTag);

    m_previewCurve->setXManager(m_xsManager);

    m_tools->setManagers(m_xsManager,m_ysManager,m_curveObjects);
    m_tools->setPreviewCurve(m_previewCurve);
    connect(m_data,&CDataProperty::resultReady,this,&CCurveProperty::calculate);
    connect(m_data,&CDataProperty::resultReady,this,&CCurveProperty::calculatePreviewCurve);

    connect(m_tools,&CTools::run, m_drawMode,&CDrawMode::start);
    connect(m_tools,&CTools::stop,m_drawMode,&CDrawMode::stop);

    connect(m_drawMode,&CDrawMode::outOfTime,this,&CCurveProperty::refresh);
    connect(m_tools,&CTools::refresh,this,&CCurveProperty::refreshForTools);
    connect(m_previewCurve,&CPreviewCurve::updatePreviewArea,this,&CCurveProperty::refresh);
}

CCurveProperty::~CCurveProperty()
{
    if(m_label)
    {
        delete m_label;
        m_label = NULL;
    }
    if(m_curveObjects)
    {
        delete m_curveObjects;
        m_curveObjects = NULL;
    }
    if(m_referenceCurves)
    {
        delete m_referenceCurves;
        m_referenceCurves = NULL;
    }
    if(m_data)
    {
        delete m_data;
        m_data = NULL;
    }
    if(m_appearance)
    {
        delete m_appearance;
        m_appearance = NULL;
    }
    if(m_gridRegion)
    {
        delete m_gridRegion;
        m_gridRegion = NULL;
    }

    if(m_xsManager)
    {
        delete m_xsManager;
        m_xsManager = NULL;
    }
    if(m_ysManager)
    {
        delete m_ysManager;
        m_ysManager = NULL;
    }
    if(m_drawMode)
    {
        delete m_drawMode;
        m_drawMode = NULL;
    }

    if(m_vernier)
    {
        delete m_vernier;
        m_vernier = NULL;
    }
    if(m_vernierForm)
    {
        delete m_vernierForm;
        m_vernierForm = NULL;
    }

    if(m_auxiliary)
    {
        delete m_auxiliary;
        m_auxiliary = NULL;
    }
    if(m_historyTag)
    {
        delete m_historyTag;
        m_historyTag = NULL;
    }
    if(m_realTag){
        delete m_realTag;
        m_realTag = NULL;
    }
    if(m_tools)
    {
        delete m_tools;
        m_tools = NULL;
    }
    if(m_previewCurve)
    {
        delete m_previewCurve;
        m_previewCurve = NULL;
    }
}


QRectF CCurveProperty::drawArea() const
{
    return m_drawArea;
}
void CCurveProperty::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
}
QRectF CCurveProperty::curveArea() const
{
    return m_curveObjects->curveArea();
}

QRectF CCurveProperty::previewArea()
{
    return m_previewArea;
}
void CCurveProperty::setBorderLineColor(const QColor &color)
{
    m_gridRegion->setBorderColor(color);
}

/*!
\section functions
*/
void CCurveProperty::addXAxis(CXAxis *axis)
{
    m_xsManager->appendAxis(axis);
}

void CCurveProperty::addYAxis(CYAxis *axis)
{
    m_ysManager->appendCurve(axis);
}

void CCurveProperty::addCurve(CCurveObject *curve)
{
    m_curveObjects->appendCurve(curve);
}

void CCurveProperty::addReferenceCurve(CReferenceCurve *reCurve)
{
    m_referenceCurves->appendReferenceCurve(reCurve);
}

void CCurveProperty::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;

    m_tools->setIsRealTime(m_isRealTime);
    m_previewCurve->setIsReal(m_isRealTime);

    m_data->setIsRealTime(m_isRealTime);// set data state
    if(m_isRealTime)
        m_appearance->header()->setText(QObject::tr("实时曲线"));
    else
        m_appearance->header()->setText(QObject::tr("历史曲线"));
}
void CCurveProperty::setDrawEnvironment(IDrawEnvrionment *value)
{
    if(!value)
        return;
    m_isStudioMode = value->isStudioMode();
    m_data->setIsStudioModel(m_isStudioMode);
    // this func would be called after the setIsRealTime Func.

    m_historyTag->setIsStudioMode(m_isStudioMode);
    m_historyTag->setProjectPath(value->projectPath());
    m_realTag->setIsStudioMode(m_isStudioMode);// need to change
    m_tools->setIsStudioMode(m_isStudioMode);
    m_previewCurve->setIsStudioMode(m_isStudioMode);
    m_curveObjects->setProjectPath(value->projectPath());

    if(!m_isRealTime)
    {
        setEndTime(QDateTime::currentDateTime());
    }
    if(m_isStudioMode)
    {
        applyChanges();
    }
    else if( IMDProjectExpert *projectExpert = value->projectExpert())
    {
        IMDVariableArchivesManager* variantManager = projectExpert->variableArchivesManager();
        m_data->setData(variantManager);
        m_data->setData(value);

        setOrientation();
    }

    if(!m_isStudioMode && m_isRealTime)
    {
        m_tools->buttonRun(); // start run
    }
    else
    { // failed
        disconnect(m_drawMode,&CDrawMode::outOfTime,this,&CCurveProperty::refresh);
    }
}
void CCurveProperty::setCurrentMousePoint(const QPoint &point)
{
    setLabelPoint(point.x());
    showVernier(point);
    m_auxiliary->setVisble(true);
    m_auxiliary->setMousePointX(point.x());

    m_label->calculate();
    m_auxiliary->calculate();
}

void CCurveProperty::setParentWidget(QWidget *parent)
{
    m_vernierForm->setParent(parent);
    m_tools->setParentWidget(parent);
}

void CCurveProperty::setVernierClosed()
{
    m_vernierForm->close();
    m_auxiliary->setVisble(false);

}

void CCurveProperty::refresh()
{
    setXYPointers();
//    m_data->setFreshTime(m_drawMode->freshTime());
    m_data->getData();

    if(m_isRealTime)
    {
        setEndTime(QDateTime::currentDateTime());
        m_curveObjects->setType(m_isRealTime);
        m_curveObjects->setFresh(m_drawMode->freshTime());
        calculatePreviewCurve();
    }
    calculate();
    emit updateWidget();
}

void CCurveProperty::refreshForTools()
{
    setXYPointers();
//    m_data->setFreshTime(m_drawMode->freshTime());
    m_data->getData();

    if(m_isRealTime){
//        setEndTime(QDateTime::currentDateTime());
        m_curveObjects->setType(m_isRealTime);
        m_curveObjects->setFresh(m_drawMode->freshTime());
    }
    calculate();
    calculatePreviewCurve();

    emit updateWidget();
}

void CCurveProperty::applyChanges()
{
    setXYPointers();
//    m_data->setFreshTime(m_drawMode->freshTime());
    m_data->getData();
    //    setEndTime(QDateTime::currentDateTime());

    calculate();
    calculatePreviewCurve();
    changeLanguage();
}

/*!
\section js function
*/
void CCurveProperty::setHistoryDataSearchFilter(const QString &filter)
{
    m_data->setCurveDataSearchFilter(filter);
}

void CCurveProperty::setHistoryTagSearchFilter(const QString &filter)
{
    m_data->setTagDataSearchFilter(filter);
}

void CCurveProperty::query()
{
    m_data->query();
}

/*!
\section class points
*/
CAppearance *CCurveProperty::appearance() const
{
    return m_appearance;
}

CLabelManager *CCurveProperty::label() const
{
    return m_label;
}

CDataProperty *CCurveProperty::data() const
{
    return m_data;
}

CCurveObjectManager *CCurveProperty::curves() const
{
    return m_curveObjects;
}

CXAxisManager* CCurveProperty::xs() const
{
    return m_xsManager;
}

CYAxisManager* CCurveProperty::ys() const
{
    return m_ysManager;
}
CVernier *CCurveProperty::vernier() const
{
    return m_vernier;
}

CHistoryTagManager *CCurveProperty::historyTag() const
{
    return m_historyTag;//Mark
}

CGridRegion *CCurveProperty::gridRegion() const
{
    return m_gridRegion;
}

CDrawMode *CCurveProperty::drawMode() const
{
    return m_drawMode;
}

CRealTimeTagManager *CCurveProperty::realTag() const
{
    return m_realTag;
}

CTools *CCurveProperty::tools() const
{
    return m_tools;
}

CReferenceCurveManager *CCurveProperty::referenceCurves() const
{
    return m_referenceCurves;
}

CPreviewCurve *CCurveProperty::previewCurve() const
{
    return m_previewCurve;
}

void CCurveProperty::setIsRun(bool isRun)
{
    for(int i = 0; i < m_curveObjects->list().count();i++){
        CCurveObject *curve = m_curveObjects->list().at(i);
        curve->setIsRun(isRun);
    }
}

void CCurveProperty::showVernier(const QPoint &point)
{
    if(m_vernier->display())
    {
        m_vernier->setMoursePressX(point.x());
        m_vernier->calculate();
        m_vernierForm->setWidgetSize(m_vernier->widgetSize());
        m_vernierForm->setLabelList(m_vernier->labelList());
        if(m_vernier->isFixed())
            m_vernierForm->move(m_vernier->fixedPoint());

        else
            m_vernierForm->move(point);
        m_vernierForm->show();
    }
}

void CCurveProperty::changeLanguage()
{
    m_appearance->changeLanguage();
    if(m_isRealTime)
    {
        m_realTag->changeLanguage();
    }
    else
    {
        m_historyTag->changeLanguage();
    }

}

void CCurveProperty::collectWords()
{
    m_appearance->collectWords();
    if(m_isRealTime)
    {
        m_realTag->collectWords();
    }
    else
    {
        m_historyTag->collectWords();
    }
}

void CCurveProperty::setStartMove(const double &dx)
{
    m_previewCurve->startMove(dx);
}

void CCurveProperty::setEndMove(const double &dx)
{
    m_previewCurve->endMove(dx);
}

void CCurveProperty::setDragMove(const double &dx)
{
    m_previewCurve->dragMove(dx);
}

QRectF CCurveProperty::startMoveRect() const
{
    return m_previewCurve->startMoveRect();
}

QRectF CCurveProperty::endMoveRect() const
{
    return m_previewCurve->endMoveRect();
}

QRectF CCurveProperty::dragMoveRect() const
{
    return m_previewCurve->dragMoveRect();
}

QRectF CCurveProperty::startRollRect() const
{
    return m_previewCurve->startRollMoveRect();
}

QRectF CCurveProperty::endRollRect() const
{
    return m_previewCurve->endRollMoveRect();
}

void CCurveProperty::setPreviewMoveLength(const double &dx)
{
    m_previewCurve->setMoveLength(dx);
}
/*!
\section actions
*/
void CCurveProperty::move(const double &length)
{
    for(int i = 0; i < m_xsManager->list().count(); i++)
    {
        CXAxis *x = m_xsManager->list().at(i);
//        double  second = x->valueLength()/ m_gridRegion->gridRegionArea().width() *length;
        double second = length / m_gridRegion->gridRegionArea().width();
        double start = x->startScale() - second;
        double end = x->endScale() - second;

        if(CXAxis::ValueType::Time == x->valueType())
        {
            QDateTime  newEndTime = x->endTime();
            if(m_isRealTime)
            {
                if(m_drawMode->orietation() == CDrawMode::Orientation::RigthToLeft)
                {
                    if(newEndTime < QDateTime::currentDateTime())
                    {
                        if(start < 0)
                        {
                            start = 0;
                            end = x->endScale() - x->startScale();
                        }

                        if(end > 1)
                        {
                            end = 1;
                            start = 1 - (x->endScale() - x->startScale());
                        }

                        x->setScale(start ,end);
                    }
                    if(newEndTime == QDateTime::currentDateTime() && second > 0)
                    {
                        if(start < 0)
                        {
                            start = 0;
                            end = x->endScale() - x->startScale();
                        }

                        if(end > 1)
                        {
                            end = 1;
                            start = 1 - (x->endScale() - x->startScale());
                        }

                        x->setScale(start,end);
                    }
                }
                else
                {
                    if(newEndTime > QDateTime::currentDateTime() && newEndTime < x->originEndTime())
                    {
                        if(start < 0)
                        {
                            start = 0;
                            end = x->endScale() - x->startScale();
                        }

                        if(end > 1)
                        {
                            end = 1;
                            start = 1 - (x->endScale() - x->startScale());
                        }

                        x->setScale(start,end);
                    }
                    if(newEndTime > QDateTime::currentDateTime() && second > 0)
                    {
                        if(start < 0)
                        {
                            start = 0;
                            end = x->endScale() - x->startScale();
                        }

                        if(end > 1)
                        {
                            end = 1;
                            start = 1 - (x->endScale() - x->startScale());
                        }

                        x->setScale(start,end);
                    }
                    if(newEndTime < QDateTime::currentDateTime())
                    {
                        if(start < 0)
                        {
                             start = 0;
                             end = x->endScale() - x->startScale();
                        }

                        if(end > 1)
                        {
                             end = 1;
                             start = 1 - (x->endScale() - x->startScale());
                        }

                        x->setScale(start,end);
                    }
                }
            }
            else
            {
                if(newEndTime <= x->originEndTime())
                {
                    if(start < 0)
                    {
                        start = 0;
                        end = x->endScale() - x->startScale();
                    }

                    if(end > 1)
                    {
                         end = 1;
                         start = 1 - (x->endScale() - x->startScale());
                    }

                    x->setScale(start,end);
                }
                if(newEndTime >= x->originEndTime() && second >0)
                {
                    if(start < 0)
                    {
                        start = 0;
                        end = x->endScale() - x->startScale();
                    }

                    if(end > 1)
                    {
                        start = 1 - (x->endScale() - x->startScale());
                        end = 1;
                    }

                    x->setScale(start,end);
                }
            }
        }
        else
        {
            double  newEnd = x->maxValue();
            if(newEnd < x->originMax())
            {
                if(start < 0)
                {
                     start = 0;
                     end = x->endScale() - x->startScale();
                }

                if(end > 1)
                {
                    end = 1;
                    start = 1 - (x->endScale() - x->startScale());
                }

                x->setScale(start,end);
            }
            if(newEnd == x->originMax() && second >0)
            {
                if(start < 0)
                {
                    start = 0;
                    end = x->endScale() - x->startScale();
                }

                if(end > 1)
                {
                    end = 1;
                    start = 1 - (x->endScale() - x->startScale());
                }

                x->setScale(start,end);
            }
        }
        m_previewCurve->setScale(start,end);
    }

    for(int i = 0;i< curves()->list().count();i++)
    {
        CCurveObject *curve = curves()->list().at(i);
        curve->clearDataForZoom();
    }
//    m_data->setFreshTime(m_drawMode->freshTime());
    m_data->getData();
    calculate();
    calculatePreviewCurve();
}

void CCurveProperty::moveHeight(const double &height)
{
    for(int i = 0; i < m_ysManager->list().count(); i++)
    {
        CYAxis *y = m_ysManager->list().at(i);
//        double  second = (y->maxValue()-y->minValue())/ m_gridRegion->gridRegionArea().height() *height;
        double second = height / m_gridRegion->gridRegionArea().height();
        double start = y->startScale() - second;
        double end = y->endScale() - second;
        if(y->maxValue() == y->originMax() && y->minValue() == y->originMin())
            continue;
        if((y->maxValue() >= y->originMax() && second <0) || (y->minValue() <= y->originMin() && second > 0))
            continue;

        if(start < 0 )
        {
            start = 0;
            end = y->endScale() - y->startScale();
        }
        if(end > 1)
        {
            end = 1;
            start =1 - (y->endScale() - y->startScale());
        }
        y->setScale(start,end);
    }
//    m_data->setFreshTime(m_drawMode->freshTime());
    m_data->getData();
    calculate();
}

void CCurveProperty::zoomIn(const QRect &rect,const QRect &curve)
{
    m_tools->buttonSelectZoom(rect,curve);
}

void CCurveProperty::buttonRun()
{
    m_tools->buttonRun();
}

bool CCurveProperty::buttonZoomIn()
{
    return m_tools->buttonZoomIn();
}

bool CCurveProperty::buttonZoomOut()
{
    return m_tools->buttonZoomOut();
}
bool CCurveProperty::buttonNormal()
{
    return m_tools->buttonNormal();
}

bool CCurveProperty::buttonSelectZoomIn()
{
    return m_tools->button(CTools::NameId::ZoomSelect)->isChecked();
}

bool CCurveProperty::isRun() const
{
    return m_tools->isRun();
}

bool CCurveProperty::isMoveButtonChecked() const
{
    return m_tools->button(CTools::NameId::Move)->isChecked();
}

bool CCurveProperty::isZoomSelectChecked() const
{
    return m_tools->button(CTools::NameId::ZoomSelect)->isChecked();
}

bool CCurveProperty::isStudioMode() const
{
    return m_isStudioMode;
}

/*!
\section calculate
*/
void CCurveProperty::calculate()
{
    setXYPointers();
    m_data->setFreshTime(m_drawMode->freshTime());
    m_data->getData();
    QRectF rect = m_drawArea;

    //! previewCurve
    if(m_previewCurve->previewData()->isPreviewVisible())
    {
        rect.adjust(0,0,0,-60);
    }

    QRectF appearanceArea = rect;
    rect.adjust(Interval,Interval,-Interval,-Interval);

    if(m_isStudioMode)
    {
        m_tools->calculate();
    }

    rect = QRectF(rect.x(),rect.y()+m_tools->buttonSize().height(),rect.width(),rect.height()-m_tools->buttonSize().height());
    QRectF r = rect;

   //! label
    m_label->setIsStudioMode(m_isStudioMode);

    QPointF p = QPointF(r.x(),r.y()-5);
    r.setTopLeft(p);

    m_label->setDrawArea(r);
    m_label->setCurves(m_curveObjects->list());
    m_label->calculate(); // should move to the top

    double topHeight = m_label->height();
    double leftWidth = m_ysManager->width(true);
    double rightWidth = m_ysManager->width(false);
    double bottomHeight = m_xsManager->height();
    double yLabelHeight = m_ysManager->labelHeight();
    double yHeight = rect.height() - topHeight - bottomHeight;

    // y
    r.moveTop(r.top() + topHeight +3);
    r.setHeight(yHeight -3);
    QRectF leftRect = QRectF(r.left(), r.top(), leftWidth, r.height());
    QRectF rightRect = QRectF(r.right()-rightWidth, r.top(), rightWidth, r.height());
    m_ysManager->setRect(leftRect, rightRect);
    m_ysManager->calculate();

    //x
    r = rect;

    qreal height2 = r.height() - 5;
    r.setHeight(height2);

    QRectF xRect = QRectF(r.left()+leftWidth, r.bottom()-bottomHeight, r.width()-leftWidth-rightWidth+Interval, bottomHeight);
    m_xsManager->setDrawArea(xRect);
    m_xsManager->setOrientation(m_drawMode->orietation());
    m_xsManager->calculate();

    // Curve
    r = rect;
    QRectF curveRect = QRectF(xRect.left(), leftRect.top()+yLabelHeight, xRect.width() - Interval, leftRect.height()-yLabelHeight);
    m_curveObjects->setCurveArea(curveRect);
    m_curveObjects->calculate();
    m_previewCurve->setCurveArea(curveRect);

    //referenceCurve
    m_referenceCurves->setReferenceCurveArea(curveRect);
    m_referenceCurves->calculate();

    // Vernier
    m_vernier->setIsStudioMode(m_isStudioMode);
    m_vernier->setList(m_curveObjects->list());
    m_vernier->setReferenceList(m_referenceCurves->list());
    m_vernier->calculate();

    //VerinerForm
    m_vernierForm->setFont(m_vernier->font());
    m_vernierForm->setWidgetSize(m_vernier->widgetSize());
    m_vernierForm->setLabelList(m_vernier->labelList());

    // Feature Tag

    CTagManager *tag = NULL;
    if(m_isRealTime)
    {
        tag = m_realTag;
        tag->setXs(m_xsManager);
    }
    else
    {
        tag = m_historyTag;
        tag->setXs(m_xsManager);
    }
    tag->setDrawArea(curveRect);
    tag->calculate();


    // GridRegion
    m_gridRegion->setGridRegionArea(curveRect);
    m_gridRegion->calculate();

    // Appearance
    enum {Margin = 70};
    r = /*m_drawArea*/appearanceArea;
    r.moveTop(-Margin/2);
    r.setHeight(r.height()+Margin/2);
    m_appearance->setWidgetArea(r);
    m_appearance->setCurveArea(curveRect);
    m_appearance->calculate();

    // Auxiliary
    if(!m_isStudioMode)
    {
        m_auxiliary->setCurves(m_curveObjects);
        m_auxiliary->setReCurves(m_referenceCurves);
        m_auxiliary->calculate();
    }

    m_isCalculated = true;
    emit updateWidget();
}

void CCurveProperty::calculatePreviewCurve()
{
    //! previewCurve
    QRectF rect = m_drawArea;
    if(m_previewCurve->previewData()->isPreviewVisible())
    {
        m_previewArea = QRectF(rect.x(),rect.y()+rect.height()-60,rect.width(),60);
        m_previewArea.adjust(1,0,-1,0);
        m_previewCurve->setPreviewArea(m_previewArea);
        rect.adjust(0,0,0,-60);
        m_previewCurve->setCurves(m_curveObjects);
        m_previewCurve->setXManager(m_xsManager);
        m_previewCurve->calculate();
    }
}

void CCurveProperty::draw(QPainter *p,const QRect &rect)
{
    if(!m_isCalculated)
        return;
    m_appearance->draw(p,rect);

    m_label->draw(p);
    m_xsManager->draw(p);
    m_ysManager->draw(p);
    m_gridRegion->draw(p);
    m_referenceCurves->draw(p);
    m_curveObjects->draw(p);
    if(m_isRealTime)
        m_realTag->draw(p);
    else
        m_historyTag->draw(p);


    m_auxiliary->draw(p);
    m_previewCurve->draw(p);
}

/*!
\section serialize
*/

void CCurveProperty::serialize(QJsonObject &json)
{
    json.insert("IsRealTime",m_isRealTime);
    QJsonObject appearanceJson;
    m_appearance->serialize(appearanceJson);
    json.insert("Appearance",appearanceJson);
    QJsonObject labelJson;
    m_label->serialize(labelJson);
    json.insert("Label",labelJson);
    QJsonObject xManagerJson;
    m_xsManager->serialize(xManagerJson);
    json.insert("XManager",xManagerJson);
    QJsonObject yManagerJson;
    m_ysManager->serialize(yManagerJson);
    json.insert("YManager",yManagerJson);
    QJsonObject gridRegionJson;
    m_gridRegion->serialize(gridRegionJson);
    json.insert("GridRegion",gridRegionJson);
    QJsonObject curveJson;
    m_curveObjects->serialize(curveJson);
    json.insert("CurveObjects",curveJson);
    if(!m_isRealTime)
    {
        QJsonObject historyJson;
        m_historyTag->serialize(historyJson);
        json.insert("HistoryTag",historyJson);
    }
    else
    {
        QJsonObject realJson;
        m_realTag->serialize(realJson);
        json.insert("RealTag",realJson);
    }
    QJsonObject vernierJson;
    m_vernier->serialize(vernierJson);
    json.insert("Vernier",vernierJson);
    QJsonObject drawModeJson;
    m_drawMode->serialize(drawModeJson);
    json.insert("DrawMode",drawModeJson);
    QJsonObject toolsJson;
    m_tools->serialize(toolsJson);
    json.insert("Tools",toolsJson);
    QJsonObject dataJson;
    m_data->serialize(dataJson);
    json.insert("Data",dataJson);
    QJsonObject referenceJson;
    m_referenceCurves->serialize(referenceJson);
    json.insert("ReferenceCurve",referenceJson);
    QJsonObject previewJson;
    m_previewCurve->serialize(previewJson);
    json.insert("PreviewCurve",previewJson);
}

void CCurveProperty::deserialize(const QJsonObject &json)
{
    m_isRealTime = json.value("IsRealTime").toBool();
    QJsonObject appearanceJson = json.value("Appearance").toObject();
    m_appearance->deserialize(appearanceJson);
    QJsonObject labelJson = json.value("Label").toObject();
    m_label->deserialize(labelJson);
    QJsonObject xManagerJson = json.value("XManager").toObject();
    m_xsManager->deserialize(xManagerJson);
    QJsonObject yManagerJson = json.value("YManager").toObject();
    m_ysManager->deserialize(yManagerJson);
    QJsonObject gridRegionJson = json.value("GridRegion").toObject();
    m_gridRegion->deserialize(gridRegionJson);
    QJsonObject curveJson = json.value("CurveObjects").toObject();
    m_curveObjects->deserialize(curveJson);

    if(!m_isRealTime)
    {
        QJsonObject historyJson = json.value("HistoryTag").toObject();
        m_historyTag->deserialize(historyJson);
    }
    else
    {
        QJsonObject realJson = json.value("RealTag").toObject();
        m_realTag->deserialize(realJson);
    }
    QJsonObject vernierJson = json.value("Vernier").toObject();
    m_vernier->deserialize(vernierJson);
    QJsonObject drawModeJson = json.value("DrawMode").toObject();
    m_drawMode->deserialize(drawModeJson);
    QJsonObject toolsJson = json.value("Tools").toObject();
    m_tools->deserialize(toolsJson);
    QJsonObject dataJson = json.value("Data").toObject();
    m_data->deserialize(dataJson);
    QJsonObject referenceJson = json.value("ReferenceCurve").toObject();
    m_referenceCurves->deserialize(referenceJson);
    QJsonObject previewJson = json.value("PreviewCurve").toObject();
    m_previewCurve->deserialize(previewJson);

}

CCurveProperty *CCurveProperty::clone(QWidget *parent)
{
    CCurveProperty *curve = new CCurveProperty;

//    curve->m_curveObjects->setLabels(curve->m_label);

    curve->m_isRealTime = this->m_isRealTime;

    *curve->m_appearance=  *this->m_appearance ;
    *curve->m_gridRegion= *this->m_gridRegion ;
    *curve->m_curveObjects =*this->m_curveObjects;
    *curve->m_referenceCurves = *this->m_referenceCurves;
    *curve->m_previewCurve = *this->m_previewCurve;
    *curve->m_xsManager= *this->m_xsManager ;
    *curve->m_ysManager= *this->m_ysManager ;

    delete curve->m_drawMode;
    delete curve->m_data;
    curve->m_drawMode = this->m_drawMode->clone();
    curve->m_data= this->m_data->clone() ;

    *curve->m_label= *this->m_label  ;
    *curve->m_vernier= *this->m_vernier ;
    *curve->m_auxiliary= *this->m_auxiliary ;

    *curve->m_historyTag = *this->m_historyTag;
    *curve->m_realTag = *this->m_realTag;

    delete curve->m_tools;
    curve->m_tools = this->m_tools->clone(parent);

    delete curve->m_vernierForm;
    curve->m_vernierForm =this->m_vernierForm->clone();
    curve->m_vernierForm->setParent(parent);//Mark set the parent Widget
    curve->m_drawArea = this->m_drawArea;

//    curve->m_curveObjects->setXs(curve->m_xsManager);
//    curve->m_curveObjects->setYs(curve->m_ysManager);

    curve->m_data->setCurves(curve->m_curveObjects);

    curve->m_data->setHistoryTags(curve->m_historyTag);
    curve->m_data->setRealTags(curve->m_realTag);

    curve->m_tools->setManagers(m_xsManager,m_ysManager,m_curveObjects);
    return curve;
}

/*!
\section inside tools functions
*/
void CCurveProperty::setXYPointers()
{
    m_curveObjects->setXY();
    m_realTag->setXList();
    m_historyTag->setXList();
    m_referenceCurves->setXY();
}
void CCurveProperty::setEndTime(const QDateTime &time)
{
    m_xsManager->setEndTime(time,m_isStudioMode);
}

void CCurveProperty::setOrientation()
{
    m_xsManager->setOrientationOnRun();
}

void CCurveProperty::setLabelPoint(double x)
{
    if(m_label->movingValueVisible() && m_label->lableVisible())
        m_label->setMoursePressX(x);
}
