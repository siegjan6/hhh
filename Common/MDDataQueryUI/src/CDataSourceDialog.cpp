#include "CDataSourceDialog.h"
#include "ui_CDataSourceDialog.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
CDataSourceDialog::CDataSourceDialog(QWidget *parent, bool isReport) :
    QDialog(parent),
    ui(new Ui::CDataSourceDialog)
{
    ui->setupUi(this);

    setFixedSize(width(),height());
    addSqlDriverItems();
    setReportUI(isReport);

    setWindowTitle(tr("数据源设置"));
}

CDataSourceDialog::~CDataSourceDialog()
{
    delete ui;
}

void CDataSourceDialog::setHostName(const QString &name)
{
    ui->databaseLineEdit->setText(name);
}

void CDataSourceDialog::setDriver(const QString &driver)
{
    QMapIterator<QString,QString> iter(m_drivers);
    while(iter.hasNext()){
        iter.next();
        if(!QString::compare(driver,iter.key(),Qt::CaseInsensitive) ||
                !QString::compare(driver,iter.value(),Qt::CaseInsensitive) )
            break;
    }
    ui->sqlDriverComboBox->setCurrentText(iter.key());
}

void CDataSourceDialog::setIP(const QString &IP)
{
    ui->IPLineEdit->setText(IP);
}

void CDataSourceDialog::setUserName(const QString &user)
{
    ui->userNameLineEdit->setText(user);
}

void CDataSourceDialog::setPassword(const QString &text)
{
    ui->passwordLineEdit->setText(text);
}

void CDataSourceDialog::setPort(const QString &port)
{
    ui->portLineEdit->setText(port);
}

QString CDataSourceDialog::hostName() const
{
    return ui->databaseLineEdit->text();
}

QString CDataSourceDialog::driver() const
{
    return ui->sqlDriverComboBox->currentText();
}
QString CDataSourceDialog::IP() const
{
    return ui->IPLineEdit->text();
}
QString CDataSourceDialog::userName() const
{
    return ui->userNameLineEdit->text();
}
QString CDataSourceDialog::password() const
{
    return ui->passwordLineEdit->text();
}
QString CDataSourceDialog::port() const
{
    return ui->portLineEdit->text();
}

bool CDataSourceDialog::isDatabaseEnabled() const
{
    return ui->enableCheckBox->isChecked();
}

void CDataSourceDialog::on_confirmButton_clicked()
{
    accept();
}

void CDataSourceDialog::on_cancelButton_clicked()
{
    reject();
}

void CDataSourceDialog::on_testConnectButton_clicked()
{
    bool ok = true;
    QString msg;

    QSqlDatabase database = QSqlDatabase::addDatabase(driverTrans(ui->sqlDriverComboBox->currentText()), "external_test" );
    ok = database.isValid();
    if ( !ok )
        msg = tr("Could not load %1 database driver.").arg(ui->sqlDriverComboBox->currentText());

    if (ok) {
        database.setHostName( ui->IPLineEdit->text() );
        database.setDatabaseName(ui->databaseLineEdit->text());
        database.setUserName( ui->userNameLineEdit->text());
        database.setPassword( ui->passwordLineEdit->text());
        if ( ui->portLineEdit->text().toInt() != 0 )
            database.setPort( ui->portLineEdit->text().toInt() );

        ok = database.open();

        if ( ok ){
            database.close();
        }
        else{
            msg = database.lastError().databaseText();
        }
    }

    QSqlDatabase::removeDatabase("external_test");

    if ( ok )
        QMessageBox::information( this, tr("Connection OK"), tr("Connection succeded!") );
    else
        QMessageBox::warning( this, tr("Connection error"), msg );

}
void CDataSourceDialog::on_enableCheckBox_clicked()
{
    if(ui->enableCheckBox->isChecked()){
        setDatabaseUI(true);
    }else{
        setDatabaseUI(false);
    }
}
void CDataSourceDialog::slotDataSourceChanged(const QString &sourceName)
{
    enum{Count = 6};

    QStringList sourceInfor = m_dataSourceInfor.value(sourceName);

    if(sourceInfor.isEmpty() || sourceInfor.count() < Count)
        return;

    setHostName(sourceInfor.at((int)Infor::Database));
    setDriver(sourceInfor.at((int)Infor::Driver));
    setIP(sourceInfor.at((int)Infor::IP));
    setPort(sourceInfor.at((int)Infor::Port));
    setUserName(sourceInfor.at((int)Infor::UserName));
    setPassword(sourceInfor.at((int)Infor::Password));
//    setFilter(sourceInfor.at((int)Infor::Filter));
}

void CDataSourceDialog::slotEditInfor()
{
    QString name = sender()->objectName();
    if(name == QString("databaseLineEdit")){
        resetEditItemValue(sender(),Infor::Database);
    }else if(name == QString("sqlDriverComboBox")){
       resetComboBoxItemValue(sender(),Infor::Driver);
    }else if(name == QString("IPLineEdit")){
        resetEditItemValue(sender(),Infor::IP);
    }else if(name == QString("portLineEdit")){
        resetEditItemValue(sender(),Infor::Port);
    }else if(name == QString("userNameLineEdit")){
        resetEditItemValue(sender(),Infor::UserName);
    }else if(name == QString("passwordLineEdit")){
        resetEditItemValue(sender(),Infor::Password);
    }
}

void CDataSourceDialog::createConnections(bool isReport)
{
    connect(ui->dataSourceComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotDataSourceChanged(QString)));

    if(!isReport) // when the dialog is called for the curve, the following slots is no use.
        return;
    connect(ui->databaseLineEdit,SIGNAL(editingFinished()),this,SLOT(slotEditInfor()));
    connect(ui->sqlDriverComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(slotEditInfor()));
    connect(ui->userNameLineEdit,SIGNAL(editingFinished()),this,SLOT(slotEditInfor()));
    connect(ui->passwordLineEdit,SIGNAL(editingFinished()),this,SLOT(slotEditInfor()));
    connect(ui->portLineEdit,SIGNAL(editingFinished()),this,SLOT(slotEditInfor()));
    connect(ui->IPLineEdit,SIGNAL(editingFinished()),this,SLOT(slotEditInfor()));
}

void CDataSourceDialog::addSqlDriverItems()
{
    ui->sqlDriverComboBox->clear();
    m_drivers.clear();

    m_drivers["mysql"] = tr("QMYSQL");
    m_drivers["kingbase"] = tr("QKINGBASE");

    QMapIterator<QString,QString> iter(m_drivers);
    while(iter.hasNext()){
        ui->sqlDriverComboBox->addItem(iter.next().key());
    }
}

void CDataSourceDialog::setDatabaseUI(bool enable)
{
    ui->databaseLineEdit->setEnabled(enable);
    ui->sqlDriverComboBox->setEnabled(enable);
    ui->IPLineEdit->setEnabled(enable);
    ui->portLineEdit->setEnabled(enable);
    ui->passwordLineEdit->setEnabled(enable);
    ui->userNameLineEdit->setEnabled(enable);
}

void CDataSourceDialog::setReportUI(bool isReport)
{
    ui->dataSourceLabel->setVisible(isReport);
    ui->dataSourceComboBox->setVisible(isReport);
    ui->enableCheckBox->setVisible(!isReport);

    createConnections(isReport);
}
void CDataSourceDialog::resetComboBoxItemValue( QObject *sender, const CDataSourceDialog::Infor &index)
{
    QComboBox *box = qobject_cast<QComboBox*> (sender);
    if(box){
        resetValue(box->currentText(),index);
    }
}

void CDataSourceDialog::resetEditItemValue( QObject *sender,const CDataSourceDialog::Infor &index)
{
    QLineEdit *lineEdit = qobject_cast<QLineEdit*>(sender);
    if(lineEdit){
        resetValue(lineEdit->text(),index);
    }
}

void CDataSourceDialog::resetValue(const QString &text, const CDataSourceDialog::Infor &index)
{
    QString sourceName = ui->dataSourceComboBox->currentText();
    QStringList infor = m_dataSourceInfor.value(sourceName);

    if(infor.isEmpty())
        return;

    infor.replace((int)index,text);
    m_dataSourceInfor.insert(sourceName,infor);
}

QString CDataSourceDialog::driverTrans(const QString &driverName) const
{
    if(m_drivers.contains(driverName))
        return m_drivers.value(driverName);
    return QString();
}
QMap<QString, QStringList> CDataSourceDialog::dataSourceInfor()
{
    return m_dataSourceInfor;
}

void CDataSourceDialog::setDataSourceInfor(const QMap<QString, QStringList> &dataSourceInfor)
{
    m_dataSourceInfor = dataSourceInfor;
    if(m_dataSourceInfor.isEmpty())
        return;
    ui->dataSourceComboBox->clear();
    QMapIterator<QString,QStringList> iter(m_dataSourceInfor) ;
    while(iter.hasNext()){
        iter.next();
        ui->dataSourceComboBox->addItem(iter.key());
    }
    slotDataSourceChanged(m_dataSourceInfor.firstKey());//
}

