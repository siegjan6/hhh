/*!

\author emhhbmdsZQ==
\class CMDBatchAddWidget
\section 功能
    批量增加设置界面
*/
#ifndef MDBATCHADDWIDGET_H
#define MDBATCHADDWIDGET_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class CMDBatchAddWidget;
}

class CMDBatchAddWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CMDBatchAddWidget(QWidget *parent = 0);
    ~CMDBatchAddWidget();

private:
    Ui::CMDBatchAddWidget *ui;

public:
    void setAddJson(const QJsonObject &json);
    void initUI();

public:
    void setErrorInfor(const QString &err);
    QString errorInfor() const;

private slots:
    void closeWindow();
    void setProgressBar(QString str);
    void operatorAdd();

signals:
    void sendNewJson(const QJsonObject &json);

private:
    QJsonObject  m_json;
    QString      m_errorInfor;
    QStringList  m_propIDList;

};

#endif // MDBATCHADDWIDGET_H
