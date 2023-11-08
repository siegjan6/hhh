#include <QtCore>
#include <QtGui>
#include <QAction>
#include <QDebug>
#include <QToolBar>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QListWidget>
#include <QPrinterInfo>

#include "MDHMIForm.h"
#include "ui_HMIForm.h"
#include "HMIFormData.h"
#include "Environment.h"
#include "Studio.h"
#include "Run.h"
#include "MDDrawRect.h"
#include "MDDrawText.h"
#include "ControlPointContainer.h"
#include "MDPropertyDrawText.h"
#include "../ControlPoint/PropertySelectObject.h"
#include "IHMIStudio.h"
#include "MDJavaScriptCodeEditor.h"
#include "MDActionUI.h"
#include "FormTypeDialog.h"
#include "MDActionSPUI.h"
#include "MDProjectConfiger.h"
#include "FormCommand.h"
#include "MDDrawObjectConfig.h"
#include "../../include/MDScriptEditor/MDScriptEditor.h"


//////////////////////////////////////////////////////////////////////////
void CMDHMIForm::setPropertyRect(const QRect &value)
{
    m_data->m_studio->undo()->push(new CFormCommand(this, rect(), value, (int)FormId::Rect));
    update();
    setModify(true);
}
QRect CMDHMIForm::rect()
{
    return m_data->m_common->rect();
}
void CMDHMIForm::setFormType(const QString& value)
{
    QList<QVariant> oldValue;
    oldValue.append((int)m_data->m_common->formType());
    oldValue.append(m_data->m_common->hasFrame());
    oldValue.append(m_data->m_common->fullScreen());

    CFormTypeDialog dlg(this);
    dlg.init(m_data->m_common->formType(), m_data->m_common->hasFrame(), m_data->m_common->fullScreen());
    if (dlg.exec() == QDialog::Accepted)
    {
        QList<QVariant> newValue;
        newValue.append((int)dlg.type());
        newValue.append(dlg.hasFrame());
        newValue.append(dlg.fullScreen());

        m_data->m_studio->undo()->push(
            new CFormCommand(this, oldValue, newValue, (int)FormId::FormType));

        setModify(true);
    }
}
QString CMDHMIForm::formType()
{
    switch (m_data->m_common->formType())
    {
    case FormType::Normal:
        return tr("子窗体");
    case FormType::Modal:
        return tr("模态窗体");
    case FormType::TopMost:
        return tr("顶层窗体");
    default:
        break;
    }

    return "";
}
void CMDHMIForm::setText(const QString &value)
{
    m_data->m_studio->undo()->push(
        new CFormCommand(this, m_data->m_common->text(), value, (int)FormId::Text));
    setModify(true);
}
QString CMDHMIForm::text()
{
    return m_data->m_common->text();
}
void CMDHMIForm::setBrush(const QString &/*value*/)
{
    CMDBrushManager* oldValue = new CMDBrushManager;
    *oldValue = *m_data->m_common->brush();

    if (m_data->m_common->brush()->showDialog())
    {
        CMDBrushManager* newValue = new CMDBrushManager;
        *newValue = *m_data->m_common->brush();

        m_data->m_studio->undo()->push(
            new CFormCommand(this, (qulonglong)oldValue, (qulonglong)newValue, (int)FormId::Brush));

        update();
        setModify(true);
    }
    else
        delete oldValue;
}
QString CMDHMIForm::brush()
{
    return m_data->m_common->brush()->text();
}

void CMDHMIForm::setMouseDown(const QString &value)
{
    changeActions((int)FormId::MouseDownAction);
    setModify(true);
}
QString CMDHMIForm::mouseDown()
{
    QString str;
    str.setNum(m_data->m_common->mouseDownActions()->list()->count());
    return str;
}
void CMDHMIForm::setMouseUp(const QString &value)
{
    changeActions((int)FormId::MouseUpAction);
    setModify(true);
}
QString CMDHMIForm::mouseUp()
{
    QString str;
    str.setNum(m_data->m_common->mouseUpActions()->list()->count());
    return str;
}
void CMDHMIForm::setRightMouseDown(const QString &value)
{
    changeActions((int)FormId::RightMosueDownAction);
    setModify(true);
}
QString CMDHMIForm::rightMouseDown()
{
    QString str;
    str.setNum(m_data->m_common->rightMouseDownActions()->list()->count());
    return str;
}
void CMDHMIForm::setRightMouseUp(const QString &value)
{
    changeActions((int)FormId::RightMouseUpAction);
    setModify(true);
}
QString CMDHMIForm::rightMouseUp()
{
    QString str;
    str.setNum(m_data->m_common->rightMouseUpActions()->list()->count());
    return str;
}
void CMDHMIForm::setOpenProperty(const QString &value)
{
    changeActions((int)FormId::OpenAction);
    setModify(true);
}
QString CMDHMIForm::openProperty()
{
    QString str;
    str.setNum(m_data->m_common->openActions()->list()->count());
    return str;
}
void CMDHMIForm::setCloseProperty(const QString &value)
{
    changeActions((int)FormId::CloseAction);
    setModify(true);
}
QString CMDHMIForm::closeProperty()
{
    QString str;
    str.setNum(m_data->m_common->closeActions()->list()->count());
    return str;
}
void CMDHMIForm::setConfigSecurity(const QString &value)
{
    CMDActionSPList srcList;
    srcList.append(new CMDActionSP(*m_data->m_common->openSecurity()));
    srcList.append(new CMDActionSP(*m_data->m_common->closeSecurity()));
    CMDActionSPList desList;

    QList<QVariant> oldValue;
    CMDActionSP* oldOpen = new CMDActionSP(*srcList.at(0));
    CMDActionSP* oldClose = new CMDActionSP(*srcList.at(1));
    oldValue.append((qulonglong)oldOpen);
    oldValue.append((qulonglong)oldClose);

    CMDActionSPUI dlg;
    dlg.init(framework()->projectConfiger()->powerGroupConfiger());
    if (dlg.doModalActionSPListDialog(srcList, desList))
    {
        CMDActionSP* sOpen = desList.find(MDActionType::window_Open);
        CMDActionSP* sClose = desList.find(MDActionType::window_Close);

        Q_ASSERT(sOpen != NULL);
        Q_ASSERT(sClose != NULL);

        QList<QVariant> newValue;
        CMDActionSP* newOpen = new CMDActionSP(*sOpen);
        CMDActionSP* newClose = new CMDActionSP(*sClose);
        newValue.append((qulonglong)newOpen);
        newValue.append((qulonglong)newClose);

        m_data->m_studio->undo()->push(
            new CFormCommand(this, oldValue, newValue, (int)FormId::Security));

        setModify(true);
    }
    else
    {
        delete oldOpen;
        delete oldClose;
    }
}
void CMDHMIForm::setVariable(const QString &/*value*/)
{
    QList<CMDExpInfo*>* list = m_data->m_common->expManager()->list();

    QDialog* dlg = CMDDrawObj::externalBindVarDialog(
        this, list, m_data->m_common);
    if (dlg->exec() == QDialog::Accepted)
        setModify(true);

    delete dlg;
}
QString CMDHMIForm::variable()
{
    QString str;
    return str.setNum(m_data->m_common->expManager()->expressionCount());
}
void CMDHMIForm::changeActions(int id)
{
    CMDHMIActionManager *manager = NULL;
    switch (id)
    {
    case (int)FormId::MouseDownAction:
        manager = m_data->m_common->mouseDownActions();
        break;
    case (int)FormId::MouseUpAction:
        manager = m_data->m_common->mouseUpActions();
        break;
    case (int)FormId::RightMosueDownAction:
        manager = m_data->m_common->rightMouseDownActions();
        break;
    case (int)FormId::RightMouseUpAction:
        manager = m_data->m_common->rightMouseUpActions();
        break;
    case (int)FormId::OpenAction:
        manager = m_data->m_common->openActions();
        break;
    case (int)FormId::CloseAction:
        manager = m_data->m_common->closeActions();
        break;
    default:
        break;
    }

    CMDActionUI action;
    action.setStudioInterface(framework()->studioProject());

    QList<QString>* actionList = manager->list();
    QStringList source(*actionList);
    QStringList target;

    CMDHMIActionManager* oldValue = new CMDHMIActionManager;
    oldValue->list()->append(source);

    if (action.selectActions(source, target))
    {
        CMDHMIActionManager* newValue = new CMDHMIActionManager;
        newValue->list()->append(target);

        m_data->m_studio->undo()->push(
            new CFormCommand(this, (qulonglong)oldValue, (qulonglong)newValue, id));
    }
    else
        delete oldValue;
}
void CMDHMIForm::initTranslateMap()
{
    m_translateMap.insert("GUID_Layout", tr("布局"));
    m_translateMap.insert("GUID_Appearance", tr("外观"));
    m_translateMap.insert("GUID_ActionEvent", tr("事件动作"));
    m_translateMap.insert("GUID_Security", tr("安全"));

    m_translateMap.insert("rect", tr("大小"));
    m_translateMap.insert("FormType", tr("窗体类型"));
    m_translateMap.insert("text", tr("文本"));
    m_translateMap.insert("brush", tr("背景"));
    m_translateMap.insert("mouseDown", tr("鼠标按下事件"));
    m_translateMap.insert("mouseUp", tr("鼠标弹起事件"));
    m_translateMap.insert("rightMouseDown", tr("鼠标右键按下事件"));
    m_translateMap.insert("rightMouseUp", tr("鼠标右键弹起事件"));
    m_translateMap.insert("open", tr("打开窗体事件"));
    m_translateMap.insert("close", tr("关闭窗体事件"));
    m_translateMap.insert("config", tr("配置"));

    m_translateMap.insert("GUID_Variable", tr("变量"));
    m_translateMap.insert("variable", tr("变量配置"));
}
//////////////////////////////////////////////////////////////////////////
CMDHMIForm::CMDHMIForm(IHMIFramework *framework, QWidget *parent) :
    CBaseForm(parent),
    ui(new Ui::HMIForm)
{
    m_data = new CHMIFormData;
    ui->setupUi(this);

    Q_ASSERT(framework != NULL);
    m_data->m_framework = framework;
    init();
    if (m_data->m_framework->isStudioMode())
    {
        m_data->m_studio = new CStudio(this);
        m_data->m_common = m_data->m_studio;

        initStudio();
    }
    else
    {
        m_data->m_run = new CRun(this);
        m_data->m_common = m_data->m_run;

        initRun();
    }

    //timer
    startTimer(80);
}
CMDHMIForm::~CMDHMIForm()
{
    delete ui;
}
IHMIFramework *CMDHMIForm::framework()
{
    return m_data->m_framework;
}
//////////////////////////////////////////////////////////////////////////
void CMDHMIForm::init()
{
    setNeedScroll(true);
    setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->statusBar->setMouseTracking(true);
    ui->menuBar->setVisible(false);
}
void CMDHMIForm::initStudio()
{
    //vector
    m_data->m_studio = new CStudio(this);
    m_data->m_common = m_data->m_studio;

    initMenu();
    initPopMenu();
    initToolBar();
    initStatusBar();
    initLanguage();
    initTranslateMap();
}
void CMDHMIForm::initPopMenu()
{
    m_data->m_popupMenu = new QMenu(this);
    QAction* copyAction = new QAction(m_data->sCopy, m_data->m_popupMenu);
    connect(copyAction, &QAction::triggered, this, &CMDHMIForm::copy);
    m_data->m_popupMenu->addAction(copyAction);
    QAction* pasteAction = new QAction(m_data->sPaste, m_data->m_popupMenu);
    connect(pasteAction, &QAction::triggered, this, &CMDHMIForm::paste);
    m_data->m_popupMenu->addAction(pasteAction);
    QAction* deleteAction = new QAction(m_data->sDelete, m_data->m_popupMenu);
    connect(deleteAction, &QAction::triggered, this, &CMDHMIForm::deleteObj);
    m_data->m_popupMenu->addAction(deleteAction);

    m_data->m_popupMenu->addSeparator();
    m_data->m_groupAction = new QAction(m_data->sGroup, m_data->m_popupMenu);
    m_data->m_groupAction->setIcon(QIcon(":/rs/image/group.png"));
    connect(m_data->m_groupAction, &QAction::triggered, this, &CMDHMIForm::group);
    m_data->m_popupMenu->addAction(m_data->m_groupAction);
    m_data->m_unGroupAction = new QAction(m_data->sUnGroup, m_data->m_popupMenu);
    m_data->m_unGroupAction->setIcon(QIcon(":/rs/image/unGroup.png"));
    connect(m_data->m_unGroupAction, &QAction::triggered, this, &CMDHMIForm::unGroup);
    m_data->m_popupMenu->addAction(m_data->m_unGroupAction);
    m_data->m_combineAction = new QAction(m_data->sCombine, m_data->m_popupMenu);
    m_data->m_combineAction->setIcon(QIcon(":/rs/image/combine.png"));
    connect(m_data->m_combineAction, &QAction::triggered, this, &CMDHMIForm::combine);
    m_data->m_popupMenu->addAction(m_data->m_combineAction);
    m_data->m_unCombineAction = new QAction(m_data->sUnCombine, m_data->m_popupMenu);
    m_data->m_unCombineAction->setIcon(QIcon(":/rs/image/unCombine.png"));
    connect(m_data->m_unCombineAction, &QAction::triggered, this, &CMDHMIForm::unCombine);
    m_data->m_popupMenu->addAction(m_data->m_unCombineAction);


    m_data->m_popupMenu->addSeparator();
    m_data->m_saveToStorageAction = new QAction(m_data->sSaveToStorage, m_data->m_popupMenu);
    connect(m_data->m_saveToStorageAction, &QAction::triggered, this, &CMDHMIForm::saveToStorage);
    m_data->m_popupMenu->addAction(m_data->m_saveToStorageAction);
}
void CMDHMIForm::initToolBar()
{
    m_data->m_toolBarList.append(new QToolBar(m_data->sToolBar, this));
    QToolBar* toolBar = m_data->m_toolBarList.at(0);

    //script
    m_data->m_scriptAction = new QAction(m_data->sScript, toolBar);
    m_data->m_scriptAction->setIcon(QIcon(":/rs/image/script.png"));
    connect(m_data->m_scriptAction, &QAction::triggered, this, &CMDHMIForm::script);
    toolBar->addAction(m_data->m_scriptAction);

    //graphics storage
    QAction* storageAction = new QAction(m_data->sStorage, toolBar);
    storageAction->setIcon(QIcon(":/rs/image/storage.png"));
    connect(storageAction, &QAction::triggered, this, &CMDHMIForm::storage);
    toolBar->addAction(storageAction);

    //form layout
    QAction* formLayoutAction = new QAction(m_data->sFormLayout, toolBar);
    formLayoutAction->setIcon(QIcon(":/rs/image/formLayout.png"));
    connect(formLayoutAction, &QAction::triggered, this, &CMDHMIForm::formLayout);
    toolBar->addAction(formLayoutAction);

    //batch edit
    m_data->m_batchEditAction = new QAction(m_data->sBatchEdit, toolBar);
    m_data->m_batchEditAction->setIcon(QIcon(":/rs/image/batchEdit.png"));
    connect(m_data->m_batchEditAction, &QAction::triggered, this, &CMDHMIForm::batchEdit);
    toolBar->addAction(m_data->m_batchEditAction);
    toolBar->addSeparator();

    //undo
    QAction* undoAction = m_data->m_studio->undo()->createUndoAction(toolBar);
    undoAction->setIcon(QIcon(":/rs/image/undo.png"));
    undoAction->setShortcut(tr("Ctrl+Z"));
    connect(undoAction, &QAction::triggered, this, &CMDHMIForm::undo);
    toolBar->addAction(undoAction);
    QAction* redoAction = m_data->m_studio->undo()->createRedoAction(toolBar);
    redoAction->setIcon(QIcon(":/rs/image/redo.png"));
    redoAction->setShortcut(tr("Ctrl+Y"));
    connect(redoAction, &QAction::triggered, this, &CMDHMIForm::redo);
    toolBar->addAction(redoAction);
    toolBar->addSeparator();

    //state
    m_data->m_orthoAction = new QAction(m_data->sOrtho, toolBar);
    m_data->m_orthoAction->setIcon(QIcon(":/rs/image/Ortho.png"));
    connect(m_data->m_orthoAction, &QAction::triggered, this, &CMDHMIForm::ortho);
    m_data->m_orthoAction->setCheckable(true);
    toolBar->addAction(m_data->m_orthoAction);
    m_data->m_gridAction = new QAction(m_data->sGrid, toolBar);
    m_data->m_gridAction->setIcon(QIcon(":/rs/image/Grid.png"));
    connect(m_data->m_gridAction, &QAction::triggered, this, &CMDHMIForm::grid);
    m_data->m_gridAction->setCheckable(true);
    toolBar->addAction(m_data->m_gridAction);
    initScale();
    //layer
    m_data->m_layerAction = new QAction(m_data->sLayer, toolBar);
    m_data->m_layerAction->setIcon(QIcon(":/rs/image/layer.png"));
    connect(m_data->m_layerAction, &QAction::triggered, this, &CMDHMIForm::layer);
    toolBar->addAction(m_data->m_layerAction);
    toolBar->addSeparator();
    //layout
    m_data->m_alignLeftAction = new QAction(m_data->sAlignLeft, toolBar);
    m_data->m_alignLeftAction->setIcon(QIcon(":/rs/image/AlignLeft.png"));
    connect(m_data->m_alignLeftAction, &QAction::triggered, this, &CMDHMIForm::alignLeft);
    toolBar->addAction(m_data->m_alignLeftAction);
    m_data->m_alignCenterAction = new QAction(m_data->sAlignCenter, toolBar);
    m_data->m_alignCenterAction->setIcon(QIcon(":/rs/image/AlignCenter.png"));
    connect(m_data->m_alignCenterAction, &QAction::triggered, this, &CMDHMIForm::alignCenter);
    toolBar->addAction(m_data->m_alignCenterAction);
    m_data->m_alignRightAction = new QAction(m_data->sAlignRight, toolBar);
    m_data->m_alignRightAction->setIcon(QIcon(":/rs/image/AlignRight.png"));
    connect(m_data->m_alignRightAction, &QAction::triggered, this, &CMDHMIForm::alignRight);
    toolBar->addAction(m_data->m_alignRightAction);
    toolBar->addSeparator();

    m_data->m_alignTopAction = new QAction(m_data->sAlignTop, toolBar);
    m_data->m_alignTopAction->setIcon(QIcon(":/rs/image/AlignTop.png"));
    connect(m_data->m_alignTopAction, &QAction::triggered, this, &CMDHMIForm::alignTop);
    toolBar->addAction(m_data->m_alignTopAction);
    m_data->m_alignMiddleAction = new QAction(m_data->sAlignMiddle, toolBar);
    m_data->m_alignMiddleAction->setIcon(QIcon(":/rs/image/AlignMiddle.png"));
    connect(m_data->m_alignMiddleAction, &QAction::triggered, this, &CMDHMIForm::alignMiddle);
    toolBar->addAction(m_data->m_alignMiddleAction);
    m_data->m_alignBottomAction = new QAction(m_data->sAlignBottom, toolBar);
    m_data->m_alignBottomAction->setIcon(QIcon(":/rs/image/AlignBottom.png"));
    connect(m_data->m_alignBottomAction, &QAction::triggered, this, &CMDHMIForm::alignBottom);
    toolBar->addAction(m_data->m_alignBottomAction);
    toolBar->addSeparator();

    m_data->m_sameWidthAction = new QAction(m_data->sSameWidth, toolBar);
    m_data->m_sameWidthAction->setIcon(QIcon(":/rs/image/SameWidth.png"));
    connect(m_data->m_sameWidthAction, &QAction::triggered, this, &CMDHMIForm::sameWidth);
    toolBar->addAction(m_data->m_sameWidthAction);
    m_data->m_sameHeightAction = new QAction(m_data->sSameHeight, toolBar);
    m_data->m_sameHeightAction->setIcon(QIcon(":/rs/image/SameHeight.png"));
    connect(m_data->m_sameHeightAction, &QAction::triggered, this, &CMDHMIForm::sameHeight);
    toolBar->addAction(m_data->m_sameHeightAction);
    m_data->m_sameSizeAction = new QAction(m_data->sSameSize, toolBar);
    m_data->m_sameSizeAction->setIcon(QIcon(":/rs/image/SameSize.png"));
    connect(m_data->m_sameSizeAction, &QAction::triggered, this, &CMDHMIForm::sameSize);
    toolBar->addAction(m_data->m_sameSizeAction);
    toolBar->addSeparator();

    m_data->m_hSpaceAction = new QAction(m_data->sHSpace, toolBar);
    m_data->m_hSpaceAction->setIcon(QIcon(":/rs/image/HSpace.png"));
    connect(m_data->m_hSpaceAction, &QAction::triggered, this, &CMDHMIForm::hSpace);
    toolBar->addAction(m_data->m_hSpaceAction);
    m_data->m_incrHSpaceAction = new QAction(m_data->sIncrHSpace, toolBar);
    m_data->m_incrHSpaceAction->setIcon(QIcon(":/rs/image/IncrHSpace.png"));
    connect(m_data->m_incrHSpaceAction, &QAction::triggered, this, &CMDHMIForm::incrHSpace);
    toolBar->addAction(m_data->m_incrHSpaceAction);
    m_data->m_decrHSpaceAction = new QAction(m_data->sDecrHSpace, toolBar);
    m_data->m_decrHSpaceAction->setIcon(QIcon(":/rs/image/DecrHSpace.png"));
    connect(m_data->m_decrHSpaceAction, &QAction::triggered, this, &CMDHMIForm::decrHSpace);
    toolBar->addAction(m_data->m_decrHSpaceAction);
    toolBar->addSeparator();

    m_data->m_vSpaceAction = new QAction(m_data->sVSpace, toolBar);
    m_data->m_vSpaceAction->setIcon(QIcon(":/rs/image/VSpace.png"));
    connect(m_data->m_vSpaceAction, &QAction::triggered, this, &CMDHMIForm::vSpace);
    toolBar->addAction(m_data->m_vSpaceAction);
    m_data->m_incrVSpaceAction = new QAction(m_data->sIncrVSpace, toolBar);
    m_data->m_incrVSpaceAction->setIcon(QIcon(":/rs/image/IncrVSpace.png"));
    connect(m_data->m_incrVSpaceAction, &QAction::triggered, this, &CMDHMIForm::incrVSpace);
    toolBar->addAction(m_data->m_incrVSpaceAction);
    m_data->m_decrVSpaceAction = new QAction(m_data->sDecrVSpace, toolBar);
    m_data->m_decrVSpaceAction->setIcon(QIcon(":/rs/image/DecrVSpace.png"));
    connect(m_data->m_decrVSpaceAction, &QAction::triggered, this, &CMDHMIForm::decrVSpace);
    toolBar->addAction(m_data->m_decrVSpaceAction);
    toolBar->addSeparator();

    //sequence of object
    m_data->m_topAction = new QAction(m_data->sTop, toolBar);
    m_data->m_topAction->setIcon(QIcon(":/rs/image/top.png"));
    connect(m_data->m_topAction, &QAction::triggered, this, &CMDHMIForm::top);
    toolBar->addAction(m_data->m_topAction);
    m_data->m_lastAction = new QAction(m_data->sLast, toolBar);
    m_data->m_lastAction->setIcon(QIcon(":/rs/image/last.png"));
    connect(m_data->m_lastAction, &QAction::triggered, this, &CMDHMIForm::last);
    toolBar->addAction(m_data->m_lastAction);
    m_data->m_frontAction = new QAction(m_data->sFront, toolBar);
    m_data->m_frontAction->setIcon(QIcon(":/rs/image/front.png"));
    connect(m_data->m_frontAction, &QAction::triggered, this, &CMDHMIForm::front);
    toolBar->addAction(m_data->m_frontAction);
    m_data->m_backAction = new QAction(m_data->sBack, toolBar);
    m_data->m_backAction->setIcon(QIcon(":/rs/image/back.png"));
    connect(m_data->m_backAction, &QAction::triggered, this, &CMDHMIForm::back);
    toolBar->addAction(m_data->m_backAction);
    toolBar->addSeparator();

    //flip
    m_data->m_flipXAction = new QAction(m_data->sFlipX, toolBar);
    m_data->m_flipXAction->setIcon(QIcon(":/rs/image/FlipX.png"));
    connect(m_data->m_flipXAction, &QAction::triggered, this, &CMDHMIForm::flipX);
    toolBar->addAction(m_data->m_flipXAction);
    m_data->m_flipYAction = new QAction(m_data->sFlipY, toolBar);
    m_data->m_flipYAction->setIcon(QIcon(":/rs/image/FlipY.png"));
    connect(m_data->m_flipYAction, &QAction::triggered, this, &CMDHMIForm::flipY);
    toolBar->addAction(m_data->m_flipYAction);
    toolBar->addSeparator();

    //group
    toolBar->addAction(m_data->m_groupAction);
    toolBar->addAction(m_data->m_unGroupAction);
    toolBar->addAction(m_data->m_combineAction);
    toolBar->addAction(m_data->m_unCombineAction);
    toolBar->addSeparator();

    this->addToolBar(toolBar);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    setToolBarEnabled();
}
void CMDHMIForm::initScale()
{
    QComboBox* combo = new QComboBox(this);
    QStringList list;
    list<<"400%"<<"200%"<<"150%"<<"100%"<<"75%"<<"50%"<<"25%";
    combo->addItems(list);
    combo->setCurrentIndex(3);//100%
    connect(combo, &QComboBox::currentTextChanged, this, &CMDHMIForm::scale);

    QAction* scaleAction = m_data->m_toolBarList.at(0)->addWidget(combo);
    scaleAction->setToolTip(m_data->sScale);
}
void CMDHMIForm::initStatusBar()
{
    QLabel* icoMouse = new QLabel(this);
    icoMouse->setPixmap(QPixmap(":/rs/image/mouse.png"));
    QLabel* icoLocation = new QLabel(this);
    icoLocation->setPixmap(QPixmap(":/rs/image/location1.png"));
    QLabel* icoSize = new QLabel(this);
    icoSize->setPixmap(QPixmap(":/rs/image/size.png"));

    ui->statusBar->addWidget(new QLabel(this), 100);
    ui->statusBar->addWidget(icoMouse);
    ui->statusBar->addWidget(&m_data->m_labelMouse, 1);
    ui->statusBar->addWidget(icoLocation);
    ui->statusBar->addWidget(&m_data->m_labelLocation, 1);
    ui->statusBar->addWidget(icoSize);
    ui->statusBar->addWidget(&m_data->m_labelSize, 1);
}
void CMDHMIForm::initRun()
{
    ui->statusBar->setVisible(false);
}
void CMDHMIForm::initMenu()
{
    QMenu* printMenu = new QMenu(this);
    printMenu->setTitle(tr("打印"));
    QAction* print = new QAction(tr("打印..."), printMenu);
    connect(print, &QAction::triggered, this, &CMDHMIForm::menuPrint);
    printMenu->addAction(print);
    QAction* printPreview = new QAction(tr("打印预览..."), printMenu);
    connect(printPreview, &QAction::triggered, this, &CMDHMIForm::printPreview);
    printMenu->addAction(printPreview);
    QAction* printConfig = new QAction(tr("打印设置..."), printMenu);
    connect(printConfig, &QAction::triggered, this, &CMDHMIForm::printConfig);
    printMenu->addAction(printConfig);

    QMenu* createMenu = CMDDrawObjectConfig::createMenu(this);

    m_data->m_menuList.append(createMenu);
    m_data->m_menuList.append(printMenu);
}

void CMDHMIForm::initLanguage()
{
    m_data->m_transMenu0 = CMDMultiLanguage::createWidgetTool();
    m_data->m_transMenu0->init(m_data->m_menuList.at(0));
    m_data->m_transMenu1 = CMDMultiLanguage::createWidgetTool();
    m_data->m_transMenu1->init(m_data->m_menuList.at(1));
    m_data->m_transPopMenu = CMDMultiLanguage::createWidgetTool();
    m_data->m_transPopMenu->init(m_data->m_popupMenu);
    m_data->m_transToolBar = CMDMultiLanguage::createWidgetTool();
    m_data->m_transToolBar->init(m_data->m_toolBarList.at(0));
}
//////////////////////////////////////////////////////////////////////////
void CMDHMIForm::paintEvent(QPaintEvent *e)
{
    if (m_data == NULL)
        return;

    QPainter p(this);
    m_data->m_common->paint(&p, e->rect());
}
void CMDHMIForm::mouseMoveEvent(QMouseEvent *e)
{
    if (m_data == NULL)
        return;

    m_data->m_mousePos = e->pos();
    m_data->m_common->mouseMove(e->button(), e->pos());
}
void CMDHMIForm::mousePressEvent(QMouseEvent *e)
{
    if (m_data == NULL)
        return;

    m_data->m_common->mouseDown(e->button(), e->pos());
}
void CMDHMIForm::mouseReleaseEvent(QMouseEvent *e)
{
    if (m_data == NULL)
        return;

    m_data->m_common->mouseUp(e->button(), e->pos());
}
void CMDHMIForm::timerEvent(QTimerEvent */*e*/)
{
    if (m_data == NULL)
        return;

    if (framework()->isStudioMode())
    {
        QString strX;
        strX.sprintf("%6d", m_data->m_mousePos.x());
        QString strY;
        strY.sprintf("%6d", m_data->m_mousePos.y());
        m_data->m_labelMouse.setText(strX + "," + strY);
    }else
    {
        m_data->m_run->timerEvent();
    }
}
void CMDHMIForm::closeEvent(QCloseEvent *e)
{
    CBaseForm::closeEvent(e);

    if (m_data == NULL)
        return;

    if (m_data->m_framework->isStudioMode())
    {
        if (isActived())
        {
            m_data->m_studio->resetSelectObjs(true);
            m_data->m_framework->manager()->setObjectTree(NULL);
        }
    }
    else
    {
        if (!m_data->m_run->checkSecurity(MDActionType::window_Close))
        {
            e->ignore();
            return;
        }
        m_data->m_run->closeEvent();
    }
    m_data->m_common->setWidgetParentNull();
    emit destroyed(this);

    if(m_data->m_framework->isStudioMode())
    {
        delete m_data->m_transMenu0;
        delete m_data->m_transMenu1;
        delete m_data->m_transPopMenu;
        delete m_data->m_transToolBar;
    }

    if (m_data->m_common != NULL)
    {
        delete m_data->m_common;
        m_data->m_common = NULL;
        m_data->m_run = NULL;
        m_data->m_studio = NULL;
    }

    if (m_data->m_editor != NULL)
    {
        delete m_data->m_editor;
        m_data->m_editor = NULL;
    }

    delete m_data;
    m_data = NULL;
}
void CMDHMIForm::keyPressEvent(QKeyEvent *e)
{
    if (m_data == NULL)
        return;

    m_data->m_common->keyPressEvent(e);
    CBaseForm::keyPressEvent(e);
}
void CMDHMIForm::changeEvent(QEvent *e)
{
    if (m_data == NULL)
        return;

    if (e->type() == QEvent::ActivationChange)
    {

    }

    QMainWindow::changeEvent(e);
}
void CMDHMIForm::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (m_data == NULL)
        return;

    if (framework()->isStudioMode())
        m_data->m_studio->mouseDoubleClickEvent(e->pos());
    else
        //双击时第二次MousePressEvent不会触发,故添加此代码
        mousePressEvent(e);
}
void CMDHMIForm::enterEvent(QEvent *e)
{
    if (m_data == NULL)
        return;

    if (!framework()->isStudioMode())
        m_data->m_run->onEnter();
}
void CMDHMIForm::leaveEvent(QEvent *e)
{
    if (m_data == NULL)
        return;

    if (!framework()->isStudioMode())
        m_data->m_run->loadFormLeaveEvent();
}
void CMDHMIForm::resizeEvent(QResizeEvent *e)
{
    if (m_data == NULL)
        return;

    if (!framework()->isStudioMode())
        m_data->m_common->resizeEvent(e->size());
}
void CMDHMIForm::focusInEvent(QFocusEvent *e)
{
    if (m_data == NULL)
        return;

    if (!framework()->isStudioMode())
        m_data->m_run->onGetFocuse();
}
void CMDHMIForm::focusOutEvent(QFocusEvent *e)
{
    if (m_data == NULL)
        return;

    if (!framework()->isStudioMode())
        m_data->m_run->onLostFocus();
}
void CMDHMIForm::showEvent(QShowEvent *e)
{
    if (m_data == NULL)
        return;

    if (!framework()->isStudioMode())
        m_data->m_run->onShow();
}
void CMDHMIForm::hideEvent(QHideEvent *e)
{
    if (m_data == NULL)
        return;

    if (!framework()->isStudioMode())
        m_data->m_run->onHide();
}
//////////////////////////////////////////////////////////////////////////
void CMDHMIForm::contextMenuEvent(QContextMenuEvent */*event*/)
{
    if (m_data->m_popupMenu == NULL)
        return;

    m_data->m_popupMenu->move(cursor().pos());
    m_data->m_popupMenu->show();
}
void CMDHMIForm::copy()
{
    m_data->m_studio->copy();
}
void CMDHMIForm::paste()
{
    m_data->m_studio->paste();
    setModify(true);
}
void CMDHMIForm::deleteObj()
{
    m_data->m_studio->deleteObj();

    setModify(true);
}
void CMDHMIForm::group()
{
    m_data->m_studio->group();
    setModify(true);
}
void CMDHMIForm::unGroup()
{
    m_data->m_studio->unGroup();
    setModify(true);
}
void CMDHMIForm::combine()
{
    m_data->m_studio->combine();
    setModify(true);
}
void CMDHMIForm::unCombine()
{
    m_data->m_studio->unCombine();
    setModify(true);
}
void CMDHMIForm::saveToStorage()
{
    m_data->m_studio->saveToStorage();
}
void CMDHMIForm::saveToStoragePath()
{
    m_data->m_studio->saveToStoragePath();
}
//////////////////////////////////////////////////////////////////////////
void CMDHMIForm::ortho()
{
    m_data->m_studio->ortho();
    m_data->m_orthoAction->setChecked(m_data->m_studio->isOrtho());
}
void CMDHMIForm::grid()
{
    m_data->m_studio->grid();
    m_data->m_gridAction->setChecked(m_data->m_studio->isGrid());
}
void CMDHMIForm::scale(const QString &text)
{
    m_data->m_studio->scale(text);
}
void CMDHMIForm::layer()
{
    m_data->m_studio->layer();
}
void CMDHMIForm::alignLeft()
{
    m_data->m_studio->alignLeft();
    setModify(true);
}
void CMDHMIForm::alignCenter()
{
    m_data->m_studio->alignCenter();
    setModify(true);
}
void CMDHMIForm::alignRight()
{
    m_data->m_studio->alignRight();
    setModify(true);
}
void CMDHMIForm::alignTop()
{
    m_data->m_studio->alignTop();
    setModify(true);
}
void CMDHMIForm::alignMiddle()
{
    m_data->m_studio->alignMiddle();
    setModify(true);
}
void CMDHMIForm::alignBottom()
{
    m_data->m_studio->alignBottom();
    setModify(true);
}
void CMDHMIForm::sameWidth()
{
    m_data->m_studio->sameWidth();
    setModify(true);
}
void CMDHMIForm::sameHeight()
{
    m_data->m_studio->sameHeight();
    setModify(true);
}
void CMDHMIForm::sameSize()
{
    m_data->m_studio->sameSize();
    setModify(true);
}
void CMDHMIForm::hSpace()
{
    m_data->m_studio->hSpace();
    setModify(true);
}
void CMDHMIForm::incrHSpace()
{
    m_data->m_studio->incrHSpace();
    setModify(true);
}
void CMDHMIForm::decrHSpace()
{
    m_data->m_studio->decrHSpace();
    setModify(true);
}
void CMDHMIForm::vSpace()
{
    m_data->m_studio->vSpace();
    setModify(true);
}
void CMDHMIForm::incrVSpace()
{
    m_data->m_studio->incrVSpace();
    setModify(true);
}
void CMDHMIForm::decrVSpace()
{
    m_data->m_studio->decrVSpace();
    setModify(true);
}
void CMDHMIForm::top()
{
    m_data->m_studio->sequenceFun(CStudio::SequenceType::Top);
}
void CMDHMIForm::last()
{
    m_data->m_studio->sequenceFun(CStudio::SequenceType::Last);
}
void CMDHMIForm::front()
{
    m_data->m_studio->sequenceFun(CStudio::SequenceType::Front);
}
void CMDHMIForm::back()
{
    m_data->m_studio->sequenceFun(CStudio::SequenceType::Back);
}
void CMDHMIForm::flipX()
{
    m_data->m_studio->flipX();
    setModify(true);
}
void CMDHMIForm::flipY()
{
    m_data->m_studio->flipY();
    setModify(true);
}
void CMDHMIForm::script()
{
//#ifdef __linux__
//    if (m_data->m_editor == nullptr)
//    {
//        m_data->m_editor = new CMDJavaScriptCodeEditor("");
//        connect(m_data->m_editor, &CMDJavaScriptCodeEditor::signalSave, this, &CMDHMIForm::saveEditorEvent);
//        connect(m_data->m_editor, &CMDJavaScriptCodeEditor::signalClose, this, &CMDHMIForm::closeEditorEvent);
//    }

//    m_data->m_editor->importText(" ", m_data->m_common->scriptCode());
//    m_data->m_editor->onShow();
//#elif _WIN32
    CMDScriptEditor editor;
    QList<QPair<QString, QString>> objs;
    m_data->m_common->fillObjectToScript(objs);
    QString code = m_data->m_common->scriptCode();

    int result = editor.showDialog(code, objs, this);
    if(result == QDialog::Accepted)
    {
        m_data->m_common->setScriptCode(code);
        setModify(true);
    }
    CMDScriptEditor::cleanUp();
//#endif
}
void CMDHMIForm::storage()
{
    m_data->m_studio->storage();
}
void CMDHMIForm::formLayout()
{
    m_data->m_studio->formLayout();
}
void CMDHMIForm::undo()
{
    m_data->m_studio->undoRefresh();
}
void CMDHMIForm::redo()
{
    m_data->m_studio->undoRefresh();
}
void CMDHMIForm::batchEdit()
{
    m_data->m_studio->batchEdit();
}
void CMDHMIForm::setToolBarEnabled()
{
    CSelectObjectManager* selectOjbs = m_data->m_studio->controlPoint()->selectObjs();
    int count = selectOjbs->list()->count();
    bool isVector = selectOjbs->isVector();
    bool isSingleSelect = (count == 1);
    bool isMultiSelect = (count > 1);
    bool isSelect = (count > 0);
    bool isSelectVector = isSelect && isVector;
    bool existGroup = selectOjbs->existGroup();
    bool isSingleVector = isVector && isSingleSelect;
    bool canCombine = selectOjbs->canCombine();
    bool existCombine = selectOjbs->existCombine();

    m_data->m_topAction->setEnabled(isSingleSelect);
    m_data->m_lastAction->setEnabled(isSingleSelect);
    m_data->m_frontAction->setEnabled(isSingleVector);
    m_data->m_backAction->setEnabled(isSingleVector);

    m_data->m_alignLeftAction->setEnabled(isMultiSelect);
    m_data->m_alignCenterAction->setEnabled(isMultiSelect);
    m_data->m_alignRightAction->setEnabled(isMultiSelect);
    m_data->m_alignTopAction->setEnabled(isMultiSelect);
    m_data->m_alignMiddleAction->setEnabled(isMultiSelect);
    m_data->m_alignBottomAction->setEnabled(isMultiSelect);
    m_data->m_sameWidthAction->setEnabled(isMultiSelect);
    m_data->m_sameHeightAction->setEnabled(isMultiSelect);
    m_data->m_sameSizeAction->setEnabled(isMultiSelect);
    m_data->m_hSpaceAction->setEnabled(isMultiSelect);
    m_data->m_incrHSpaceAction->setEnabled(isMultiSelect);
    m_data->m_decrHSpaceAction->setEnabled(isMultiSelect);
    m_data->m_vSpaceAction->setEnabled(isMultiSelect);
    m_data->m_incrVSpaceAction->setEnabled(isMultiSelect);
    m_data->m_decrVSpaceAction->setEnabled(isMultiSelect);

    m_data->m_flipXAction->setEnabled(isSelectVector);
    m_data->m_flipYAction->setEnabled(isSelectVector);

    m_data->m_groupAction->setEnabled(isMultiSelect);
    m_data->m_unGroupAction->setEnabled(existGroup);
    m_data->m_combineAction->setEnabled(canCombine);
    m_data->m_unCombineAction->setEnabled(existCombine);

    m_data->m_saveToStorageAction->setEnabled(isSelect);
    m_data->m_batchEditAction->setEnabled(isMultiSelect);
}
//////////////////////////////////////////////////////////////////////////
IEnvironment *CMDHMIForm::common()
{
    return m_data->m_common;
}
IStudio *CMDHMIForm::studio()
{
    return m_data->m_studio;
}
IRun *CMDHMIForm::run()
{
    return m_data->m_run;
}
void CMDHMIForm::setRect(const QRect &value)
{
    m_data->m_common->setRect(value);
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::setModify(bool value)
{
    if (framework()->isStudioMode())
        CBaseForm::setModify(value);
}
/////////////////////////////////////////////////////////////////////////
bool CMDHMIForm::save()
{
    return m_data->m_common->save();
}
bool CMDHMIForm::load(const QString &fullName, bool isBackground/* = false*/)
{
    bool result = m_data->m_common->load(fullName);

    //后台打开时不需要设置焦点
    if (result && !isBackground)
        setActive(true);

    return result;
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::createObjectState()
{
    QObject* o = sender();
    if(o == NULL)
        return;

    DrawType type = (DrawType)(o->property("type").toInt());
    m_data->m_studio->setCreateObjectState(type);
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::setActive(bool active)
{
    CBaseForm::setActive(active);

    if (m_data == NULL)
        return;
    if (!m_data->m_framework->isStudioMode())
        return;

    //此函数仅在studio环境中被调用
    m_data->m_studio->resetSelectObjs(!active);
    m_data->m_framework->manager()->toolKitDialog()->setEnabled(active);
    if (active)
    {
        m_data->m_framework->manager()->setCurrentForm(this);

        m_data->m_framework->manager()->setObjectTree(this);
        setFocus();
    }
    else
    {
        m_data->m_framework->manager()->setCurrentForm(NULL);
        m_data->m_framework->manager()->setObjectTree(NULL);
        m_data->m_studio->create()->end();
    }
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::clearCopyList()
{
    CStudio::clearCopyList();
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::setStatusBarText(const QString &location, const QString &size)
{
    m_data->m_labelLocation.setText(location);
    m_data->m_labelSize.setText(size);
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::print(bool showPrintDialog)
{
    m_data->m_common->print(showPrintDialog);
}
void CMDHMIForm::printPreview()
{
    QPrinter print;
    m_data->m_common->setPrintMargins(&print);
    QPrintPreviewDialog dlg(&print, this);
    connect(&dlg, &QPrintPreviewDialog::paintRequested, this, &CMDHMIForm::printFun);
    dlg.exec();
}
void CMDHMIForm::printConfig()
{
    m_data->m_common->printConfig();
}
void CMDHMIForm::printRegion(const QRect &rect,  bool showPrintDialog, bool isFullPage)
{
    m_data->m_common->printRegion(rect, showPrintDialog, isFullPage);
}
void CMDHMIForm::printObject(const QString &name, bool showPrintDialog, bool isFullPage)
{
    m_data->m_common->printObject(name, showPrintDialog, isFullPage);
}
void CMDHMIForm::printFun(QPrinter *printer)
{
    m_data->m_common->directPrint(printer);
}
void CMDHMIForm::menuPrint()
{
    print();
}
/////////////////////////////////////////////////////////////////////////
QList<QMenu *> *CMDHMIForm::menus() const
{
    if(m_data == NULL)
        return NULL;

    return &m_data->m_menuList;
}
QList<QToolBar*> *CMDHMIForm::toolBars() const
{
    if(m_data == NULL)
        return NULL;

    return &m_data->m_toolBarList;
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::saveEditorEvent()
{
    m_data->m_common->setScriptCode(m_data->m_editor->text());
    setModify(true);
}
void CMDHMIForm::closeEditorEvent()
{
    if (m_data->m_editor == NULL)
        return;

    m_data->m_editor->deleteLater();
    m_data->m_editor = NULL;
}
/////////////////////////////////////////////////////////////////////////
CMDDrawObj* CMDHMIForm::createDrawObj(DrawType type)
{
    return CDrawCreator::createDrawObj(type);
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::setCreateObjectState(DrawType type)
{
    m_data->m_studio->setCreateObjectState(type);
}
void CMDHMIForm::endCreate()
{
    m_data->m_studio->create()->end();
}
/////////////////////////////////////////////////////////////////////////
void CMDHMIForm::changeLanguage()
{
    m_data->m_common->changeLanguage();

    if(m_data->m_framework->isStudioMode())
    {
        m_data->m_transMenu0->changeLanguage();
        m_data->m_transMenu1->changeLanguage();
        m_data->m_transPopMenu->changeLanguage();
        m_data->m_transToolBar->changeLanguage();
    }

    update();
}
void CMDHMIForm::collectWords()
{
    m_data->m_studio->collectWords();
}
/////////////////////////////////////////////////////////////////////////




