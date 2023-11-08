#include "MDEngineMgrDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QStackedWidget>
#include <QListWidget>
#include <QCloseEvent>
#include "../../../include/MDMultiLanguage/MDMultiLanguage.h"
#include "MDEngineGeneralDialog.h"
#include "../../../include/MDCommandQueueEngine/MDCommandQueueEngine.h"
#include "MDCommandQueueEngineMgr.h"

const QString STRING_CLOSE = "关闭";

CMDEngineMgrDialog::CMDEngineMgrDialog(QWidget *parent) :
    QDialog(parent)
{
    m_listWidget = NULL;
    m_stackWidget = NULL;

    closeButton = NULL;

    initialUI();
    m_twTool = CMDMultiLanguage::instance()->createWidgetTool();
}

CMDEngineMgrDialog::~CMDEngineMgrDialog()
{
    if(m_twTool)
        delete m_twTool;
}

void CMDEngineMgrDialog::initialUI()
{
    m_listWidget =new QListWidget(this);

    m_stackWidget =new QStackedWidget(this);
    m_stackWidget->setFrameStyle(QFrame::Panel|QFrame::Raised);
    m_stackWidget->addWidget(new CMDEngineGeneralDialog);

    QHBoxLayout *mainLayout =new QHBoxLayout();
    mainLayout->setMargin(1);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(m_listWidget);
    mainLayout->addWidget(m_stackWidget);
    mainLayout->setStretchFactor(m_listWidget,8);
    mainLayout->setStretchFactor(m_stackWidget,10);

    closeButton=new QPushButton(STRING_CLOSE);

    QHBoxLayout *BtnLayout =new QHBoxLayout();
    BtnLayout->addStretch(1);
    BtnLayout->addWidget(closeButton);

    QVBoxLayout *RightLayout =new QVBoxLayout(this);
    RightLayout->setMargin(5);
    RightLayout->setSpacing(5);
    RightLayout->addLayout(mainLayout);
    RightLayout->addLayout(BtnLayout);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(on_closeButton_clicked()));

    int width = 800;
    int height = 400;
    setFixedSize(width,height); //设置窗体固定大小
}

void CMDEngineMgrDialog::on_engine_changed(int index)
{
    if(-1 == index)
        return;

    CMDEngineGeneralDialog* dlg = (CMDEngineGeneralDialog*)m_stackWidget->currentWidget();
    dlg->stopUpdate();
    dlg->init(CMDCommandQueueEngineMgr::engineAt(index));
    dlg->startUpdate();
}

void CMDEngineMgrDialog::updateEngines()
{
    m_listWidget->clear();

    QList<CMDCommandQueueEngine*> engineList;
    CMDCommandQueueEngineMgr::engines(engineList);
    foreach (CMDCommandQueueEngine* e, engineList)
    {
        QString comment = e->comment();
        if(comment.isEmpty())
            comment = QString("%1").arg((qint64)e);

        m_listWidget->addItem(comment);
    }
    m_listWidget->setCurrentRow(0);
}

/*!
\section 处理对 OK 和 Cancel 按钮的点击
*/

void CMDEngineMgrDialog::on_closeButton_clicked()
{
    closeUI();
}

void CMDEngineMgrDialog::showUI()
{
    connect(m_listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(on_engine_changed(int)));

    setWindowTitle("CommandQueueEngine Mnager");
    setModal(false);
    showNormal();

    updateEngines();
}

void CMDEngineMgrDialog::closeUI()
{
    CMDEngineGeneralDialog* dlg = (CMDEngineGeneralDialog*)m_stackWidget->currentWidget();
    dlg->stopUpdate();

    disconnect(m_listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(on_engine_changed(int)));
    m_listWidget->clear();

    close();
}

void CMDEngineMgrDialog::closeEvent(QCloseEvent *event)
{
    closeUI();

    event->accept();
}
