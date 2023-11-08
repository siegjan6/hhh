#include "NewDataBaseDialog.h"
#include "ui_NewDataBaseDialog.h"
#include "NewViewVarDialog.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <MDDBConnect/MDDBConnect.h>

NewDataBaseDialog::NewDataBaseDialog(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDataBaseDialog),
    m_windowTitle(title)
{
    ui->setupUi(this);
    ui->lineEdit_dbPassword->setEchoMode(QLineEdit::Password);
    this->setWindowTitle(m_windowTitle);
    QWidget::setTabOrder(ui->lineEdit_dbName, ui->pbt_connectTest);
    QWidget::setTabOrder(ui->pbt_connectTest, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_dbName);
}

NewDataBaseDialog::~NewDataBaseDialog()
{
    delete ui;
}

void NewDataBaseDialog::setDatabaseParameter(DBQ::DBSource *dbsource)
{
    ui->comboBox_dbType->setCurrentIndex((int)dbsource->dbType-1);
    ui->lineEdit_dbHost->setText(dbsource->host);
    ui->lineEdit_dbPort->setText(QString::number(dbsource->port,10));
    ui->lineEdit_dbUserName->setText(dbsource->userName);
    ui->lineEdit_dbPassword->setText(dbsource->password);
}

QString NewDataBaseDialog::getDBName()
{
    m_DBName = ui->lineEdit_dbName->text();
    return m_DBName;
}

void NewDataBaseDialog::setDBName(QString &dbName)
{
    ui->lineEdit_dbName->setText(dbName);
    m_DBName = dbName;
}

void NewDataBaseDialog::on_pbt_Confirm_clicked()
{
    QRegExp newdatabase_reg_exp;
    newdatabase_reg_exp.setPattern("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]{1,30}$");
    QRegExpValidator p_newdatabase_validator(newdatabase_reg_exp);

    if(p_newdatabase_validator.regExp().exactMatch(ui->lineEdit_dbName->text()))
    {
        m_DBName = ui->lineEdit_dbName->text();
    }
    else
    {
        m_DBName = "";
        QMessageBox::information(NULL,"警告",tr("数据库名称不正确!"));
        return;
    }
    this->accept();
}

void NewDataBaseDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}

void NewDataBaseDialog::on_pbt_connectTest_clicked()
{
    MDDBConnection conn;
    conn.setDBType(DBQ::DataSource_GetDBType(ui->comboBox_dbType->currentText()));
    conn.setDBName(ui->lineEdit_dbName->text());
    conn.setHost(ui->lineEdit_dbHost->text());
    conn.setPort(ui->lineEdit_dbPort->text().toInt());
    conn.setUserName(ui->lineEdit_dbUserName->text());
    conn.setPassword(ui->lineEdit_dbPassword->text());

    bool b = conn.open();
    //TODO:open失败时,获得出错信息，并在对话框中显示。
    if(b)
        QMessageBox::information( this, tr("Connection OK"), tr("Connection succeded!") );
    else
        QMessageBox::warning( this, tr("Connection error"),  tr("Connection error") );
}
