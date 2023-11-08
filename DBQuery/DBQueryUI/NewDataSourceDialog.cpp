#include "NewDataSourceDialog.h"
#include "ui_NewDataSourceDialog.h"
#include <QLineEdit>
#include <QMessageBox>

NewDataSourceDialog::NewDataSourceDialog(QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDataSourceDialog),
    m_DBSource(new DBQ::DBSource),
    m_windowTitle(title)
{
    ui->setupUi(this);
    ui->lineEdit_dbPassword->setEchoMode(QLineEdit::Password);
    this->setWindowTitle(m_windowTitle);
    QWidget::setTabOrder(ui->lineEdit_DataSrcName, ui->comboBox_dbType);
    QWidget::setTabOrder(ui->comboBox_dbType, ui->lineEdit_dbServerName);
    QWidget::setTabOrder(ui->lineEdit_dbServerName, ui->lineEdit_dbServerIP);
    QWidget::setTabOrder(ui->lineEdit_dbServerIP, ui->lineEdit_dbServerPort);
    QWidget::setTabOrder(ui->lineEdit_dbServerPort, ui->lineEdit_dbUser);
    QWidget::setTabOrder(ui->lineEdit_dbUser, ui->lineEdit_dbPassword);
    QWidget::setTabOrder(ui->lineEdit_dbPassword, ui->pbt_Confirm);
    QWidget::setTabOrder(ui->pbt_Confirm, ui->pbt_Cancel);
    QWidget::setTabOrder(ui->pbt_Cancel, ui->lineEdit_DataSrcName);
}

NewDataSourceDialog::~NewDataSourceDialog()
{
    delete ui;
    if(m_DBSource)
    {
        delete m_DBSource;
        m_DBSource = NULL;
    }
}

void NewDataSourceDialog::on_pbt_Confirm_clicked()
{
    QRegExp newdbsource_reg_exp;
    newdbsource_reg_exp.setPattern("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]{1,30}$");
    QRegExpValidator p_newdbsource_validator(newdbsource_reg_exp);

    if(p_newdbsource_validator.regExp().exactMatch(ui->lineEdit_DataSrcName->text()))
    {
        m_DBSource->dbSrcName = ui->lineEdit_DataSrcName->text();
    }
    else
    {
        m_DBSource->dbSrcName = "";
        QMessageBox::information(NULL,"警告",tr("数据源名称格式不正确!"));
        return;
    }

    QRegExp regExpIP("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    QRegExpValidator p_newdbsourceIP_validator(regExpIP);
    if(p_newdbsourceIP_validator.regExp().exactMatch(ui->lineEdit_dbServerIP->text()))
    {
        m_DBSource->host = ui->lineEdit_dbServerIP->text();
    }
    else
    {
        m_DBSource->host = "";
        QMessageBox::information(NULL,"警告",tr("数据源IP格式不正确!"));
        return;
    }

    m_DBSource->tablePrefix = m_TablePrefix;
    /*
    QRegExp regExpTablePrefix("^[a-zA-Z0-9_\u4e00-\u9fa5\\w]{0,30}$");
    QRegExpValidator p_newTablePrefix_validator(regExpTablePrefix);
    if(p_newTablePrefix_validator.regExp().exactMatch(ui->lineEdit_tablePrefix->text()))
    {
        m_DBSource->tablePrefix = ui->lineEdit_tablePrefix->text();
    }
    else
    {
        m_DBSource->tablePrefix = "";
        QMessageBox::information(NULL,"警告",tr("数据源表前缀格式不正确!"));
        return;
    }
    */

    this->accept();
}

void NewDataSourceDialog::on_pbt_Cancel_clicked()
{
    this->reject();
}
DBQ::DBSource *NewDataSourceDialog::getDBSource() const
{
    return m_DBSource;
}

void NewDataSourceDialog::getDataSourceParameter()
{
    m_DBSource->bDefault = false;
    m_DBSource->dbSrcName = ui->lineEdit_DataSrcName->text();
    m_DBSource->dbServerName = ui->lineEdit_dbServerName->text();

    QString DBTypeText = ui->comboBox_dbType->currentText();
    if(DBTypeText.compare("MYSQL", Qt::CaseInsensitive) == 0)
        m_DBSource->dbType = DBSOURCE_TYPE_MYSQL;
    else if(DBTypeText.compare("SQLSERVER", Qt::CaseInsensitive) == 0)
    {
        m_TablePrefix = "dbo";
        m_DBSource->dbType =  DBSOURCE_TYPE_SQL_SERVER;
    }
    else if(DBTypeText.compare("ORACLE", Qt::CaseInsensitive) == 0)
        m_DBSource->dbType =  DBSOURCE_TYPE_ORACLE;
    else if(DBTypeText.compare("KINGBASE", Qt::CaseInsensitive) == 0)
    {
        m_TablePrefix = "public";
        m_DBSource->dbType =  DBSOURCE_TYPE_KINGBASE;
    }
    else
        m_DBSource->dbType = DBSOURCE_TYPE_UNKNOWN;

    m_DBSource->tablePrefix = m_TablePrefix;
    m_DBSource->host = ui->lineEdit_dbServerIP->text();
    m_DBSource->port = ui->lineEdit_dbServerPort->text().toInt();
    m_DBSource->userName = ui->lineEdit_dbUser->text();
    m_DBSource->password = ui->lineEdit_dbPassword->text();
}

void NewDataSourceDialog::setOriginalDSParameter(DBQ::DBSource *DBSource)
{
    ui->lineEdit_DataSrcName->setText(DBSource->dbSrcName);
    ui->lineEdit_dbServerName->setText(DBSource->dbServerName);
    ui->comboBox_dbType->setCurrentIndex((int)DBSource->dbType-1);
    ui->lineEdit_dbServerIP->setText(DBSource->host);
    ui->lineEdit_dbServerPort->setText(QString::number(DBSource->port, 10));
    ui->lineEdit_dbUser->setText(DBSource->userName);
    ui->lineEdit_dbPassword->setText(DBSource->password);
}

void NewDataSourceDialog::on_comboBox_dbType_currentIndexChanged(const QString & currentText)
{
    if(currentText.compare("SQLSERVER", Qt::CaseInsensitive) == 0)
        m_TablePrefix = "dbo";
    else if(currentText.compare("KINGBASE", Qt::CaseInsensitive) == 0)
        m_TablePrefix = "public";
    else
        m_TablePrefix = "";
}
