#include "CTools.h"
#include "CXAxisManager.h"
#include "CXAxis.h"
#include "CYAxis.h"
#include "CYAxisManager.h"
#include "CCurveObjectManager.h"
#include "CCurveObject.h"
#include "CToolButton.h"
#include "CPreviewCurve.h"

#include <QButtonGroup>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

CTools::CTools(QObject *parent):
    QObject(parent),
    m_index(0),
    m_mulIndex(0),
    m_xManager(0),
    m_yManager(0),
    m_curveManager(0),
    m_buttonSize(32,32),
    m_parentWidget(0),
    m_buttonGroup(new QButtonGroup(this)),
    m_layout(new QGridLayout),
    m_vSpaceItem(new QSpacerItem(2,100,QSizePolicy::Minimum,QSizePolicy::Expanding)),
    m_hSpaceItem(new QSpacerItem(100,2,QSizePolicy::Expanding,QSizePolicy::Minimum)),
    m_isRun(false),
    m_isStudioMode(true),
    m_isRealTime(true)

{
    m_scales.clear();
    m_scales << 1 << 0.8 << 0.6 << 0.4 << 0.2 << 0.1 << 0.05;
    m_mulScales<< 1 << 0.8 << 0.48 << 0.192 << 0.0384 << 0.00384 << 0.000192;

    insertDefaultButtonNames();
    insertDefaultButtonIcons();

    connect(m_buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(slotButtonClicked(int)));
}

CTools::~CTools()
{
    removeAllButtons();
}

void CTools::setIsStudioMode(bool isStudioMode)
{
    m_isStudioMode = isStudioMode;
    if(!m_isStudioMode){
        setLayout();
    }
}

void CTools::setParentWidget(QWidget *parent)
{
    m_parentWidget = parent;
    createDefaultButtons(parent);
}

void CTools::calculate()
{
    if(m_isStudioMode)
        setLayout();
}

bool CTools::buttonZoomIn()
{
    if(m_index == MaxIndex -1 || m_mulIndex == 6)
        return false;
    int index = ++m_index;
    ++m_mulIndex;
    if(m_xManager && m_yManager)
    {
        for(int i = 0; i< m_xManager->list().count();i++)
        {
            CXAxis *x = m_xManager->list().at(i);
            double xStart = x->startScale();
            double xEnd = x->endScale();
//            setXScale((xEnd - xStart)*(1-m_scales[index])/2 +xStart,(xEnd - xStart)*((1-m_scales[index])/2+m_scales[index]) +xStart);
            x->setScale((xEnd - xStart)*(1-m_scales[index])/2 +xStart,(xEnd - xStart)*((1-m_scales[index])/2+m_scales[index]) +xStart);
            if(m_previewCurve)
            {
                m_previewCurve->setScale((xEnd - xStart)*(1-m_scales[index])/2 +xStart,(xEnd - xStart)*((1-m_scales[index])/2+m_scales[index]) +xStart);
            }
        }

        for(int i = 0;i< m_yManager->list().count(); i++)
        {
            CYAxis *y = m_yManager->list().at(i);
            double yStart = y->startScale();
            double yEnd = y->endScale();
            y->setScale((yEnd - yStart)*(1-m_scales[index])/2 +yStart,(yEnd - yStart)*((1-m_scales[index])/2+m_scales[index]) +yStart);

        }

        clearCurvePic();
        emit refresh();
        return true;
    }

    return false;
}

bool CTools::buttonZoomOut()
{
    if(!m_index || !m_mulIndex)
        return false;

    if(m_xManager && m_yManager)
    {
        for(int i = 0; i <m_xManager->list().count(); i++)
        {
            CXAxis *x = m_xManager->list().at(i);
            double xStart = x->startScale();
            double xEnd = x->endScale();
            double start = xStart - (1-m_scales[m_index])/2 *(m_mulScales[m_mulIndex] / m_scales[m_index]);
            double end = xEnd + (1-m_scales[m_index])/2 *(m_mulScales[m_mulIndex] / m_scales[m_index]);
            if(end > 1)
            {
                end = 1;
            }
            x->setScale(start,end);
            if(m_previewCurve)
                m_previewCurve->setScale(start,end);
        }

        for(int i = 0; i <m_yManager->list().count(); i++)
        {
            CYAxis *y = m_yManager->list().at(i);
            double yStart = y->startScale();
            double yEnd = y->endScale();
            double start = yStart - (1-m_scales[m_index])/2 *(m_mulScales[m_mulIndex] / m_scales[m_index]);
            double end = yEnd + (1-m_scales[m_index])/2 *(m_mulScales[m_mulIndex] / m_scales[m_index]);\
            if(end > 1)
            {
                end = 1;
            }
            y->setScale(start ,end);

        }
        m_index--;
        m_mulIndex--;
        clearCurvePic();
        emit refresh();
        return true;
    }

    return false;
}

bool CTools::buttonNormal()
{
    m_index = 0;
    m_mulIndex = 0;
    if(m_yManager)
    {
        for(int i = 0; i < m_yManager->list().count();i++)
        {
            CYAxis *y = m_yManager->list().at(i);
            y->setMaxValue(y->originMax());
            y->setMinValue(y->originMin());
        }
    }

    if(m_xManager)
    {
        for(int i = 0;i < m_xManager->list().count(); i++)
        {
            CXAxis *x = m_xManager->list().at(i);
            if(x->valueType() == CXAxis::ValueType::Value)
            {
                x->setMaxValue(x->originMax());
                x->setMinValue(x->originMin());
            }
            else if(x->valueType() == CXAxis::ValueType::Time)
            {
                x->setTotalLength(x->originLength());
                if(m_isRealTime)
                {
                    if(x->originEndTime() >= QDateTime::currentDateTime())
                    {
                        x->setEndTime(x->originEndTime());
                    }
                    else
                    {
                        x->setEndTime(QDateTime::currentDateTime());
                    }
                }
                else
                {
                    x->setEndTime(x->originEndTime());
                }
            }
        }
    }
    if(m_previewCurve)
    {
        m_previewCurve->setScale(0,1);
    }

    if(setXScale(0,1) && setYScale(0,1))
    {
        emit refresh();
        return true;
    }
    return false;
}

void CTools::buttonSelectZoom(const QRect &select, const QRect &source)
{
    double yEndScale = ((double)source.bottom() - select.top())/ (source.bottom() - source.top());
    double yStartScale = ((double) source.bottom() - select.bottom()) /(source.bottom() - source.top());
    double xStartScale  = ((double)select.left() - source.left()) / (source.right() - source.left());
    double xEndScale = ((double)select.right() - source.left()) / (source.right() - source.left());

    if(m_xManager && m_yManager)
    {
          for(int i = 0; i < m_xManager->list().count();i++)
          {
              CXAxis *x = m_xManager->list().at(i);
              double xStart = x->startScale();
              double xEnd = x->endScale();
              if(xStart)
              {
                  x->setScale((xEnd - xStart)*xStartScale + xStart,(xEnd - xStart)*xEndScale + xStart);
                  if(m_previewCurve)
                  {
                      m_previewCurve->setScale((xEnd - xStart)*xStartScale + xStart,(xEnd - xStart)*xEndScale + xStart);
                  }
              }

              else
              {
                   x->setScale(xStartScale,xEndScale);
                   if(m_previewCurve)
                   {
                       m_previewCurve->setScale(xStartScale,xEndScale);
                   }
              }
          }

          for(int i = 0; i < m_yManager->list().count();i++)
          {
              CYAxis *y = m_yManager->list().at(i);
              double yStart = y->startScale();
              double yEnd = y->endScale();
              if(y->startScale())
                  y->setScale((yEnd - yStart)*yStartScale + yStart,(yEnd - yStart)*yEndScale + yStart);
              else
                  y->setScale(yStartScale,yEndScale);
          }
      }
    clearCurvePic();
    emit refresh();
}

void CTools::buttonMove()
{

}

void CTools::buttonRun()
{
    m_isRun = !m_isRun;
    CToolButton *pauseButton = button(NameId::Run);
    setCurveRunning(m_isRun);
    if(m_isRun)
    { // run
        pauseButton->setIcon(m_iconsMap.value(idToName(NameId::Pause)));
        clearCurvePic();
        emit run();
    }
    else
    { // stop
        pauseButton->setIcon(m_iconsMap.value(idToName(NameId::Run)));
        emit stop();
    }
}

void CTools::slotButtonClicked(int id)
{
    switch ((NameId)id) {
    case NameId::Normal:
        buttonNormal();
        break;
    case NameId::ZoomIn:
        buttonZoomIn();
        break;
    case NameId::ZoomOut:
        buttonZoomOut();
        break;
    case NameId::ZoomSelect:
        break;
    case NameId::Move:
        buttonMove();
        break;
    case NameId::Run:
        buttonRun();
        break;
    default:
        break;
    }
}

CTools *CTools::clone(QWidget *parent)
{
    CTools *tools = new CTools;
    tools->m_buttonSize = this->m_buttonSize;
    tools->m_iconsMap = this->m_iconsMap;
    tools->m_index = this->m_index;
    tools->m_parentWidget = parent;

    tools->deleteAllButtons();

    for(int i = 0; i < m_buttonGroup->buttons().count();i++){
        QAbstractButton *button = m_buttonGroup->buttons().at(i);
        CToolButton *tbutton = qobject_cast<CToolButton*>(button)->clone();
        tbutton->setCheckable(true);
        tbutton->setFixedSize(tools->m_buttonSize);
        tbutton->setIcon(tools->m_iconsMap.value(tbutton->buttonName()));
        tbutton->setIconSize(tools->m_buttonSize);
        tbutton->setParent(parent);
        tools->addButton(tbutton);
    }
    tools->m_buttonGroup->setParent(parent);
    tools->setLayout();
    return tools;
}

void CTools::serialize(QJsonObject &json)
{
    json.insert("ButtonWidth",m_buttonSize.width());
    json.insert("ButtonHeight",m_buttonSize.height());

    QJsonArray array;
    int count = m_iconsMap.count();
    for(int i = 0 ; i<count; i++)
    {
        QJsonObject myJson;
        QString str = m_iconsMap.keys().at(i);
        myJson.insert("Key",str);
        QIcon icon = m_iconsMap.find(str).value();
        myJson.insert(str,icon.name());
        array.append(myJson);
    }
    json.insert("IconMap",array);

    QJsonArray buttonArray;
    CToolButton *toolButton = NULL;
    for(int i = 0; i < m_buttonGroup->buttons().count();i++)
    {
        QJsonObject buttonJson;
        QAbstractButton *button = m_buttonGroup->buttons().at(i);
        toolButton = qobject_cast<CToolButton*> (button);
        toolButton->serialize(buttonJson);
        buttonArray.append(buttonJson);
    }
    json.insert("Tools",buttonArray);
}

void CTools::deserialize(const QJsonObject &json)
{
    int width = json.value("ButtonWidth").toInt();
    int height = json.value("ButtonHeight").toInt();
    m_buttonSize.setWidth(width);
    m_buttonSize.setHeight(height);
    QJsonArray iconArray = json.value("IconMap").toArray();
    int iconCount = iconArray.count();
    for(int i =0; i<iconCount;i++)
    {
        QJsonObject myJson = iconArray.at(i).toObject();
        QString str = myJson.value("Key").toString();
        QString iconName = myJson.value(str).toString();
        QMap<QString,QIcon>map;
        QIcon icon = QIcon(iconName);
        map.insert(str,icon);
    }

    QJsonArray buttonArray = json.value("Tools").toArray();
    for(int i = 0; i < buttonArray.count();i++)
    {
        QJsonObject buttonJson = buttonArray.at(i).toObject();
        CToolButton *toolButton = NULL;
        QAbstractButton *button = m_buttonGroup->buttons().at(i);
        toolButton = qobject_cast<CToolButton*> (button);
        toolButton->deserialize(buttonJson);
    }
}

void CTools::addButton(CToolButton *button)
{
    if(button){
        m_buttonGroup->addButton(button,(int)nameToId(button->buttonName()));
    }
}

void CTools::deleteAllButtons()
{
    if(m_buttonGroup){
        QList<QAbstractButton*> buttonLists = m_buttonGroup->buttons();
        for(int i  =0; i < buttonLists.count();i++){
            QAbstractButton *button = buttonLists.at(i);
            m_buttonGroup->removeButton(button);
            delete button;
            button = NULL;
        }
    }
}

void CTools::setXAxisManager(CXAxisManager *xs)
{
    m_xManager = xs;
}

void CTools::setYAxisManger(CYAxisManager *ys)
{
    m_yManager = ys;
}

void CTools::setManagers(CXAxisManager *xs, CYAxisManager *ys, CCurveObjectManager *curves)
{
    m_xManager = xs;
    m_yManager = ys;
    m_curveManager = curves;
}

CToolButton *CTools::createButton(const QString &name) const
{
    CToolButton *button = new CToolButton;
    button->setCheckable(true);
    button->setButtonName(name);
    button->setFixedSize(m_buttonSize);
    return button;
}

void CTools::createDefaultButtons(QWidget *parent)
{
    CToolButton *button = NULL;

    for(int i = 0; i < m_defaultbuttonNames.count();i++){
        QString name = m_defaultbuttonNames.at(i);
        button = createButton(name);
        button->setParent(parent);
        button->setButtonPos((int)nameToId(name));
        button->setIcon(m_iconsMap.value(name));   // set icon
        button->setIconSize(m_buttonSize);
        button->setVisible(false);
        m_buttonGroup->addButton(button,(int)nameToId(name));
    }
    m_buttonGroup->setParent(parent);
}

void CTools::removeAllButtons()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != 0) {
        delete child;
        child = NULL;
    }
    if(m_layout){
        delete m_layout;
        m_layout = NULL;
    }
    if(m_buttonGroup){
        //        qDeleteAll(m_buttonGroup->buttons()); // buttons should be delete manully
        deleteAllButtons();
        delete m_buttonGroup;
        m_buttonGroup = NULL;
    }
    m_iconsMap.clear();
    m_defaultbuttonNames.clear();
}

CTools::NameId CTools::nameToId(const QString &name)
{
    if(name == tr("Normal"))
        return NameId::Normal;
    else if(name == tr("ZoomIn"))
        return NameId::ZoomIn;
    else if(name == tr("ZoomOut"))
        return NameId::ZoomOut;
    else if(name == tr("SelectZoom"))
        return NameId::ZoomSelect;
    else if(name == tr("Move"))
        return NameId::Move;
    else if(name == tr("Pause"))
        return NameId::Pause;
    else if(name == tr("Start"))
        return NameId::Run;
    else
        return NameId::Error;
}

QString CTools::idToName(const CTools::NameId &id) const
{
    switch (id) {
    case NameId::Normal:
        return tr("Normal");
    case NameId::ZoomIn:
        return tr("ZoomIn");
    case NameId::ZoomOut:
        return tr("ZoomOut");
    case NameId::ZoomSelect:
        return  tr("SelectZoom");
    case NameId::Move:
        return tr("Move");
    case NameId::Pause:
        return tr("Pause");
    case NameId::Run:
        return tr("Start");
    default:
        break;
    }
    return QString();
}

void CTools::setLayout()
{
//    CToolButton *toolButton = NULL;
//    int j = 0;
//    for(int i = 0; i < m_buttonGroup->buttons().count();i++)
//    {
//        QAbstractButton *button= m_buttonGroup->buttons().at(i);
//        toolButton = qobject_cast<CToolButton*> (button);
//        toolButton->setButtonVisible(toolButton->buttonVisible());
//        toolButton->setFixedSize(m_buttonSize);

//        if(toolButton->buttonVisible())
//        {
//            if(j == 0)
//            {
//                toolButton->move(m_parentWidget->childrenRect().x() + m_buttonSize.width()*j,
//                                 m_parentWidget->childrenRect().y());
//            }
//            else
//                toolButton->move(m_parentWidget->childrenRect().x() + (m_buttonSize.width()+5)*j ,
//                                 m_parentWidget->childrenRect().y());

//            j++;
//        }
//    }

    CToolButton *toolButton = NULL;
    int k = 0;
    for(int i = 0; i<m_buttonGroup->buttons().count();i++)
    {
        for(int j = 0; j<m_buttonGroup->buttons().count();j++)
        {
            QAbstractButton *button= m_buttonGroup->buttons().at(j);
            toolButton = qobject_cast<CToolButton*> (button);
            toolButton->setButtonVisible(toolButton->buttonVisible());
            toolButton->setFixedSize(m_buttonSize);

            if(toolButton->buttonVisible() && toolButton->buttonPos() == i)
            {
                if(k == 0)
                {
                    toolButton->move(m_parentWidget->childrenRect().x() + m_buttonSize.width()*k,
                                     m_parentWidget->childrenRect().y());
                }
                else
                    toolButton->move(m_parentWidget->childrenRect().x() + (m_buttonSize.width()+5)*k ,
                                     m_parentWidget->childrenRect().y());
                k++;
            }
        }
    }
}

void CTools::removeOldLayout()
{
    for(int i = 0; i < m_buttonGroup->buttons().count();i++){
        QAbstractButton *button= m_buttonGroup->buttons().at(i);
        m_layout->removeWidget(button);
    }
    m_layout->removeItem(m_hSpaceItem);
    m_layout->removeItem(m_vSpaceItem);
}

void CTools::insertDefaultButtonNames()
{
    m_defaultbuttonNames <<idToName(NameId::Normal)
                        <<idToName(NameId::ZoomIn)
                       <<idToName(NameId::ZoomOut)
                      <<idToName(NameId::ZoomSelect)
                     <<idToName(NameId::Move)
                    << idToName(NameId::Run);
}

void CTools::insertDefaultButtonIcons()
{
    m_iconsMap.insert(idToName(NameId::Normal),QIcon(":/icon/icon/MDCurveNormal.png"));
    m_iconsMap.insert(idToName(NameId::ZoomIn),QIcon(":/icon/icon/MDCurveZoomIn.png"));
    m_iconsMap.insert(idToName(NameId::ZoomOut),QIcon(":/icon/icon/MDCurveZoomOut.png"));
    m_iconsMap.insert(idToName(NameId::ZoomSelect),QIcon(":/icon/icon/MDCurveZoomSelect.png"));
    m_iconsMap.insert(idToName(NameId::Move),QIcon(":/icon/icon/MDCurveMove.png"));
    m_iconsMap.insert(idToName(NameId::Pause),QIcon(":/icon/icon/MDCurvePause.png"));
    m_iconsMap.insert(idToName(NameId::Run),QIcon(":/icon/icon/MDCurveStart.png"));

}

bool CTools::setXScale(double start, double end)
{
    if(m_xManager){
        for(int i = 0; i < m_xManager->list().count();i++){
            CXAxis *x = m_xManager->list().at(i);
            x->setScale(start,end);
        }
        clearCurvePic();
        return true;
    }
    return false;
}

bool CTools::setYScale(double start, double end)
{
    if(m_yManager){
        for(int i = 0; i < m_yManager->list().count();i++){
            CYAxis *y = m_yManager->list().at(i);
            y->setScale(start,end);
        }
        clearCurvePic();
        return true;
    }
    return false;
}

void CTools::clearCurvePic()
{
    if(m_curveManager)
    {
        for(int i = 0; i < m_curveManager->list().count();i++){
            CCurveObject *curve = m_curveManager->list().at(i);
            curve->clearDataForZoom();
        }
    }
}

void CTools::setCurveRunning(bool run)
{
    if(m_curveManager){
        for(int i = 0; i < m_curveManager->list().count();i++){
            CCurveObject *curve = m_curveManager->list().at(i);
            curve->setIsRun(run);
        }
    }
}

void CTools::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;
    if( !m_isRealTime)
    {
        for(int i = 0;i<m_defaultbuttonNames.count();i++)
        {
            if(m_defaultbuttonNames.at(i) == "Start")
                m_defaultbuttonNames.removeAt(i);
        }
        setDefaultbuttonNames(m_defaultbuttonNames);
        deleteAllButtons();
        createDefaultButtons(m_parentWidget);
    }
    setLayout();
}

void CTools::setPreviewCurve(CPreviewCurve *preview)
{
    m_previewCurve = preview;
}

QMap<QString, QIcon> CTools::iconsMap() const
{
    return m_iconsMap;
}

void CTools::setIconsMap(const QMap<QString, QIcon> &iconsMap)
{
    m_iconsMap = iconsMap;
}

bool CTools::isRun() const
{
    return m_isRun;
}

void CTools::setIsRun(bool isRun)
{
    m_isRun = isRun;
}

QButtonGroup *CTools::buttonGroup() const
{
    return m_buttonGroup;
}

CToolButton *CTools::button(NameId id) const
{
    return qobject_cast<CToolButton*>(m_buttonGroup->button((int)id));
}

CToolButton *CTools::button(const QString &name) const
{
    for(int i = 0; i < m_buttonGroup->buttons().count();i++){
        QAbstractButton *button= m_buttonGroup->buttons().at(i);
        CToolButton *tButton = qobject_cast<CToolButton*> (button);
        if(tButton->buttonName() == name)
            return tButton;
    }
    return NULL;
}

QSize CTools::buttonSize() const
{
    return m_buttonSize;
}

void CTools::setButtonSize(const QSize &buttonSize)
{
    m_buttonSize = buttonSize;

    // resize the buttons
    for(int i = 0; i < m_buttonGroup->buttons().count();i++){
        QAbstractButton *button= m_buttonGroup->buttons().at(i);
        button->setFixedSize(m_buttonSize);
    }
}

QList<QString> CTools::defaultbuttonNames() const
{
    return m_defaultbuttonNames;
}

void CTools::setDefaultbuttonNames(const QList<QString> &defaultbuttonNames)
{
    m_defaultbuttonNames = defaultbuttonNames;
}

