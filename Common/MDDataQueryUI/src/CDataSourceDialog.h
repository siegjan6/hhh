#ifndef CDATASOURCEDIALOG_H
#define CDATASOURCEDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class CDataSourceDialog;
}

class CDataSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDataSourceDialog(QWidget *parent = 0,bool isReport = false);
    ~CDataSourceDialog();

    enum class Infor{
        Driver = 0,
        IP,
        Port,
        Database,
        UserName,
        Password,
        Filter
    };

    void setDataSourceInfor(const QMap<QString, QStringList> &dataSourceInfor);

    void setHostName(const QString &name);
    void setDriver(const QString&);
    void setIP(const QString &IP);
    void setPort(const QString &);
    void setUserName(const QString &user);
    void setPassword(const QString &);

    QMap<QString, QStringList> dataSourceInfor();
    QString hostName() const;
    QString driver() const;
    QString IP() const;
    QString port() const;
    QString userName() const;
    QString password() const;

    bool isDatabaseEnabled() const;
private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();
    void on_testConnectButton_clicked();
    void on_enableCheckBox_clicked();
    void slotDataSourceChanged(const QString &);

    void slotEditInfor();

private:
    void createConnections(bool);
    void addSqlDriverItems();

    void setDatabaseUI(bool);
    void setReportUI(bool);

    void resetComboBoxItemValue( QObject* sender, const Infor &index);
    void resetEditItemValue( QObject* sender, const Infor &index);
    void resetValue(const QString &text, const Infor & index);
    QString driverTrans(const QString &)const;
private:
    Ui::CDataSourceDialog *ui;

    QMap<QString,QStringList> m_dataSourceInfor;
    QMap<QString,QString> m_drivers;
};

#endif // CDATASOURCEDIALOG_H
