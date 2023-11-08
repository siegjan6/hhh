#include "DrawCreator.h"

#include "MDDrawRect.h"
#include "MDDrawText.h"
#include "MDDrawGroup.h"
#include "MDDrawCombine.h"
#include "MDDrawArc.h"
#include "MDDrawShear.h"
#include "MDDrawSector.h"
#include "MDDrawEllipse.h"
#include "MDDrawButton.h"

#include "MDDrawCalendarWidget.h"
#include "MDDrawCheckBox.h"
#include "MDDrawComboBox.h"
#include "MDDrawDateTimeEdit.h"
#include "MDDrawList.h"
#include "MDDrawRadioButton.h"
#include "MDDrawSpinBox.h"
#include "MDDrawTextEdit.h"
#include "MDDrawTableView.h"
#include "MDDrawSpecial/MDDrawReport.h"
#include "MDDrawSpecial/MDDrawCurve.h"
#include "MDDrawSpecial/MDDrawGauge.h"
#include "MDDrawSpecial/MDDrawAlarm.h"
#include "MDDrawSpecial/MDDrawLog.h"
#include "MDDrawSpecial/MDDrawDataArchives.h"
#include "MDDrawSpecial/MDDrawDiagram.h"
#include "MDDrawNode/MDDrawBezier.h"
#include "MDDrawNode/MDDrawFoldLine.h"
#include "MDDrawNode/MDDrawLine.h"
#include "MDDrawNode/MDDrawPipe.h"
#include "MDDrawNode/MDDrawPolygon.h"
#include "MDDrawNode/MDDrawClosedBezier.h"
#include "MDDrawSpecial/MDDrawRecipeGrid.h"
#include "MDDrawSpecial/MDDrawProcDocker.h"
#include "MDDrawSpecial/MDDrawPdfViewer.h"
#include "MDDrawCalendarWidgetWrapper.h"
#include "MDDrawCheckBoxWrapper.h"
#include "MDDrawComboBoxWrapper.h"
#include "MDDrawDateTimeEditWrapper.h"
#include "MDDrawListWrapper.h"
#include "MDDrawRadioButtonWrapper.h"
#include "MDDrawSpinBoxWrapper.h"
#include "MDDrawTextEditWrapper.h"
#include "DrawReportWrapper.h"
#include "DrawCurveWrapper.h"
#include "DrawTableViewWrapper.h"
#include "DrawDataArchivesWrapper.h"
#include "DrawProcDockerWrapper.h"
#include "DrawPdfViewerWrapper.h"

#include "Environment.h"
//////////////////////////////////////////////////////////////////////////
bool CDrawCreator::isDrawNode(DrawType type)
{
    switch (type)
    {
    case DrawType::StraightLine:
    case DrawType::FoldLine:
    case DrawType::Bezier:
    case DrawType::Polygon:
    case DrawType::ClosedBezier:
    case DrawType::Pipe:
        return true;
    default:
        break;
    }

    return false;
}
OrthoMode CDrawCreator::drawOrthoMode(DrawType type)
{
    switch (type)
    {
    case DrawType::Rect:
    case DrawType::Ellipse:
    case DrawType::Arc:
    case DrawType::Sector:
    case DrawType::Shear:
    case DrawType::Text:
    case DrawType::Button:
    case DrawType::Gauge:
        return OrthoMode::Square;
    case DrawType::StraightLine:
    case DrawType::FoldLine:
    case DrawType::Polygon:
    case DrawType::Pipe:
        return OrthoMode::HoriOrVert;
    default:
        break;
    }

    return OrthoMode::Invalid;
}
//////////////////////////////////////////////////////////////////////////
QList<QString> CDrawCreator::m_names;
QList<QString> *CDrawCreator::names()
{
    if (m_names.count() == 0)
    {
        m_names<<"Group"<<"Combine";
        m_names<<"Rect"<<"Ellipse"<<"Arc"<<"Sector"<<"Shear"<<"Text"<<"Button";
        m_names<<"StraightLine"<<"FoldLine"<<"Bezier"<<"Polygon"<<"ClosedBezier";
        m_names<<"TextEdit"<<"ComboBox"<<"List"<<"RadioButton"<<"CheckBox"<<"SpinBox"<<"DateTime"<<"Calendar";
        m_names<<"Alarm"<<"Log"<<"Gauge"<<"Curve"<<"Report";
        m_names<<"Pipe"<<"DataArchives"<<"Diagram"<<"HistoryCurve"<<"Table"<<"Recipe"<<"ProcDocker"<<"PdfViewer";
    }

    return &m_names;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CDrawCreator::createDrawObj(DrawType type)
{
    CMDDrawObj* obj = NULL;
    switch (type)
    {
    case DrawType::Group:
        obj = new CMDDrawGroup();
        break;
    case DrawType::Combine:
        obj = new CMDDrawCombine();
        break;

    case DrawType::Rect:
        obj = new CMDDrawRect();
        break;
    case DrawType::Ellipse:
        obj = new CMDDrawEllipse;
        break;
    case DrawType::Arc:
        obj = new CMDDrawArc;
        break;
    case DrawType::Sector:
        obj = new CMDDrawSector;
        break;
    case DrawType::Shear:
        obj = new CMDDrawShear;
        break;
    case DrawType::Text:
        obj = new CMDDrawText;
        break;
    case DrawType::Button:
        obj = new CMDDrawButton;
        break;

    case DrawType::StraightLine:
        obj = new CMDDrawLine;
        break;
    case DrawType::FoldLine:
        obj = new CMDDrawFoldLine;
        break;
    case DrawType::Bezier:
        obj = new CMDDrawBezier;
        break;
    case DrawType::Polygon:
        obj = new CMDDrawPolygon;
        break;
    case DrawType::ClosedBezier:
        obj = new CMDDrawClosedBezier;
        break;
    case DrawType::Pipe:
        obj = new CMDDrawPipe;
        break;


    case DrawType::TextEdit:
        obj = new CMDDrawTextEdit;
        break;
    case DrawType::ComboBox:
        obj = new CMDDrawComboBox;
        break;
    case DrawType::List:
        obj = new CMDDrawList();
        break;
    case DrawType::RadioButton:
        obj = new CMDDrawRadioButton;
        break;
    case DrawType::CheckBox:
        obj = new CMDDrawCheckBox;
        break;
    case DrawType::SpinBox:
        obj = new CMDDrawSpinBox;
        break;
    case DrawType::DateTime:
        obj = new CMDDrawDateTimeEdit;
        break;
    case DrawType::Calendar:
        obj = new CMDDrawCalendarWidget;
        break;

    case DrawType::Alarm:
        obj = new CMDDrawAlarm;
        break;
    case DrawType::Log:
        obj = new CMDDrawLog;
        break;
    case DrawType::Gauge:
        obj = new CMDDrawGauge;
        break;
    case DrawType::Curve:
        obj = new CMDDrawCurve;
        break;
    case DrawType::HistoryCurve:
        obj = new CMDDrawHistory;
        break;
    case DrawType::Report:
        obj = new CMDDrawReport;
        break;
    case DrawType::DataArchives:
        obj = new CMDDrawDataArchives;
        break;
    case DrawType::Diagram:
        obj = new CMDDrawDiagram;
        break;
    case DrawType::TableView:
        obj = new CMDDrawTableView;
        break;
    case DrawType::RecipeGrid:
        obj = new CMDDrawRecipeGrid;
        break;
    case DrawType::ProcDocker:
        obj = new CMDDrawProcDocker;
        break;
    case DrawType::PdfViewer:
        obj = new CMDDrawPdfViewer;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    if (obj != NULL)
        obj->initCreate();

    return obj;
}
CMDObjectWrapper *CDrawCreator::createObjectWrapper(CMDDrawObj *obj)
{
    CMDObjectWrapper* w = NULL;

    switch (obj->type())
    {
    case DrawType::Group:

    case DrawType::Combine:
    case DrawType::Rect:
    case DrawType::Ellipse:
    case DrawType::Arc:
    case DrawType::Sector:
    case DrawType::Shear:
        return new CMDDrawVectorWrapper(obj, obj->name());
    case DrawType::Button:
    case DrawType::Text:
        return new CMDDrawTextWrapper(obj, obj->name());

    case DrawType::StraightLine:
    case DrawType::FoldLine:
    case DrawType::Bezier:
    case DrawType::Polygon:
    case DrawType::ClosedBezier:
    case DrawType::Pipe:
        return new CMDDrawVectorWrapper(obj, obj->name());

    case DrawType::TextEdit:
        return new CMDDrawTextEditWrapper(obj, obj->name());
    case DrawType::ComboBox:
        return new CMDDrawComboBoxWrapper(obj, obj->name());
    case DrawType::List:
        return new CMDDrawListWrapper(obj, obj->name());
    case DrawType::RadioButton:
        return new CMDDrawRadioButtonWrapper(obj, obj->name());
    case DrawType::CheckBox:
        return new CMDDrawCheckBoxWrapper(obj, obj->name());
    case DrawType::SpinBox:
        return new CMDDrawSpinBoxWrapper(obj, obj->name());
    case DrawType::DateTime:
        return new CMDDrawDateTimeEditWrapper(obj, obj->name());
    case DrawType::Calendar:
        return new CMDDrawCalendarWidgetWrapper(obj, obj->name());

    case DrawType::Alarm:
    case DrawType::Log:
    case DrawType::Gauge:
    case DrawType::Curve:
        return new CDrawRealCurveWrapper(obj, obj->name());
    case DrawType::HistoryCurve:
        return new CDrawHisCurveWrapper(obj, obj->name());
    case DrawType::Report:
        return new CDrawReportWrapper(obj, obj->name());
    case DrawType::DataArchives:
        return new CDrawDataArchivesWrapper(obj, obj->name());
    case DrawType::Diagram:
        return new CMDDrawObjWrapper(obj, obj->name());
    case DrawType::TableView:
        return new CDrawTableViewWrapper(obj, obj->name());
    case DrawType::RecipeGrid:
        return new CMDDrawObjWrapper(obj, obj->name());
    case DrawType::ProcDocker:
        return new CDrawProcDockerWrapper(obj, obj->name());
    case DrawType::PdfViewer:
        return new CDrawPdfViewerWrapper(obj, obj->name());
    default:
        Q_ASSERT(false);
        break;
    }

    return w;
}
//////////////////////////////////////////////////////////////////////////
void CDrawCreator::initExpression()
{
    if(CMDExpConfig::expManager((int)DrawType::Obj) != NULL)
        return;

    CMDExpConfig::initExpManager((int)DrawType::Form, CEnvironment::expNames());
    CMDExpConfig::initExpManager((int)DrawType::Obj, CMDDrawObj::expNames());
    CMDExpConfig::initExpManager((int)DrawType::Vector, CMDDrawVector::expNames());

    CMDExpConfig::initFactExpManager(
        (int)DrawType::Group, CMDDrawGroup::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Combine, CMDDrawCombine::expNames(), true);

    CMDExpConfig::initFactExpManager(
        (int)DrawType::Rect, QList<QStringList>(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Ellipse, QList<QStringList>(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Arc, CMDDrawBasicArc::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Sector, CMDDrawBasicArc::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Shear, CMDDrawBasicArc::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Text, CMDDrawText::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Button, QList<QStringList>(), true);

    CMDExpConfig::initFactExpManager(
        (int)DrawType::Pipe, CMDDrawPipe::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::StraightLine, CMDDrawLine::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::FoldLine, CMDDrawNode::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Bezier, CMDDrawBezier::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Polygon, CMDDrawNode::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::ClosedBezier, CMDDrawClosedBezier::expNames(), true);

    CMDExpConfig::initFactExpManager(
        (int)DrawType::TextEdit, CMDDrawTextEdit::expNames(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::ComboBox, CMDDrawComboBox::expNames(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::SpinBox, CMDDrawSpinBox::expNames(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::CheckBox, CMDDrawCheckBox::expNames(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::RadioButton, CMDDrawRadioButton::expNames(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Calendar, CMDDrawCalendarWidget::expNames(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::DateTime, CMDDrawDateTimeEdit::expNames(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::List, QList<QStringList>(), false, true);

    CMDExpConfig::initFactExpManager(
        (int)DrawType::Alarm, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Log, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Gauge, CMDDrawGauge::expNames(), true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Curve, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Report, QList<QStringList>(), false, true);

    CMDExpConfig::initFactExpManager(
        (int)DrawType::DataArchives, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::Diagram, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::HistoryCurve, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::TableView, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::RecipeGrid, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::ProcDocker, QList<QStringList>(), false, true);
    CMDExpConfig::initFactExpManager(
        (int)DrawType::PdfViewer, QList<QStringList>(), false, true);
}
//////////////////////////////////////////////////////////////////////////
