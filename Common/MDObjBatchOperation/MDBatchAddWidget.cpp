#include "MDBatchAddWidget.h"
#include "ui_MDBatchAddWidget.h"
#include "MDMultiLanguage.h"
#include <QLineEdit>
#include <QJsonDocument>
#include <QMessageBox>

CMDBatchAddWidget::CMDBatchAddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMDBatchAddWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("批量增加"));
    this->setWindowModality(Qt::ApplicationModal);

    connect(ui->createPushButton,SIGNAL(clicked()),this,SLOT(operatorAdd()));
    connect(ui->closePushButton,SIGNAL(clicked()),this,SLOT(closeWindow()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(setProgressBar(QString)));
    CMDMultiLanguage::instance()->translateWidget(this);
}

CMDBatchAddWidget::~CMDBatchAddWidget()
{
    delete ui;
}

void CMDBatchAddWidget::setAddJson(const QJsonObject &json)
{
    if(json.empty())
    {
        return;
    }
    m_json = json;
    initUI();
}

void CMDBatchAddWidget::initUI()
{
    ui->progressBar->setValue(0);
    ui->stateLabel->setText("");
    ui->lineEdit->setText("10");
    ui->progressLabel->setText("0/10");
    ui->problemLabel->setText("(0)");
    ui->textEdit->clear();
    QJsonObject myJson =m_json;
    QJsonArray array = myJson.value("BatchAdd").toArray();
    int count = array.count();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(count);
    QStringList horizontalHeaderList;
    horizontalHeaderList<<"参考对象"<<"批量格式"<<"起始数字"<<"步长"<<"进制";
    ui->tableWidget->setHorizontalHeaderLabels(horizontalHeaderList);
    for(int i=0; i< count; i++)
    {
        for(int j = 0; j<6;j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;
            ui->tableWidget->setItem(i,j,item);
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackgroundColor(QColor(255,255,255));
        }
    }
    QStringList verticalHeaderList;
    for(int i = 0; i<count; i++)
    {
        QJsonObject myJson = array.at(i).toObject();
        QString propComment = myJson.value("PropComment").toString();
        QString propID = myJson.value("PropID").toString();
        m_propIDList.append(propID);
        verticalHeaderList.append(propComment);
    }
    ui->tableWidget->setVerticalHeaderLabels(verticalHeaderList);
    for(int column =0; column<5;column++)
    {
        for(int obj = 0; obj < count; obj++)
        {
            QJsonObject myJson = array.at(obj).toObject();
            switch(column)
            {
            case 0:
            {
                QString refValue = myJson.value("RefValue").toString();
                ui->tableWidget->item(obj,column)->setText(refValue);
            }
                break;
            case 1:
            {
                QString refValue = myJson.value("FormatValue").toString();
                ui->tableWidget->item(obj,column)->setText(refValue);
            }
                break;
            case 2:
            {
                int start = myJson.value("Start").toInt();
                QString str;
                str.setNum(start);
                ui->tableWidget->item(obj,column)->setText(str);
            }
                break;
            case 3:
            {
                int step = myJson.value("Step").toInt();
                QString str;
                str.setNum(step);
                ui->tableWidget->item(obj,column)->setText(str);
            }
                break;

            case 4:
            {
                int base = myJson.value("Base").toInt();
                QString str;
                str.setNum(base);
                ui->tableWidget->item(obj,column)->setText(str);
            }
                break;
            default:
                break;
            }
        }
    }
}

void CMDBatchAddWidget::setErrorInfor(const QString &err)
{
    m_errorInfor = err;
}

QString CMDBatchAddWidget::errorInfor() const
{
    return m_errorInfor;
}

void CMDBatchAddWidget::setProgressBar(QString str)
{
    int maxValue = str.toInt();
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(maxValue);
    ui->progressLabel->setText("0/"+str);
}

void CMDBatchAddWidget::operatorAdd()
{
    int showMessage(const QString & comment,QWidget *parent = 0,
                QMessageBox::StandardButtons buttons = QMessageBox::Ok);
    QJsonObject newJson;
    int row = ui->tableWidget->rowCount();
    QString textEditArray;
    int errorNum = 0;
    for(int i = 0; i< ui->lineEdit->text().toInt(); i++)
    {
        ui->stateLabel->clear();
        QString stateString;
        QJsonArray array;
        for(int r =0; r<row;r++)
        {
            QJsonObject addJson;
            int c = 1;
            QString idName = m_propIDList.at(r);
            addJson.insert("PropID",idName);
            QString name = ui->tableWidget->item(r,c)->text();
            c++;
            int start = ui->tableWidget->item(r,c)->text().toInt();
            c++;
            int step = ui->tableWidget->item(r,c)->text().toInt();
            c++;
            int base = ui->tableWidget->item(r,c)->text().toInt();
            if(base != 10 && base != 16)
            {
                showMessage("批量增加进制应为十或十六进制！",this);
                return;
            }
            int index = 0;
            index = name.indexOf("(");
            if(index == 0 && name.contains("(*)"))
            {
                showMessage("批量格式的第一个字符不能为空！",this);
                return;
            }
            else if(index != 0 && name.contains("(*)"))
            {
                if(base == 10)
                {
                    QString num;
                    num.setNum(start + i*step);

                    name.remove(index,3);
                    name.insert(index,num);
                }
                if(base == 16)
                {
                    QString suffix;
                    int total = start + i*step;
                    suffix = QString::number(total, 16);
                    name.remove(index,3);
                    name.insert(index,suffix);
                }
            }
            else if(name.isEmpty() || (!name.contains("(")&&!name.contains("*")&&!name.contains(")")))
            {
            }
            else
            {
                showMessage("通配符格式错误！",this);
                return;
            }
            addJson.insert("Value",name);
            array.append(addJson);
            stateString.append(name+"; ");
        }
        newJson.insert("BatchAdd",array);
        ui->stateLabel->setText(stateString);
        m_json = newJson;
        emit sendNewJson(m_json);

        QString addInfor = errorInfor();
        if(!addInfor.isEmpty())
        {
            errorNum++;
        }
        setErrorInfor("");
        if(!addInfor.isEmpty())
        {
            QString indexStr;
            indexStr.setNum(i+1);
            textEditArray.append(indexStr +",");
            textEditArray.append(addInfor +'\n');
            ui->textEdit->setPlainText(textEditArray);
        }

        //! 显示增加进度

        ui->progressBar->setValue(i+1);
        QString strNum;
        strNum.setNum(i+1);
        QString label6;
        label6 = strNum+"/"+ ui->lineEdit->text();
        ui->progressLabel->setText(label6);

        QString errorStr;
        errorStr.setNum(errorNum);
        ui->problemLabel->setText("("+errorStr+")");
    }
}

void CMDBatchAddWidget::closeWindow()
{
    close();
}

int  showMessage(const QString & comment,QWidget *parent = 0,
            QMessageBox::StandardButtons buttons = QMessageBox::Ok)
{
    QMessageBox dialog(parent);
    dialog.setWindowTitle(QObject::tr("警告"));
    dialog.setText(comment);
    dialog.setStandardButtons(buttons);
    return dialog.exec();
}

