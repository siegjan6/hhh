#include "VariablePropertyDialog.h"
#include "MDMultiLanguage.h"
CVariablePropertyDialog::CVariablePropertyDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("变量属性"));

    m_isRunning = false;
    m_listWidget =new QListWidget(this);
    m_listWidget->insertItem(0,tr("通用"));
    m_listWidget->insertItem(1,tr("初始化"));
    m_listWidget->insertItem(2,tr("工程转换"));
    m_listWidget->insertItem(3,tr("写保护"));
    m_listWidget->insertItem(4,tr("数据归档"));
    m_listWidget->insertItem(5,tr("限值报警"));
    m_listWidget->insertItem(6,tr("安全策略"));
    //m_listWidget->insertItem(6,tr("其他"));

    m_writeProtectDialog = new CVariableWriteProtectDialog();
    m_variableAlarmDialog = new CVariableAlarmDialog();
    m_otherDialog = new CVariableOtherDialog();
    m_generalDialog = new CVariableGeneralDialog();
    m_dataConversionDialog = new CVariableDataConversionDialog();

    m_intializeDialog = new CVariableInitializeDialog();
    m_variableArchivesDialog= new CVariableArchivesDialog();
    m_variableSecurityPolicyDialog = new  CVariableSecurityPolicyDialog();


    m_stackWidget =new QStackedWidget(this);
    m_stackWidget->setFrameStyle(QFrame::Panel|QFrame::Raised);
    m_stackWidget->addWidget(m_generalDialog);
    m_stackWidget->addWidget(m_intializeDialog);
    m_stackWidget->addWidget(m_dataConversionDialog);
    m_stackWidget->addWidget(m_writeProtectDialog);
    m_stackWidget->addWidget(m_variableArchivesDialog);
    m_stackWidget->addWidget(m_variableAlarmDialog);
    m_stackWidget->addWidget(m_variableSecurityPolicyDialog);
    //m_stackWidget->addWidget(m_otherDialog);

    m_mainLayout =new QHBoxLayout();
    m_mainLayout->setMargin(1);
    m_mainLayout->setSpacing(5);
    m_mainLayout->addWidget(m_listWidget);
    m_mainLayout->addWidget(m_stackWidget);
    m_mainLayout->setStretchFactor(m_listWidget,4);
    m_mainLayout->setStretchFactor(m_stackWidget,9);


    m_okButton=new QPushButton(tr("确定"));
    m_cancleButton=new QPushButton(tr("取消"));

    m_btnLayout =new QHBoxLayout();
    m_btnLayout->addStretch(1);
    m_btnLayout->addWidget(m_cancleButton);
    m_btnLayout->addWidget(m_okButton);


    m_rightLayout =new QVBoxLayout(this);
    m_rightLayout->setMargin(5);
    m_rightLayout->setSpacing(5);
    m_rightLayout->addLayout(m_mainLayout);
    m_rightLayout->addLayout(m_btnLayout);

    connect(m_okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(m_cancleButton, SIGNAL(clicked()), this, SLOT(onCancelButtonClicked()));
    connect(m_listWidget,SIGNAL(currentRowChanged(int)),m_stackWidget,SLOT(setCurrentIndex(int)));

    m_mdRTDBManager = NULL;
    m_curVariableGroup = NULL;
    m_curRTDBVariable = NULL;
    int width = 400+200;
    int height = 415+160;
    this->setFixedSize(width,height); //设置窗体固定大小

    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CVariablePropertyDialog::~CVariablePropertyDialog()
{
    if(m_listWidget != NULL)
    {
        delete m_listWidget;
        m_listWidget = NULL;
    }
    if(m_writeProtectDialog != NULL)
    {
        delete m_writeProtectDialog;
        m_writeProtectDialog = NULL;
    }
    if(m_variableAlarmDialog != NULL)
    {
        delete m_variableAlarmDialog;
        m_variableAlarmDialog = NULL;
    }
    if(m_otherDialog != NULL)
    {
        delete m_otherDialog;
        m_otherDialog = NULL;
    }

    if(m_generalDialog != NULL)
    {
        delete m_generalDialog;
        m_generalDialog = NULL;
    }

    if(m_dataConversionDialog != NULL)
    {
        delete m_dataConversionDialog;
        m_dataConversionDialog = NULL;
    }

    if(m_intializeDialog != NULL)
    {
        delete m_intializeDialog;
        m_intializeDialog = NULL;
    }
    if(m_variableArchivesDialog != NULL)
    {
        delete m_variableArchivesDialog;
        m_variableArchivesDialog = NULL;
    }
    if(m_variableSecurityPolicyDialog != NULL)
    {
        delete m_variableSecurityPolicyDialog;
        m_variableSecurityPolicyDialog = NULL;
    }

    if(m_stackWidget != NULL)
    {
        delete m_stackWidget;
        m_stackWidget = NULL;
    }
    if(m_okButton != NULL)
    {
        delete m_okButton;
        m_okButton = NULL;
    }
    if(m_cancleButton != NULL)
    {
        delete m_cancleButton;
        m_cancleButton = NULL;
    }
    if(m_mainLayout != NULL)
    {
        delete m_mainLayout;
        m_mainLayout = NULL;
    }
    if(m_btnLayout != NULL)
    {
        delete m_btnLayout;
        m_btnLayout = NULL;
    }

    if(m_rightLayout != NULL)
    {
        delete m_rightLayout;
        m_rightLayout = NULL;
    }
}

void CVariablePropertyDialog::setRunModel(bool isRunning)
{
    m_isRunning = isRunning;
    m_okButton->setEnabled(!m_isRunning);
}
/*!
\section 设置父节点
*/
void CVariablePropertyDialog::setProjectConfiger(CMDProjectConfiger *projectConfiger)
{
    m_projectConfiger = projectConfiger;
    if(m_projectConfiger != NULL)
    {
        m_mdRTDBManager= m_projectConfiger->nsRTDBManager();
    }
}


void CVariablePropertyDialog::setCurrentVariableGroup(IMDVariableGroup *varGroup)
{
    m_curVariableGroup =  varGroup;
}
void CVariablePropertyDialog::setCurrentVariable(IMDRTDBVariable *variable)
{
    m_curRTDBVariable =  variable;

}
//!刷新相应变量的属性
void CVariablePropertyDialog::initial()
{
    //!通用属性
    m_generalDialog->setRTDBManager(m_mdRTDBManager);
    m_generalDialog->setRunModel(m_isRunning);
	m_generalDialog->setCurrentVariableGroup(m_curVariableGroup);
    m_generalDialog->initial(m_curRTDBVariable);

    //!初始化
    m_intializeDialog->setRunModel(m_isRunning);
    m_intializeDialog->initial(m_curRTDBVariable);

    //!归档
    m_variableArchivesDialog->setRunModel(m_isRunning);
    m_variableArchivesDialog->initial(m_curRTDBVariable);

    //!写保护
    m_writeProtectDialog->setRunModel(m_isRunning);
    m_writeProtectDialog->initial(m_curRTDBVariable);

    //!限值报警
    m_variableAlarmDialog->setRTDBManager(m_mdRTDBManager);
    m_variableAlarmDialog->setRunModel(m_isRunning);
    m_variableAlarmDialog->initial(m_curRTDBVariable);

    //!工程转换
    m_dataConversionDialog->setRunModel(m_isRunning);
    m_dataConversionDialog->initial(m_curRTDBVariable);

    //!权限管理
    if(m_projectConfiger != NULL)
    {
        m_variableSecurityPolicyDialog->setRunModel(m_isRunning);
         m_variableSecurityPolicyDialog->initial(m_projectConfiger->powerGroupConfiger(),m_curRTDBVariable);
    }
    else
    {
        m_variableSecurityPolicyDialog->setRunModel(m_isRunning);
        m_variableSecurityPolicyDialog->initial(NULL,m_curRTDBVariable);
    }    
}
//!点击OK检查变量属性有效性
void CVariablePropertyDialog::onOkButtonClicked()
{
    if(m_isRunning)
    {
        this->setResult(QDialog::Accepted);
        this->accept();
        return;
    }
    m_generalDialog->m_name.trimmed();
    if(m_generalDialog->m_name=="")
    {
       //****************************** Y2hw *****************************
       /*
       QMessageBox::warning(NULL, "Error", "变量名称不能为空！", QMessageBox::Ok);
       */
       QMessageBox box;
       box.setText(tr("变量名称不能为空！"));
       box.setWindowTitle(tr("Error"));
       box.setStandardButtons(QMessageBox::Ok);

       CMDMultiLanguage::instance()->translateWidget(&box);
       box.exec();
       //----------------------------------------------------------------
       return;
    }
    //变量组为空，just go
    if(NULL==m_curVariableGroup)
    {
        return;
    }
    //新增变量
    if(NULL==m_curRTDBVariable)
    {
        //如果IO变量，验证地址
        if(m_generalDialog->m_variableType == MDVariableType::IOVariable)
        {
			if (!m_generalDialog->checkVarAddress())
				return;
        }
        IMDRTDBVariable* rtdbVariable = m_mdRTDBManager->variableManager()->createVariable(m_curVariableGroup);
        rtdbVariable->setLocalName(m_generalDialog->m_name);
        rtdbVariable->setDataType((MDDataType)m_generalDialog->m_dataType);
		rtdbVariable->setUnit(m_generalDialog->m_unit);
		rtdbVariable->setDescription(m_generalDialog->m_description);
        rtdbVariable->setExt(m_generalDialog->m_ext);
        rtdbVariable->setPrecision(m_generalDialog->m_precision);
        rtdbVariable->setDeviceName(m_generalDialog->m_device);
        rtdbVariable->setAddress(m_generalDialog->m_address);
        rtdbVariable->setVariableType((MDVariableType)m_generalDialog->m_variableType);
        rtdbVariable->setOriginalDataType((MDDataType)m_generalDialog->m_originalDataType);
        rtdbVariable->setDataSaveRulerParameter(m_variableArchivesDialog->m_dataSaveRulerParameter);
        rtdbVariable->setWriteProtectParameter(m_writeProtectDialog->m_writeProtectParameter);
        rtdbVariable->setVariableAlarmParameter(m_variableAlarmDialog->m_variableAlarmParameter);
        rtdbVariable->setDataConversionParameter(m_dataConversionDialog->m_dataConversionParameter);
        rtdbVariable->setActionSPList(&m_variableSecurityPolicyDialog->m_actionSPList);
        rtdbVariable->setInitializeParameter(m_intializeDialog->m_initialParameter);
        QString error;
        if(m_curVariableGroup->addVariable(rtdbVariable,error))
        {
            /*
            if(rtdbVariable->variableType()==MDVariableType::MiddleVariable)
            {
                m_mdRTDBManager->middleVariableManager()->addVariable(rtdbVariable,error);
            }*/
            this->setResult(QDialog::Accepted);
            this->accept();
        }
        else
        {
            //****************************** Y2hw *****************************
            /*
            QMessageBox::warning(NULL, "Error", error, QMessageBox::Ok);
            */
            QMessageBox box;
            box.setText(error);
            box.setWindowTitle(tr("Error"));
            box.setStandardButtons(QMessageBox::Ok);

            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            delete rtdbVariable;
            //----------------------------------------------------------------
        }
    }
    else//查看或修改属性
    {
        bool bChangeName = false;
        if(0!=QString::compare(m_curRTDBVariable->localName(),m_generalDialog->m_name, Qt::CaseInsensitive))
        {
            if(NULL!=m_curVariableGroup->getVariable(m_generalDialog->m_name))
            {
                //****************************** Y2hw *****************************
                /*
                QMessageBox::warning(NULL, "Error", "变量已经存在！", QMessageBox::Ok);
                */
                QMessageBox box;
                box.setText(tr("变量已经存在！"));
                box.setWindowTitle(tr("Error"));
                box.setStandardButtons(QMessageBox::Ok);

                CMDMultiLanguage::instance()->translateWidget(&box);
                box.exec();
                //----------------------------------------------------------------
                return;
            }
            bChangeName = true;
            m_curVariableGroup->removeVariable(m_curRTDBVariable);
            //m_mdRTDBManager->variableManager()->removeVariable(m_curRTDBVariable);
        }
        //checkAddress
        QString error;
        bool bAddressChange = false;
        bool bRemoveFromDB = false;

        if(m_generalDialog->m_variableType == MDVariableType::IOVariable)
        {
            if(!m_generalDialog->checkVarAddress())
                return;
        }

        //检查IO变量数据块是否变化或变量类型从IO变量变为非IO
        if(m_curRTDBVariable->variableType() == MDVariableType::IOVariable &&
                (MDVariableType)m_generalDialog->m_variableType != MDVariableType::IOVariable)
        {
            if(!bChangeName)
                bRemoveFromDB = true;
        }
        if(bRemoveFromDB == false)
        {
            QString str1 = "",str2 = "";
            int nindex = m_curRTDBVariable->address().indexOf('.');
            if(nindex != -1)
                str1 = m_curRTDBVariable->address().left(nindex);

            nindex = m_generalDialog->m_address.indexOf('.');
            if(nindex != -1)
                str2 = m_generalDialog->m_address.left(nindex);

            //设备名或数据块名变化
            if(str1.compare(str2,Qt::CaseInsensitive) != 0 ||
                    m_generalDialog->m_device.compare(m_curRTDBVariable->deviceName()) != 0)
            {
                bAddressChange = true;
                if(m_curRTDBVariable->variableType() == MDVariableType::IOVariable)
                {
                    if(!bChangeName)
                        bRemoveFromDB = true;
                }
            }
        }
        //!先从DB移除
        if(bRemoveFromDB)
        {
            IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(m_curRTDBVariable->deviceName());
            if(pdev != NULL)
            {
                pdev->removeVariable(m_curRTDBVariable);
            }
        }
        //m_mdRTDBManager->middleVariableManager()->removeVariable(m_curRTDBVariable);
        m_curRTDBVariable->setInitializeParameter(m_intializeDialog->m_initialParameter);
        m_curRTDBVariable->setLocalName(m_generalDialog->m_name);
        m_curRTDBVariable->setDataType((MDDataType)m_generalDialog->m_dataType);
		m_curRTDBVariable->setUnit(m_generalDialog->m_unit);
		m_curRTDBVariable->setDescription(m_generalDialog->m_description);
        m_curRTDBVariable->setExt(m_generalDialog->m_ext);
        m_curRTDBVariable->setPrecision(m_generalDialog->m_precision);
        m_curRTDBVariable->setDeviceName(m_generalDialog->m_device);
        m_curRTDBVariable->setAddress(m_generalDialog->m_address);
        m_curRTDBVariable->setVariableType((MDVariableType)m_generalDialog->m_variableType);
        m_curRTDBVariable->setOriginalDataType((MDDataType)m_generalDialog->m_originalDataType);
        m_curRTDBVariable->setDataSaveRulerParameter(m_variableArchivesDialog->m_dataSaveRulerParameter);
        m_curRTDBVariable->setWriteProtectParameter(m_writeProtectDialog->m_writeProtectParameter);
        m_curRTDBVariable->setVariableAlarmParameter(m_variableAlarmDialog->m_variableAlarmParameter);

        m_curRTDBVariable->setDataConversionParameter(m_dataConversionDialog->m_dataConversionParameter);
        m_curRTDBVariable->setActionSPList(&m_variableSecurityPolicyDialog->m_actionSPList);

        if(bChangeName)
        {
            m_curVariableGroup->addVariable(m_curRTDBVariable,error);
            //!已经添加到数据块
            bAddressChange = false;
        }
        /*
        if(m_curRTDBVariable->variableType() ==MDVariableType::MiddleVariable)
        {
            m_mdRTDBManager->middleVariableManager()->addVariable(m_curRTDBVariable,error);
        }*/

        //!地址变化,添加到数据块
        if(bAddressChange && m_curRTDBVariable->variableType() == MDVariableType::IOVariable)
        {
            IDevice* pdev = m_mdRTDBManager->driverManager()->getDevice(m_curRTDBVariable->deviceName());
            if(pdev != NULL)
            {
                if(pdev->addVariable(m_curRTDBVariable,error))
                    m_curRTDBVariable->setValid(true);
                else
                    m_curRTDBVariable->setValid(false);
            }
        }

        this->setResult(QDialog::Accepted);
        this->accept();
    }
}
//取消，just返回
void CVariablePropertyDialog::onCancelButtonClicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}
