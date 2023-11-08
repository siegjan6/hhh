#include "CCurveWidget.h"

#include "CCurveProperty.h"
#include "MDCurveSettingDialog.h"
#include "IDrawEnvironment.h"
#include "IMDDatabaseParam.h"
//#include "MDDataSourceFactory.h"

//#include "MDDataQueryUI.h"
#include "CAppearance.h"
#include "CPreviewCurve.h"
#include "CReferenceCurveManager.h"

#include "MDDBQueryEngineFactory.h"
#include "MDDBQueryUI.h"
//#include "MDDBQueryUI\MDDBQueryUI.h"
#include "IMDProjectExpert.h"

#include <QDebug>

CCurveWidget::CCurveWidget(QWidget *parent):
    QWidget(parent),
    m_isRealTime(true),
    m_environment(NULL),
    m_curve( new CCurveProperty(this)),
    m_rubberBand(NULL),
    m_rubberBandVisble(false),
    m_isPressed(false),
    m_isPreview(false)
{
    setFocusPolicy(Qt::StrongFocus);

    connect(m_curve,SIGNAL(updateWidget()),this,SLOT(update())); // 刷新界面
    int height  = size().height();
    int width = size().width();

    m_curve->setParentWidget(this);
    m_curve->setDrawArea(QRectF(0,0,width,height));

    setMouseTracking(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenu(QPoint)));// 响应右键菜单

    m_curve->setVernierClosed(); //! 初始时游标和线条不显示
    createMenu();
}

CCurveWidget::~CCurveWidget()
{
    if(m_curve)
    {
        delete m_curve;
        m_curve = NULL;
    }
}

int CCurveWidget::setDILProperty()
{
    MDCurveSettingDialog dialog(this);
    dialog.setData(m_curve,m_environment,m_isRealTime);
    dialog.setWidget(this);
    return dialog.exec();

}
/*!
\section js
*/
void CCurveWidget::setHistoryDataSearchFilter(const QString &filter)
{
    m_curve->setHistoryDataSearchFilter(filter);
}

void CCurveWidget::setHistoryTagSearchFilter(const QString &filter)
{
    m_curve->setHistoryTagSearchFilter(filter);
}

void CCurveWidget::setDataSourceParameter(int type, const QString &parameter)
{
//   IMDDataBaseParam* databaseParam = CMDDataSourceFactory::createDatabasePaream();
//    if(!databaseParam)
//        return;

//    switch ((DataSourceParameters)type)
//    {
//    case DataSourceParameters::DBType:
//        databaseParam->setdbServeType((DATABASEPARAM::DATABASE_TYPE)stringToType(parameter));
//        break;
//    case DataSourceParameters::DBIP:
//        databaseParam->setdbServerIP(parameter);
//        break;
//    case DataSourceParameters::DBPort:
//        databaseParam->setdbServerPort(parameter.toInt());
//        break;
//    case DataSourceParameters::DBName:
//        databaseParam->setdbName(parameter);
//        break;
//    case DataSourceParameters::DBUser:
//        databaseParam->setdbUser(parameter);
//        break;
//    case DataSourceParameters::DBPassword:
//        databaseParam->setdbPassword(parameter);
//        break;

//    default:
//        break;
//    }

//    m_curve->data()->setDBSetting(databaseParam);
}
void CCurveWidget::query()
{
    m_curve->query();
    update();
}

int CCurveWidget::stringToType(const QString & strParam)
{
    int nType = -1;
    if (0 == strParam.compare(tr("MYSQL"), Qt::CaseInsensitive))
    {
        nType = DATABASEPARAM::DB_TYPE_MYSQL;
    }
    else if (0 == strParam.compare(tr("ORACLE"), Qt::CaseInsensitive))
    {
        nType = DATABASEPARAM::DB_TYPE_ORACLE;
    }
    else if (0 == strParam.compare(tr("KINGBASE"), Qt::CaseInsensitive))
    {
        nType = DATABASEPARAM::DB_TYPE_KINGBASE;
    }
    else if (0 == strParam.compare(tr("DB2"), Qt::CaseInsensitive))
    {
        nType = DATABASEPARAM::DB_TYPE_DB2;
    }
    else if (0 == strParam.compare(tr("SQLITE"), Qt::CaseInsensitive))
    {
        nType = DATABASEPARAM::DB_TYPE_SQLLITE;
    }
    else if (0 == strParam.compare(tr("SQLSERVER"), Qt::CaseInsensitive))
    {
        nType = DATABASEPARAM::DB_TYPE_SQL;
    }
    return nType;
}

void CCurveWidget::createMenu()
{
    m_menu = new QMenu(this);
    QAction* queryAction = new QAction("查询", this);
    m_menu->addAction(queryAction);
    QAction* configAction = new QAction("参数配置", this);
    m_menu->addAction(configAction);
    connect (queryAction,&QAction::triggered,this,&CCurveWidget::queryHistory);
    connect (configAction,&QAction::triggered,this,&CCurveWidget::setConfig);

}

/*!
\section environment function
*/
void CCurveWidget::setIsRealTime(bool isRealTime)
{
    m_curve->setIsRealTime(isRealTime);
    m_isRealTime = isRealTime;
}


void CCurveWidget::setDrawEnvironment(IDrawEnvrionment *value)
{
    if(value)
    {
        m_curve->setDrawEnvironment(value);
//        m_curve->setParentWidget(this);
        m_environment = value;
    }
}

void CCurveWidget::moveLength(const double &length)
{
    if(length == 0)
        return;
    m_curve->move(length);
}

void CCurveWidget::moveHeight(const double &height)
{
    if(height == 0)
        return;
    m_curve->moveHeight(height);
}
void CCurveWidget::setCurrentMousePoint(const QPoint &p)
{
    m_curve->setCurrentMousePoint(p);
    update();
}

void CCurveWidget::setCurve(CCurveProperty *curve)
{
    if(m_curve == curve)
        return;
    if(m_curve)
        delete m_curve;
    m_curve = curve;
}

void CCurveWidget::setCurveParentWidget(QWidget *parent)
{
    m_curve->setParentWidget(parent);
}

CCurveProperty *CCurveWidget::clone(QWidget *parent)
{
    CCurveProperty *curve = m_curve->clone(parent);
    return curve;
}

/*!
\section reimplemention
*/
void CCurveWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //留白
    QRect rect(QPoint(0,0),size());
    painter.fillRect(rect,Qt::white);
    m_curve->draw(&painter,rect);

}

void CCurveWidget::resizeEvent(QResizeEvent *e)
{
    int height  = e->size().height();
    int width = e->size().width();

    m_curve->setDrawArea(QRectF(0,0,width,height));
    m_curve->calculate();// need to test
    m_curve->calculatePreviewCurve();
}
void CCurveWidget::mouseMoveEvent(QMouseEvent *e)
{
    QRectF rect;
    rect.setTopLeft(QPointF(m_curve->curveArea().left(),m_curve->curveArea().top()));
    rect.setSize(m_curve->curveArea().size());

    m_endPoint = e->pos();
    if(!rect.contains(e->pos()))
    {
        m_curve->setVernierClosed();
        m_isPressed = false;
    }

    if(rect.contains(e->pos()) && !m_isPressed)
    { // the current point
        setCurrentMousePoint(m_endPoint);
        setCursor(Qt::CrossCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
//        m_curve->setVernierClosed();

    }
    if(m_isPressed && m_curve->isMoveButtonChecked() && rect.contains(e->pos()))
    {
        //move the curve
        setCursor(Qt::ClosedHandCursor);
        moveLength(m_endPoint.x() - m_startPoint.x());
        moveHeight(m_endPoint.y() - m_startPoint.y());
        m_startPoint = m_endPoint;
    }


    if(m_rubberBandVisble && rect.contains(e->pos())){
        if(m_startPoint.x() > e->pos().x())
            return;
        m_rubberBand->setGeometry(QRect(m_startPoint,e->pos()).normalized());
    }

    //! 预览

    if(m_isPreview)
    {
        m_curve->setPreviewMoveLength(m_endPoint.x() - m_startPoint.x());
        m_startPoint = m_endPoint;
    }

    QWidget::mouseMoveEvent(e);

}

void CCurveWidget::mousePressEvent(QMouseEvent *e)
{
    m_isPressed = true;
    QRectF rect(m_curve->curveArea());
    rect.moveTop(m_curve->curveArea().top());

    m_startPoint = e->pos();
    if(e->button() == Qt::LeftButton && rect.contains(m_startPoint))
    {
        if(m_curve->isMoveButtonChecked())
        {
            setCursor(Qt::OpenHandCursor);
        }
        if(m_curve->buttonSelectZoomIn() && rect.contains(m_startPoint)) { //select zoomIn
            if(!m_rubberBand)
                m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            m_rubberBand->setGeometry(QRect(m_startPoint, QSize()));
            m_rubberBand->show();
            m_rubberBandVisble = true;
        }
    }

    if(e->button() == Qt::LeftButton)
    {
        m_startMoveRect = m_curve->startMoveRect();
        m_endMoveRect = m_curve->endMoveRect();
        m_dragMoveRect = m_curve->dragMoveRect();
        m_startArrows = m_curve->startRollRect();
        m_endArrows = m_curve->endRollRect();


        if(m_startMoveRect.contains(e->pos()) == true)
        {
            setCursor(Qt::SizeHorCursor);
            m_moveType = moveType::moveLeft;
            m_isPreview = true;
            m_curve->previewCurve()->setMoveType((int)moveType::moveLeft);
        }
        if(m_endMoveRect.contains(e->pos()) == true)
        {
            setCursor(Qt::SizeHorCursor);
            m_moveType = moveType::moveRight;
            m_isPreview = true;
            m_curve->previewCurve()->setMoveType((int)moveType::moveRight);

        }
        if(m_dragMoveRect.contains(e->pos()) == true)
        {
            setCursor(Qt::SizeHorCursor);
            m_moveType = moveType::move;
            m_isPreview = true;
            m_curve->previewCurve()->setMoveType((int)moveType::move);
        }
        if(m_startArrows.contains(e->pos()) == true)
        {
            m_curve->previewCurve()->startRollMove();
        }
        if(m_endArrows.contains(e->pos()) == true)
        {
            m_curve->previewCurve()->endRollMove();
        }
    }
    QWidget::mousePressEvent(e);
}

void CCurveWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QRectF rect(m_curve->curveArea());
    rect.moveTop(m_curve->curveArea().top());

    m_endPoint = e->pos();

    if(m_isPressed)
    {
        m_isPressed = false;
    }
    if(m_isPreview)
    {
        m_isPreview = false;
    }
    if(m_rubberBandVisble/*&& rect.contains(e->pos())*/)
    {
        QSize size = m_rubberBand->rect().size();
        if(size.width() == 0 || size.height() == 0)
            return;
        QRect rubberRect;
        rubberRect.setTopLeft(m_startPoint);
        rubberRect.setSize(size);

        m_curve->zoomIn(rubberRect,rect.toRect());
        m_rubberBand->hide();
        m_rubberBandVisble = false;
    }
    setCursor(Qt::ArrowCursor);

    QWidget::mouseReleaseEvent(e);

}

void CCurveWidget::slotCustomMenu(const QPoint &p)
{
    if(!m_isRealTime && !m_curve->isStudioMode())
    {
        QPoint point =  mapToGlobal(p);
        m_menu->exec(point);
    }
}

void CCurveWidget::queryHistory()
{
    if(!m_isRealTime && !m_curve->isStudioMode())
    {
         query();
    }
}

void CCurveWidget::setConfig()
{
    if(!m_isRealTime && !m_curve->isStudioMode())
    {
        IMDProjectExpert *project = m_environment->projectExpert();
        if(project)
        {
            IMDDBQueryEngine*engine =  project->queryEngine();
            DBQUI::CallStandardRumtimeParmatersConfigUI(engine);
            query();
        }
    }
}

/*!
\section serialize
*/
void CCurveWidget::serialize(QJsonObject &json)
{
    m_curve->serialize(json);
}

void CCurveWidget::deserialize(const QJsonObject &json)
{
    m_curve->deserialize(json);
}
void CCurveWidget::changeLanguage()
{
    if(m_curve){
        m_curve->changeLanguage();
    }
}

void CCurveWidget::collectWords()
{
    if(m_curve)
    {
        m_curve->collectWords();
    }
}
void CCurveWidget::getBrushes(QList<CMDBrushManager *> &list)
{
    m_curve->appearance()->getBrushes(list);
    m_curve->previewCurve()->getBrushes(list);
}

void CCurveWidget::setValue(int index, QVariant value)
{
    m_curve->referenceCurves()->setValue(index,value);
}

void CCurveWidget::getExpressionList(QList<int> &indexs, QList<QString> &expressions)
{
    m_curve->referenceCurves()->getExpressionList(indexs,expressions);
}

void CCurveWidget::setStartTime(const QString &curveName, const QVariant &startTime)
{
    m_curve->referenceCurves()->setStartTime(curveName,startTime);
}

void CCurveWidget::setReferenceCurveData(const QString &curveName, const QVariant &xValue, const QVariant &yValue)
{
    m_curve->data()->setReferenceCurveData(curveName,xValue,yValue);
    update();
}

