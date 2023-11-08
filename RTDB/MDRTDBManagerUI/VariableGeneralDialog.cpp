#include "VariableGeneralDialog.h"
#include "ui_VariableGeneralDialog.h"
#include "SelectVariableDialog.h"
//#include "MDSelectVariableWidget.h"
#include "MDMultiLanguage.h"
#include <QMessageBox>

CVariableGeneralDialog::CVariableGeneralDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariableGeneralDialog)
{
    ui->setupUi(this);
    m_isRunning = false;
    m_precision =3;
    m_name="";
    m_device="";
    m_address ="";
	m_unit = "";
    m_description="";
    m_ext = "";
    m_deviceCnt = 0;
    //m_setAddress = false;
    m_dataType=MDDataType::Int16;
    m_originalDataType=MDDataType::Int16;
    m_variableType=MDVariableType::IOVariable;
    ui->addressTableWidget->setColumnCount(1);
    ui->addressTableWidget->setColumnWidth(0,ui->addressTableWidget->width()*2);
    QStringList headers;
    headers << tr("设备可选地址");
    ui->addressTableWidget->setHorizontalHeaderLabels(headers);
    ui->addressTableWidget->setVisible(false);
    ui->addressTableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);

    QRegExp regExp("^[_a-zA-Z]+[a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp,this));

    connect(ui->nameEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onNameEditTextChanged(const QString&)));
    connect(ui->dataTypeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onDataTypeBoxCurrentIndexChanged(int)));
    connect(ui->originalDataTypeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onOriginalDataTypeBoxCurrentIndexChanged(int)));
    connect(ui->variableTypeBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onVariableTypeBoxCurrentIndexChanged(int)));
    connect(ui->deviceBox,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(onDeviceBoxCurrentIndexChanged(const QString &)));
	connect(ui->unitEdit, SIGNAL(textChanged()), this, SLOT(onUnitEditTextChanged()));
	connect(ui->descriptionEdit, SIGNAL(textChanged()), this, SLOT(onDescriptionEditTextChanged()));
    connect(ui->extEdit, SIGNAL(textChanged()), this, SLOT(onExtEditTextChanged()));
    connect(ui->precisionEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onPrecisionEditTextChanged(const QString&)));
    connect(ui->addressEdit,SIGNAL(textChanged(const QString&)),this,SLOT(onAddressEditTextChanged(const QString&)));
    connect(ui->addressButton,SIGNAL(clicked()),this,SLOT(onAddressButtonClicked()));
    connect(ui->addressTableWidget,SIGNAL(cellDoubleClicked(int, int)),this,SLOT(onAddressTableWidgetCellDoubleClicked(int, int)));
    connect(ui->addressTableWidget,SIGNAL(cellClicked(int, int)),this,SLOT(onAddressTableWidgetCellClicked(int, int)));
    connect(ui->addressEdit,SIGNAL(editingFinished()),this,SLOT(onAddressEditFinished()));
    ui->addressTableWidget->installEventFilter(this);
    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
    //m_dlg = NULL;

	m_curVariableGroup = NULL;
}

CVariableGeneralDialog::~CVariableGeneralDialog()
{
    delete ui;
}

void CVariableGeneralDialog::setRunModel(bool isRunning)
{
    m_isRunning = isRunning;
}
void CVariableGeneralDialog::setRTDBManager(IMDRTDBManager  *rtdbManager)
{
    m_rtdbManager = rtdbManager;
    if(m_rtdbManager!=NULL)
    {
        m_driverManager = m_rtdbManager->driverManager();
    }
}
//!名字
void CVariableGeneralDialog::onNameEditTextChanged(const QString &arg1)
{
    m_name = arg1;
}
//!精度
void CVariableGeneralDialog::onPrecisionEditTextChanged(const QString &arg1)
{
    m_precision = arg1.toInt();
}

//!数据类型
void CVariableGeneralDialog::onDataTypeBoxCurrentIndexChanged(int index)
{
    if(m_dataType !=(MDDataType) index)
    {
        m_dataType =(MDDataType) index;
        m_originalDataType = m_dataType;
        updateContols();
    }
}
//!原始数据类型
void CVariableGeneralDialog::onOriginalDataTypeBoxCurrentIndexChanged(int index)
{
    m_originalDataType = (MDDataType)index;
}
//!变量类型
void CVariableGeneralDialog::onVariableTypeBoxCurrentIndexChanged(int index)
{
    m_variableType =(MDVariableType) index;
    if(m_variableType == MDVariableType::IOVariable &&m_deviceCnt > 0)
    {
        if(!m_device.isEmpty())
        {
            ui->deviceBox->setCurrentText(m_device);
        }
        else
        {
            ui->deviceBox->setCurrentIndex(0);
            m_device = ui->deviceBox->currentText();
        }
    }
    else
    {
        ui->deviceBox->setCurrentIndex(-1);
        m_device = "";
    }

    updateContols();
}
//!设备
void CVariableGeneralDialog::onDeviceBoxCurrentIndexChanged(const QString &arg1)
{
    m_device = arg1;
}
//!单位
void CVariableGeneralDialog::onUnitEditTextChanged()
{
	m_unit = ui->unitEdit->toPlainText();
}
//!变量描述
void CVariableGeneralDialog::onDescriptionEditTextChanged()
{
    m_description = ui->descriptionEdit->toPlainText();
}
//!扩展
void CVariableGeneralDialog::onExtEditTextChanged()
{
    m_ext = ui->extEdit->toPlainText();
}
void CVariableGeneralDialog::initial(IMDRTDBVariable *curRTDBVariable)
{
    if(m_driverManager != NULL)
    {
        m_deviceCnt = m_driverManager->getDeviceCount();
        for(int i=0;i<m_deviceCnt;i++)
        {
              IDevice* device = m_driverManager->getDeviceAt(i) ;
              ui->deviceBox->addItem(device->name());
        }
        //默认device的当前值为空
        ui->deviceBox->setCurrentIndex(-1);
        m_device = "";
    }

    if(curRTDBVariable!= NULL)
    {
        m_name = curRTDBVariable->localName() ;
        m_device = curRTDBVariable->deviceName() ;
        m_address = curRTDBVariable->address() ;
		m_unit = curRTDBVariable->getUnit();
		m_description = curRTDBVariable->getDescription();
        m_ext = curRTDBVariable->getExt();
        m_precision = curRTDBVariable->precision();
        m_dataType = curRTDBVariable->dataType();
        m_originalDataType = curRTDBVariable->originalDataType();
        m_variableType = curRTDBVariable->variableType();
        /*
        if(!m_address.isEmpty())
        {
            m_setAddress = true;
        }*/
        if(m_variableType == MDVariableType::IOVariable &&m_deviceCnt > 0)
        {
            /*
            ui->deviceBox->setCurrentIndex(0);
            m_device = ui->deviceBox->currentText();
            */ //Y2hw
        }
        else
        {
            ui->deviceBox->setCurrentIndex(-1);
            m_device = "";
        }
    }
    updateContols();
}
//!变量地址选择
void CVariableGeneralDialog::onAddressButtonClicked()
{
    if(m_variableType==MDVariableType::IOVariable)
    {
         ui->addressTableWidget->setRowCount(0);
         ui->addressTableWidget->setVisible(true);
         ui->addressTableWidget->setFocus();
        if(m_driverManager == NULL||m_device.isEmpty())
        {
            return;
        }
        IDevice* device = m_driverManager->getDevice(m_device) ;
        if(device == NULL)
        {
            return;
        }
        //获取数据块地址列表
        QStringList varAddressList;
        device->getVariableAddressList(varAddressList);
        ui->addressTableWidget->setRowCount(varAddressList.size());
        for(int i=0;i<varAddressList.size();i++)
        {
            ui->addressTableWidget->setItem(i, 0, new QTableWidgetItem(varAddressList[i]));
        }
    }
    else if(m_variableType==MDVariableType::MiddleVariable)
    {
          ui->addressTableWidget->setVisible(false);

          QList<IMDVariable*>varList;
          //选择变量          
          CSelectVariableDialog dlg;
          dlg.setRTDBManager(m_rtdbManager);
          dlg.exec();
          if(QDialog::Accepted == dlg.result())
          {
              dlg.getSelectedVariables(varList);
              for(int i=0;i<varList.count();i++)
              {
                  IMDVariable *variable = (IMDVariable *)varList[i];
                  if(variable==NULL)
                   {
                      continue;
                   }
                   //拼接表达式
                   QString express = ui->addressEdit->text();
                   express.trimmed();
                   if(!express.isEmpty())
                   {
                       //默认+法
                       express=express+"+"+variable->name();
                   }
                   else
                   {
                       express = variable->name();
                   }
                   ui->addressEdit->setText(express);
                   m_address = express;
                   break;
              }
          }
          //1234
          /*
          if(m_dlg == NULL)
            m_dlg = new CMDSelectVariableWidget();
          m_dlg->setRTDBManager(m_rtdbManager);
          m_dlg->show();*/
    }
    else
    {
          ui->addressTableWidget->setVisible(false);
    }
}

bool CVariableGeneralDialog::eventFilter(QObject *watched, QEvent *event)
{
     //!变量地址选择框
     if (watched==ui->addressTableWidget)
     {
          if (event->type()==QEvent::FocusOut)
          {
             ui->addressTableWidget->setVisible(false);
          }
     }
     return QDialog::eventFilter(watched,event);
}
//!单击变量地址
void CVariableGeneralDialog::onAddressTableWidgetCellClicked(int row, int column)
{
    QString address = ui->addressTableWidget->item(row,column)->text();
    //******************************* Y2hw ****************************
    /*
    if(!address.isEmpty()&&!m_setAddress)
    {
         int pos = address.indexOf("-");
         if(pos>0)
         {
             address = address.left(pos);
         }
         ui->addressEdit->setText(address);
    }
    */
    QString addressedit = ui->addressEdit->text();
    addressedit.trimmed();
    if(!address.isEmpty()&& addressedit.isEmpty())
    {
         int pos = address.indexOf("-");
         if(pos>0)
         {
             address = address.left(pos);
         }
         ui->addressEdit->setText(address);
         m_address = address;
    }
    //-----------------------------------------------------------------

}
//!双击变量地址，并设置
void CVariableGeneralDialog::onAddressTableWidgetCellDoubleClicked(int row, int column)
{
    QString address = ui->addressTableWidget->item(row,column)->text();
    if(!address.isEmpty())
    {
        int pos = address.indexOf("-");
        if(pos>0)
        {
            address = address.left(pos);
        }
       ui->addressEdit->setText(address);
       m_address = address;
       //m_setAddress = true;
    }
    ui->addressTableWidget->setVisible(false);
}
//!修改变量地址
void CVariableGeneralDialog::onAddressEditTextChanged(const QString &arg1)
{
    m_address = arg1;
    m_address.trimmed();/*
    if(m_address.isEmpty())
    {
        m_setAddress = false;
    }*/
}
bool CVariableGeneralDialog::checkVarAddress()
{
    bool bRet = false;
    if(m_driverManager != NULL && m_rtdbManager != NULL && !m_device.isEmpty())
    {
        IDevice* device = m_driverManager->getDevice(m_device);
        if(device != NULL)
        {
            QString address = ui->addressEdit->text();
            IMDRTDBVariable* vartemp = m_rtdbManager->variableManager()->createVariable(m_curVariableGroup);
            if(vartemp != NULL)
            {
                vartemp->setAddress(address);
                vartemp->setOriginalDataType(m_dataType);
                QString error;
                if(!(device->checkVarAddress(vartemp,error)))
                {
                    QMessageBox box;
                    box.setText(error);
                    box.setWindowTitle(tr("错误"));
                    box.setStandardButtons(QMessageBox::Ok);

                    CMDMultiLanguage::instance()->translateWidget(&box);
                    box.exec();
                    ui->addressEdit->setText(m_address);
                }
                else
                {
                    m_address = address;
                    bRet = true;
                }
				vartemp->removeFromUa();
                delete vartemp;
            }
        }//end device != NULL
    }
    return bRet;
}

//!验证变量地址
void CVariableGeneralDialog::onAddressEditFinished()
{
    //当前获取焦点的控件是浏览地址按钮时：
    QWidget* pFocusWidget = focusWidget();
    if(pFocusWidget->objectName() != "addressButton")
    {
        checkVarAddress();
    }
}

void CVariableGeneralDialog::updateContols()
{
    switch (m_variableType)
    {
    case MDVariableType::IOVariable:
        ui->deviceBox->setEnabled(true);
        ui->addressEdit->setEnabled(true);
        ui->addressButton->setEnabled(true);
        ui->addressTableWidget->setEnabled(true);
        ui->originalDataTypeBox->setEnabled(true);        
        break;
    case MDVariableType::MiddleVariable:
        ui->deviceBox->setEnabled(false);
        ui->addressEdit->setEnabled(true);
        ui->addressButton->setEnabled(true);
        ui->addressTableWidget->setEnabled(false);
        ui->originalDataTypeBox->setEnabled(false);
        break;
    case MDVariableType::VirtualVariable:
    default:
        ui->deviceBox->setEnabled(false);
        ui->addressEdit->setEnabled(false);
        ui->addressButton->setEnabled(false);
        ui->addressTableWidget->setEnabled(false);
        ui->originalDataTypeBox->setEnabled(false);
        break;
    }
    //单精或双精时，精度设置有效
    if(m_dataType == MDDataType::Float||m_dataType == MDDataType::Double)
    {
        ui->precisionEdit->setEnabled(true);
    }
    else
    {
        ui->precisionEdit->setEnabled(false);
    }
    ui->nameEdit->setText(m_name);
    ui->dataTypeBox->setCurrentIndex((int) m_dataType);
	ui->unitEdit->setPlainText(m_unit);
	ui->descriptionEdit->setPlainText(m_description);
    ui->extEdit->setPlainText(m_ext);
    ui->precisionEdit->setText(QString("%1").arg(m_precision));
    ui->addressEdit->setText(m_address);
    ui->variableTypeBox->setCurrentIndex((int)m_variableType);
    ui->originalDataTypeBox->setCurrentIndex((int)m_originalDataType);
    ui->deviceBox->setCurrentText(m_device);
   /*/
    if(m_isRunning)
    {
        ui->nameEdit->setEnabled(false);
        ui->dataTypeBox->setEnabled(false);
        ui->descriptionEdit->setEnabled(false);
        ui->precisionEdit->setEnabled(false);
        ui->deviceBox->setEnabled(false);
        ui->addressEdit->setEnabled(false);
        ui->variableTypeBox->setEnabled(false);
        ui->originalDataTypeBox->setEnabled(false);
        ui->descriptionEdit->setEnabled(false);
        ui->addressButton->setEnabled(false);
    }/*/
}

void CVariableGeneralDialog::setCurrentVariableGroup(IMDVariableGroup *varGroup)
{
	m_curVariableGroup = varGroup;
}
