#include "MDBatchModifyWidget.h"
#include "ui_MDBatchModifyWidget.h"

CMDBatchModifyWidget::CMDBatchModifyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMDBatchModifyWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("批量修改"));
    initUI();
    connect(ui->closePushButton,SIGNAL(clicked()),this,SLOT(closeWindow()));
    connect(ui->modifyPushButton,SIGNAL(clicked()),this,SLOT(setModify()));
}

CMDBatchModifyWidget::~CMDBatchModifyWidget()
{
    delete ui;
}

void CMDBatchModifyWidget::setObjectModify(QObject *empty)
{
    m_object = empty;

}

void CMDBatchModifyWidget::initUI()
{
    ui->progressBar->setValue(0);
    ui->stateLabel->setText("");
    ui->progressLabel->setText("0/0");
    ui->problemLabel->setText("(0)");
    createDock();
}

void CMDBatchModifyWidget::createDock()
{
//    ui->dockWidget->setWidget(m_propertyBrowser);
//    ui->dockWidget->setWindowTitle(tr("属性栏"));

}

void CMDBatchModifyWidget::closeWindow()
{
    close();
}

void CMDBatchModifyWidget::setModify()
{

}
