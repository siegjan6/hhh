#include "MDDataBlockDialog.h"
#include "ui_MDDataBlockDialog.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include <QMessageBox>

#include "MDMultiLanguage.h"//多语言

#ifdef Q_OS_WIN
    //#pragma execution_character_set("utf-8")   //解决 VS编译器下中文乱码
#endif

CMDDataBlockDialog::CMDDataBlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDataBlockDialog)
{
    ui->setupUi(this);
    setFixedSize(487,469);
    setWindowTitle(tr("数据块参数"));
    m_dataBlock = NULL;
    m_comboRow = -1;
    m_comboCol = -1;
    m_pLineEdit = NULL;

    //!tableWidget初始化设置
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setColumnWidth(0,70);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,198);
    ui->tableWidget->setRowCount(1);

    ui->tableWidget->setRowHeight(0,20);

    for(int col = 0;col<4;col++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;

        ui->tableWidget->setItem(0,col,item);
        item->setBackgroundColor(QColor(255,255,255));
    }

    QStringList header;
    header<<"寄存器"<<"开始地址"<<"结束地址"<<"提示信息";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ui->tableWidget->setStyleSheet("selection-background-color:lucency");
    //ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //!正则表达式--约束界面填入信息
    QRegExp regExp1("^[a-zA-Z]+[a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp1,this));
    QRegExp regExp2("^\\d+$");
    ui->rateEdit->setValidator(new QRegExpValidator(regExp2,this));
    //!多语言
    QStringList types;
    types << "QLineEdit" << "QComboBox";
    CMDMultiLanguage::instance()->translateWidget(this, &types);

    //!信号槽
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(okButton_clicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelButton_clicked()));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(tableWidget_cellDoubleClicked(int, int)));

    //!事件初始化
    ui->tableWidget->installEventFilter(this);
    ui->label_2->installEventFilter(this);
    ui->label_4->installEventFilter(this);
    ui->label_5->installEventFilter(this);
    ui->label->installEventFilter(this);
    //ui->cancelButton->installEventFilter(this);
    ui->descriptionEdit->installEventFilter(this);
    ui->enableCheckBox->installEventFilter(this);
    ui->nameEdit->installEventFilter(this);
    //ui->okButton->installEventFilter(this);
    ui->rateEdit->installEventFilter(this);

}

CMDDataBlockDialog::~CMDDataBlockDialog()
{
    delete ui;
}
void CMDDataBlockDialog::setDataBlock(CMDDataBlock *dataBlock)
{
    m_dataBlock = dataBlock;
    if(m_dataBlock!=NULL)
    {
        ui->nameEdit->setText(m_dataBlock->name());
        ui->descriptionEdit->setText(m_dataBlock->description());
        ui->enableCheckBox->setChecked(m_dataBlock->isEnabled());
        ui->rateEdit->setText(QString("%1").arg(m_dataBlock->pollRate()));

        listNoOrderRegistersConfig listRegistersConfig;
        listRegistersConfig.clear();
        m_dataBlock->listRegistersConfig(listRegistersConfig);

        ui->tableWidget->setRowCount(listRegistersConfig.count()+1);

        for(int row=0;row<listRegistersConfig.count()+1;row++)
        {
            ui->tableWidget->setRowHeight(row,20);
            for(int col = 0;col<4;col++)
            {
                QTableWidgetItem *item = new QTableWidgetItem;

                ui->tableWidget->setItem(row,col,item);
                item->setBackgroundColor(QColor(255,255,255));
            }
            if(row!=listRegistersConfig.count())
            {
                ui->tableWidget->item(row,0)->setText(listRegistersConfig.at(row)->m_strRegister);
                ui->tableWidget->item(row,1)->setText(listRegistersConfig.at(row)->m_strStartAddress);
                ui->tableWidget->item(row,2)->setText(listRegistersConfig.at(row)->m_strEndAddress);

                int nAddressFormat = 0;
                int verifyRegisterFlag = m_dataBlock->verifyRegisterFormat(listRegistersConfig.at(row)->m_strRegister,nAddressFormat);
                if(verifyRegisterFlag != REGISTER_FORMAT_RIGHT)
                {
                    QString strPrompt  = CMDMultiLanguage::instance()->value(tr("寄存器错误"));
                    ui->tableWidget->item(row,3)->setText(strPrompt);
                    //ui->tableWidget->item(row,3)->setText("寄存器错误");
                    ui->tableWidget->item(row,3)->setTextColor(QColor(255,0,0));
                }
                else
                {
                    qint32 startAddress = 0;
                    int verifyStartAddressFlag = m_dataBlock->verifyAddressFormat(listRegistersConfig.at(row)->m_strStartAddress,startAddress,nAddressFormat);

                    qint32 endAddress = 0;
                    int verifyEndAddressFlag = m_dataBlock->verifyAddressFormat(listRegistersConfig.at(row)->m_strEndAddress,endAddress,nAddressFormat);

                    if(verifyStartAddressFlag!=ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
                    {
                        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):开始结束地址错误")).arg(nAddressFormat);
                        ui->tableWidget->item(row,3)->setText(strPrompt);
                        //ui->tableWidget->item(row,3)->setText(QString("地址(%1进制):开始结束地址错误").arg(nAddressFormat));
                        ui->tableWidget->item(row,3)->setTextColor(QColor(255,0,0));

                    }
                    else if(verifyStartAddressFlag!=ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag==ADDRESS_FORMAT_RIGHT)
                    {
                        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):开始地址错误")).arg(nAddressFormat);
                        ui->tableWidget->item(row,3)->setText(strPrompt);
                        //ui->tableWidget->item(row,3)->setText(QString("地址(%1进制):开始地址错误").arg(nAddressFormat));
                        ui->tableWidget->item(row,3)->setTextColor(QColor(255,0,0));

                    }
                    else if(verifyStartAddressFlag==ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
                    {
                        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):结束地址错误")).arg(nAddressFormat);
                        ui->tableWidget->item(row,3)->setText(strPrompt);
                        //ui->tableWidget->item(row,3)->setText(QString("地址(%1进制):结束地址错误").arg(nAddressFormat));
                        ui->tableWidget->item(row,3)->setTextColor(QColor(255,0,0));

                    }
                    else
                    {
                        int verifyLengthFlag = m_dataBlock->verifyLength(startAddress,endAddress);
                        if(verifyLengthFlag!=LENGTH_RIGHT)
                        {
                            QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):长度错误")).arg(nAddressFormat);
                            ui->tableWidget->item(row,3)->setText(strPrompt);
                            //ui->tableWidget->item(row,3)->setText(QString("地址(%1进制):长度错误").arg(nAddressFormat));
                            ui->tableWidget->item(row,3)->setTextColor(QColor(255,0,0));

                        }
                        else
                        {
                            QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制)")).arg(nAddressFormat);
                            ui->tableWidget->item(row,3)->setText(strPrompt);
                            //ui->tableWidget->item(row,3)->setText(QString("地址(%1进制)").arg(nAddressFormat));
                            ui->tableWidget->item(row,3)->setTextColor(QColor(0,0,0));
                        }
                    }
                }
            }

        }
        if(m_dataBlock->verifyFlag() == FLAG_RIGHT)
        {
            refreshBytesCount();
        }
        else
        {
            if(m_dataBlock->verifyFlag() == FLAG_LENGT_OVERRUN_ERROR)
            {
                refreshBytesCount();
            }
            else
            {
                QString strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>寄存器&地址配置表：【当前配置空间：<span style='color:#ff0000;'>配置错误无法计算</span>（最大配置空间8192字节）】</p></body></html>"));
                ui->label->setText(strPrompt);
            }
        }
        CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
        if(device)
        {
            if(device->isRuntimeMode()||device->isRunning())
            {
                ui->okButton->setEnabled(false);
            }
        }
    }
}

void CMDDataBlockDialog::okButton_clicked()
{
    if(m_dataBlock== NULL)
    {
        this->setResult(QDialog::Rejected);
        this->reject ();
        return;
    }
    CMDDevice *device = (CMDDevice*)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device== NULL)
    {
        this->setResult(QDialog::Rejected);
        this->reject ();
        return;
    }
    QString name= ui->nameEdit->text();
    name.trimmed();
    if(name.isEmpty())
    {
       //QMessageBox::warning(NULL, "Error", "数据块名称不能为空！", QMessageBox::Ok);
        QMessageBox box;
        box.setText(tr("数据块名称不能为空！"));
        box.setWindowTitle(tr("Error"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();

       return;
    }

    IDataBlock *dataBlockTemp = device->getDataBlock(name);
    if( dataBlockTemp!=NULL && m_dataBlock!=dataBlockTemp)
    {
       //QMessageBox::warning(NULL, "Error", "数据块名称重复！", QMessageBox::Ok);
        QMessageBox box;
        box.setText(tr("数据块名称重复！"));
        box.setWindowTitle(tr("Error"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
       return;
    }
    bool verifyTableConfigFlag = 0;
    for(int row = 0;row<ui->tableWidget->rowCount()-1;row++)
    {
        int nAddressFormat = 0;
        int verifyRegisterFlag = m_dataBlock->verifyRegisterFormat(ui->tableWidget->item(row,0)->text(),nAddressFormat);
        if(verifyRegisterFlag != REGISTER_FORMAT_RIGHT)
        {
            verifyTableConfigFlag = 1;
            break;
        }
        else
        {
            qint32 startAddress = 0;
            int verifyStartAddressFlag = m_dataBlock->verifyAddressFormat(ui->tableWidget->item(row,1)->text(),startAddress,nAddressFormat);

            qint32 endAddress = 0;
            int verifyEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->tableWidget->item(row,2)->text(),endAddress,nAddressFormat);

            if(verifyStartAddressFlag!=ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
            {
                verifyTableConfigFlag = 1;
                break;

            }
            else if(verifyStartAddressFlag!=ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag==ADDRESS_FORMAT_RIGHT)
            {
                verifyTableConfigFlag = 1;
                break;

            }
            else if(verifyStartAddressFlag==ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
            {
                verifyTableConfigFlag = 1;
                break;

            }
            else
            {
                int verifyLengthFlag = m_dataBlock->verifyLength(startAddress,endAddress);
                if(verifyLengthFlag!=LENGTH_RIGHT)
                {
                    verifyTableConfigFlag = 1;
                    break;

                }
            }
        }

    }
    if(verifyTableConfigFlag==1)
    {
        QMessageBox box;
        box.setText(tr("表格配置信息错误"));
        box.setWindowTitle(tr("错误"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        return;
    }
    bool bBytesOverrunFlag = refreshBytesCount();
    if(bBytesOverrunFlag == 1)
    {
        QMessageBox box;
        box.setText(tr("超出最大配置空间（8192）"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        return;
    }
    m_dataBlock->setVerifyFlag(FLAG_RIGHT);
    QString strRegistersList = "";
    for(int row = 0;row<ui->tableWidget->rowCount()-1;row++)
    {
        if(row==ui->tableWidget->rowCount()-2)
        {
            strRegistersList = strRegistersList +ui->tableWidget->item(row,0)->text()+","+ui->tableWidget->item(row,1)->text()+","+ui->tableWidget->item(row,2)->text();
        }
        else
        {
            strRegistersList = strRegistersList +ui->tableWidget->item(row,0)->text()+","+ui->tableWidget->item(row,1)->text()+","+ui->tableWidget->item(row,2)->text()+";";
        }
    }
    m_dataBlock->setListRegistersConfigDownLoad(strRegistersList);
    listNoOrderRegistersConfig listTempRegistersConfig;
    listTempRegistersConfig.clear();
    m_dataBlock->listRegistersConfig(listTempRegistersConfig);
    m_dataBlock->setMapRegistersConfig(listTempRegistersConfig);
    m_dataBlock->setInitialMapPackage(listTempRegistersConfig);

    m_dataBlock->setName(name);
    m_dataBlock->setDescription(ui->descriptionEdit->text());
    m_dataBlock->setEnabled(ui->enableCheckBox->isChecked());
    m_dataBlock->setPollRate(ui->rateEdit->text().toInt());

    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDDataBlockDialog::cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}
bool CMDDataBlockDialog::eventFilter(QObject *watched,QEvent *event)
{

    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *e = static_cast<QKeyEvent *>(event);
        if((e->key() == Qt::Key_Delete)&&(watched->objectName() == "tableWidget"))
        {
            QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
            if(items.size()==0)
            {
                return true;
            }
            QList<int>delRow;
            int itemsCount = items.size();
            for(int delIndex = 0;delIndex<itemsCount/4;delIndex++)
            {
                delRow.append(items.at(0)->row());
                items.removeAt(0);
                int compareRow = delRow.at(delIndex);
                QList<QTableWidgetItem*>::iterator it = items.begin();
                QList<QTableWidgetItem*>::iterator end = items.end();
                while(it!=end)
                {
                    QTableWidgetItem* obj = *it;
                    if ( obj->row() == compareRow )
                    {
                        it = items.erase(it);
                        end = items.end();
                        //delete obj;
                    }
                    else {
                        ++it;
                    }
                }
            }

            int nTemp = -1;
            int nTemp1 = -1;
            for(int delIndex = 0;delIndex<delRow.size()-1;delIndex++)
            {
                for(int delIndex1 = 0;delIndex1<delRow.size()-1-delIndex;delIndex1++)
                {
                    if(delRow.at(delIndex1)>delRow.at(delIndex1+1))
                    {
                        nTemp = delRow.at(delIndex1);
                        nTemp1 = delRow.at(delIndex1+1);
                        delRow.replace(delIndex1,nTemp1);
                        delRow.replace(delIndex1+1,nTemp);
                    }
                }
            }
            for(int delIndex = 0;delIndex<delRow.size();delIndex++)
            {
               if(delRow.at(delIndex)-delIndex != ui->tableWidget->rowCount()-1)
               {
                   ui->tableWidget->removeRow(delRow.at(delIndex)-delIndex);
               }
            }
            bool bBytesOverrunFlag = refreshBytesCount();
            if(bBytesOverrunFlag == 1)
            {
                QMessageBox box;
                box.setText(tr("超出最大配置空间（8192）"));
                box.setWindowTitle(tr("提示:"));
                box.setStandardButtons(QMessageBox::Ok);
                CMDMultiLanguage::instance()->translateWidget(&box);
                box.exec();
            }
        }
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        if( watched->objectName() == "descriptionEdit"
                    ||watched->objectName() == "enableCheckBox"
                        ||watched->objectName() == "label_2"
                            ||watched->objectName() == "label_4"
                                ||watched->objectName() == "label_5"
                                    ||watched->objectName() == "nameEdit"
                                            ||watched->objectName() == "rateEdit"
                                                ||watched->objectName() == "label")
        {
            ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
        }
    }

    return QWidget::eventFilter(watched,event);
}
void CMDDataBlockDialog::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
}

void CMDDataBlockDialog::tableWidget_cellDoubleClicked(int row, int column)
{
    if(column == 0)
    {
        if(m_comboRow !=row)
            ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);

        m_comboRow = row;
        m_comboCol = column;
        QComboBox *pTempComBox = new  QComboBox;
        pTempComBox->addItem("X");
        pTempComBox->addItem("Y");
        pTempComBox->addItem("S");
        pTempComBox->addItem("M");
        pTempComBox->addItem("SPM");
        pTempComBox->addItem("D");
        pTempComBox->addItem("SPD");
        pTempComBox->addItem("T");
        pTempComBox->addItem("TS");
        pTempComBox->addItem("C16");
        pTempComBox->addItem("C32");
        pTempComBox->addItem("CS16");
        pTempComBox->addItem("CS32");
        if(ui->tableWidget->item(m_comboRow,m_comboCol)->text()=="")
        {
            pTempComBox->setCurrentIndex(-1);
        }
        else
        {
            pTempComBox->setCurrentText(ui->tableWidget->item(m_comboRow,m_comboCol)->text());
        }
        ui->tableWidget->setCellWidget(m_comboRow,m_comboCol,pTempComBox);
        connect(ui->tableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(selectOtherItem()));
        connect(pTempComBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(setColString(QString)));
    }
    else if(column == 1 ||column == 2)
    {
        ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
        m_comboRow = row;
        m_comboCol = column;
        QLineEdit *pTempLineEdit = new QLineEdit;
        pTempLineEdit->setText(ui->tableWidget->item(m_comboRow,m_comboCol)->text());
        ui->tableWidget->setCellWidget(m_comboRow,m_comboCol,pTempLineEdit);
        m_pLineEdit = pTempLineEdit;
        connect(pTempLineEdit,SIGNAL(editingFinished()),this,SLOT(addressEditFinish()));
    }
}
void CMDDataBlockDialog::setColString(QString str)
{
    ui->tableWidget->item(m_comboRow,m_comboCol)->setText(str);
    ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
    int nAddressFormat = 0;
    int verifyRegisterFlag = m_dataBlock->verifyRegisterFormat(ui->tableWidget->item(m_comboRow,m_comboCol)->text(),nAddressFormat);
    if(verifyRegisterFlag != REGISTER_FORMAT_RIGHT)
    {
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("寄存器错误"));
        ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
        //ui->tableWidget->item(m_comboRow,3)->setText("寄存器错误");
        ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
        return;
    }

    if(ui->tableWidget->item(m_comboRow,1)->text()==NULL)
    {
        ui->tableWidget->item(m_comboRow,1)->setText("0");
    }
    if(ui->tableWidget->item(m_comboRow,2)->text()==NULL)
    {
        ui->tableWidget->item(m_comboRow,2)->setText("0");
    }
    qint32 startAddress = 0;
    int verifyStartAddressFlag = m_dataBlock->verifyAddressFormat(ui->tableWidget->item(m_comboRow,1)->text(),startAddress,nAddressFormat);

    qint32 endAddress = 0;
    int verifyEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->tableWidget->item(m_comboRow,2)->text(),endAddress,nAddressFormat);

    if(verifyStartAddressFlag!=ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
    {
        QMessageBox box;
        box.setText(tr("开始结束地址错误"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("开始结束地址(%1进制):地址错误")).arg(nAddressFormat);
        ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
        //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制):地址错误").arg(nAddressFormat));
        ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
        return;
    }
    else if(verifyStartAddressFlag!=ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag==ADDRESS_FORMAT_RIGHT)
    {
        QMessageBox box;
        box.setText(tr("开始地址错误"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):开始地址错误")).arg(nAddressFormat);
        ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
        //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制):开始地址错误").arg(nAddressFormat));
        ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
        return;
    }
    else if(verifyStartAddressFlag==ADDRESS_FORMAT_RIGHT && verifyEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
    {
        QMessageBox box;
        box.setText(tr("结束地址错误"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):结束地址错误")).arg(nAddressFormat);
        ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
        //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制):结束地址错误").arg(nAddressFormat));
        ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
        return;
    }
    int verifyLengthFlag = m_dataBlock->verifyLength(startAddress,endAddress);
    if(verifyLengthFlag!=LENGTH_RIGHT)
    {
        QMessageBox box;
        box.setText(tr("长度错误"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):长度错误)")).arg(nAddressFormat);
        ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
        //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制):长度错误").arg(nAddressFormat));
        ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
        return;
    }
    QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制)")).arg(nAddressFormat);
    ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
    //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制)").arg(nAddressFormat));
    ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(0,0,0));

    int currentAddRow = ui->tableWidget->rowCount();
    if(m_comboRow == currentAddRow-1 )
    {

        ui->tableWidget->insertRow(currentAddRow);
        ui->tableWidget->setRowHeight(currentAddRow,20);

        for(int col = 0;col<4;col++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;

            ui->tableWidget->setItem(currentAddRow,col,item);
            item->setBackgroundColor(QColor(255,255,255));
        }
    }
    bool bBytesOverrunFlag = refreshBytesCount();
    if(bBytesOverrunFlag == 1)
    {
        QMessageBox box;
        box.setText(tr("超出最大配置空间（8192）"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        return;
    }
}
void CMDDataBlockDialog::addressEditFinish()
{
    if(m_pLineEdit==NULL)
    {
        ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
        return;
    }
    if(m_pLineEdit!=NULL)
    {
        if(ui->tableWidget->item(m_comboRow,m_comboCol)->text()==m_pLineEdit->text())
        {
            ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
            m_pLineEdit = NULL;
            return;
        }
    }
    int nAddressFormat = 0;
    int verifyRegisterFlag = m_dataBlock->verifyRegisterFormat(ui->tableWidget->item(m_comboRow,0)->text(),nAddressFormat);
    if(verifyRegisterFlag != REGISTER_FORMAT_RIGHT)
    {
        QMessageBox box;
        box.setText(tr("寄存器错误"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("寄存器错误"));
        //ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
        ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
        ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
        ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
        return;
    }
    qint32 address = 0;
    int verifyAddressFlag = m_dataBlock->verifyAddressFormat(m_pLineEdit->text(),address,nAddressFormat);
    if(verifyAddressFlag!=ADDRESS_FORMAT_RIGHT)
    {
        QMessageBox box;
        box.setText(tr("输入地址格式错误"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
        m_pLineEdit = NULL;
        return;
    }
    else
    {
        ui->tableWidget->item(m_comboRow,m_comboCol)->setText(m_pLineEdit->text());
        ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
        m_pLineEdit = NULL;
    }
    qint32 startOrEndAddress = 0;
    int verifyStartOrEndAddressFlag;
    if(m_comboCol == 1)
    {
        verifyStartOrEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->tableWidget->item(m_comboRow,2)->text(),startOrEndAddress,nAddressFormat);
        if(verifyStartOrEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
        {
            QMessageBox box;
            box.setText(tr("结束地址格式错误"));
            box.setWindowTitle(tr("提示:"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):结束地址错误")).arg(nAddressFormat);
            ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
            //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制):结束地址错误").arg(nAddressFormat));
            ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
            return;
        }
        int verifyLengthFlag = m_dataBlock->verifyLength(address,startOrEndAddress);
        if(verifyLengthFlag!=LENGTH_RIGHT)
        {
            ui->tableWidget->item(m_comboRow,2)->setText(ui->tableWidget->item(m_comboRow,m_comboCol)->text());
        }

    }
    else if(m_comboCol == 2)
    {
        verifyStartOrEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->tableWidget->item(m_comboRow,1)->text(),startOrEndAddress,nAddressFormat);
        if(verifyStartOrEndAddressFlag!=ADDRESS_FORMAT_RIGHT)
        {
            QMessageBox box;
            box.setText(tr("开始地址格式错误"));
            box.setWindowTitle(tr("提示:"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制):开始地址错误")).arg(nAddressFormat);
            ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
            //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制):开始地址错误").arg(nAddressFormat));
            ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(255,0,0));
            return;
        }
        int verifyLengthFlag = m_dataBlock->verifyLength(startOrEndAddress,address);
        if(verifyLengthFlag!=LENGTH_RIGHT)
        {
            ui->tableWidget->item(m_comboRow,1)->setText(ui->tableWidget->item(m_comboRow,m_comboCol)->text());
        }
    }
    QString strPrompt  = CMDMultiLanguage::instance()->value(tr("地址(%1进制)")).arg(nAddressFormat);
    ui->tableWidget->item(m_comboRow,3)->setText(strPrompt);
    //ui->tableWidget->item(m_comboRow,3)->setText(QString("地址(%1进制)").arg(nAddressFormat));
    ui->tableWidget->item(m_comboRow,3)->setTextColor(QColor(0,0,0));
    int currentAddRow = ui->tableWidget->rowCount();
    if(m_comboRow == currentAddRow-1)
    {

        ui->tableWidget->insertRow(currentAddRow);
        ui->tableWidget->setRowHeight(currentAddRow,20);

        for(int col = 0;col<4;col++)
        {
            QTableWidgetItem *item = new QTableWidgetItem;

            ui->tableWidget->setItem(currentAddRow,col,item);
            item->setBackgroundColor(QColor(255,255,255));
        }
    }
    bool bBytesOverrunFlag = refreshBytesCount();
    if(bBytesOverrunFlag == 1)
    {
        QMessageBox box;
        box.setText(tr("超出最大配置空间（8192）"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        return;
    }
}
void CMDDataBlockDialog::selectOtherItem()
{
    ui->tableWidget->removeCellWidget(m_comboRow,m_comboCol);
}
bool CMDDataBlockDialog::refreshBytesCount()
{
    int currentRows = ui->tableWidget->rowCount();
    int nBytes = 0;
    for(int i =0;i<currentRows-1;i++)
    {
        if(ui->tableWidget->item(i,0)->text() == "X"
                || ui->tableWidget->item(i,0)->text() == "Y"
                    || ui->tableWidget->item(i,0)->text() == "S"
                        || ui->tableWidget->item(i,0)->text() == "M"
                            || ui->tableWidget->item(i,0)->text() == "SPM"
                                || ui->tableWidget->item(i,0)->text() == "TS"
                                    || ui->tableWidget->item(i,0)->text() == "CS16"
                                        || ui->tableWidget->item(i,0)->text() == "CS32")
        {
            if(ui->tableWidget->item(i,0)->text() == "X"||ui->tableWidget->item(i,0)->text() == "Y")
            {
                bool ok=0;
                int startAddress = ui->tableWidget->item(i,1)->text().toInt(&ok,8);
                int endAddress = ui->tableWidget->item(i,2)->text().toInt(&ok,8);
                nBytes = nBytes + (endAddress/32 - startAddress/32+1)*4;
            }
            else
            {
                int startAddress = ui->tableWidget->item(i,1)->text().toInt();
                int endAddress = ui->tableWidget->item(i,2)->text().toInt();
                nBytes = nBytes + (endAddress/32 - startAddress/32+1)*4;
            }

        }
        else if(ui->tableWidget->item(i,0)->text() == "T"
                ||ui->tableWidget->item(i,0)->text() == "C32")
        {
            int startAddress = ui->tableWidget->item(i,1)->text().toInt();
            int endAddress = ui->tableWidget->item(i,2)->text().toInt();
            nBytes = nBytes + (endAddress - startAddress+1)*4;
        }
        else
        {
            int startAddress = ui->tableWidget->item(i,1)->text().toInt();
            int endAddress = ui->tableWidget->item(i,2)->text().toInt();
            nBytes = nBytes + (endAddress - startAddress+1)*2;
        }
    }
    if(nBytes>8192)
    {
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>寄存器&地址配置表：【当前配置空间：<span style='color:#ff0000;'>%1</span>（最大配置空间8192字节）】</p></body></html>")).arg(nBytes);
        ui->label->setText(strPrompt);
        return 1;//超过最大配置空间
    }
    else
    {
        QString strPrompt  = CMDMultiLanguage::instance()->value(tr("寄存器&地址配置表：【当前配置空间：%1（最大配置空间8192字节）】")).arg(nBytes);
        ui->label->setText(strPrompt);
        return 0;
    }
}
