#include "MDLoadProgressDialog.h"
#include "ui_MDLoadProgressDialog.h"
#include <QTimer>

CMDLoadProgressDialog::CMDLoadProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDLoadProgressDialog)
{
    ui->setupUi(this);

    QTimer* timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->setInterval(1000);
    timer->start();
}

CMDLoadProgressDialog::~CMDLoadProgressDialog()
{
    delete ui;
}

void CMDLoadProgressDialog::onLoadProgress(const QString& msg, bool finished, int finishCode)
{
    m_msg = msg;

    if(finished)
    {
        setResult(QDialog::Accepted);
        accept();
    }
}

void CMDLoadProgressDialog::update()
{
    ui->msgLabel->setText(m_msg);
}
