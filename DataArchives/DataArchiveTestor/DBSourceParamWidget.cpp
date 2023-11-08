#include "DBSourceParamWidget.h"
//#include "DataArchivesUI.h"
#include "IMDDataSource.h"
#include "IMDDataSourceManager.h"
#include "IMDDatabaseParam.h"
#include <QTextCodec>

#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QCoreApplication>

CDBSourceParamWidget::CDBSourceParamWidget(QWidget *parent)
    : QWidget(parent)
{
#ifdef Q_OS_LINUX
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
#endif
    initData();    
    createUI();
    initLanguage();
}

CDBSourceParamWidget::~CDBSourceParamWidget()
{
    if (m_pH1Layout)
    {
        m_pH1Layout->removeWidget(m_pLbDataSrcName);
        m_pH1Layout->removeWidget(m_pLineDataSrcName);
        m_pH1Layout->removeWidget(m_pCheckEnable);
    }

    if (m_pLbDataSrcName)
    {
        delete m_pLbDataSrcName;
        m_pLbDataSrcName = NULL;
    }

    if (m_pLineDataSrcName)
    {
        delete m_pLineDataSrcName;
        m_pLineDataSrcName = NULL;
    }

    if (m_pCheckEnable)
    {
        delete m_pCheckEnable;
        m_pCheckEnable = NULL;
    }

    if (m_pH2Layout)
    {
        m_pH2Layout->removeWidget(m_pLbDataSrcDecrip);
        m_pH2Layout->removeWidget(m_pLineDataSrcDecrip);
    }

    if (m_pLbDataSrcDecrip)
    {
        delete m_pLbDataSrcDecrip;
        m_pLbDataSrcDecrip = NULL;
    }

    if (m_pLineDataSrcDecrip)
    {
        delete m_pLineDataSrcDecrip;
        m_pLineDataSrcDecrip = NULL;
    }

    if (m_pH3Layout)
    {
        m_pH3Layout->removeWidget(m_pLbDatabaseType);
        m_pH3Layout->removeWidget(m_pComDatabaseType);
        m_pH3Layout->removeWidget(m_pCheckPrefix);
        m_pH3Layout->removeWidget(m_pLinePrefix);
    }

    if (m_pLbDatabaseType)
    {
        delete m_pLbDatabaseType;
        m_pLbDatabaseType = NULL;
    }

    if (m_pComDatabaseType)
    {
        delete m_pComDatabaseType;
        m_pComDatabaseType = NULL;
    }

    if (m_pCheckPrefix)
    {
        delete m_pCheckPrefix;
        m_pCheckPrefix = NULL;
    }

    if (m_pLinePrefix)
    {
        delete m_pLinePrefix;
        m_pLinePrefix = NULL;
    }

    if (m_pH4Layout)
    {
        m_pH4Layout->removeWidget(m_pLbDatabaseName);
        m_pH4Layout->removeWidget(m_pLineDatabaseName);
        m_pH4Layout->removeWidget(m_pBtnConnect);
    }

    if (m_pLbDatabaseName)
    {
        delete m_pLbDatabaseName;
        m_pLbDatabaseName = NULL;
    }

    if (m_pLineDatabaseName)
    {
        delete m_pLineDatabaseName;
        m_pLineDatabaseName = NULL;
    }

    if (m_pH5Layout)
    {
        m_pH5Layout->removeWidget(m_pLbServiceName);
        m_pH5Layout->removeWidget(m_pLineServiceName);
    }

    if (m_pLbServiceName)
    {
        delete m_pLbServiceName;
        m_pLbServiceName = NULL;
    }

    if (m_pLineServiceName)
    {
        delete m_pLineServiceName;
        m_pLineServiceName = NULL;
    }

    if (m_pH6Layout)
    {
        m_pH6Layout->removeWidget(m_pLbAddress);
        m_pH6Layout->removeWidget(m_pLineAddress);
    }

    if (m_pLbAddress)
    {
        delete m_pLbAddress;
        m_pLbAddress = NULL;
    }

    if (m_pLineAddress)
    {
        delete m_pLineAddress;
        m_pLineAddress = NULL;
    }

    if (m_pH7Layout)
    {
        m_pH7Layout->removeWidget(m_pLbPort);
        m_pH7Layout->removeWidget(m_pLinePort);
    }

    if (m_pLbPort)
    {
        delete m_pLbPort;
        m_pLbPort = NULL;
    }

    if (m_pLinePort)
    {
        delete m_pLinePort;
        m_pLinePort = NULL;
    }

//    if (m_pH8Layout)
//    {
//        m_pH8Layout->removeWidget(m_pLbDbDirectory);
//        m_pH8Layout->removeWidget(m_pLineDbDirectory);
//    }

//    if (m_pLbDbDirectory)
//    {
//        delete m_pLbDbDirectory;
//        m_pLbDbDirectory = NULL;
//    }

//    if (m_pLineDbDirectory)
//    {
//        delete m_pLineDbDirectory;
//        m_pLineDbDirectory = NULL;
//    }

    if (m_pH9Layout)
    {
        m_pH9Layout->removeWidget(m_pLbUser);
        m_pH9Layout->removeWidget(m_pLineUser);
    }

    if (m_pLbUser)
    {
        delete m_pLbUser;
        m_pLbUser = NULL;
    }

    if (m_pLineUser)
    {
        delete m_pLineUser;
        m_pLineUser = NULL;
    }

    if (m_pH10Layout)
    {
        m_pH10Layout->removeWidget(m_pLbPasswd);
        m_pH10Layout->removeWidget(m_pLinePasswd);
    }

    if (m_pLbPasswd)
    {
        delete m_pLbPasswd;
        m_pLbPasswd = NULL;
    }

    if (m_pLinePasswd)
    {
        delete m_pLinePasswd;
        m_pLinePasswd = NULL;
    }

    if (m_pBtnConnect)
    {
        delete m_pBtnConnect;
        m_pBtnConnect = NULL;
    }

    if (m_pVGrpboxLayout)
    {
        QLayoutItem *pItem = NULL;
        while (NULL != (pItem = m_pVMainLayout->takeAt(0)))
        {
            delete pItem;
            pItem = NULL;
        }
        delete m_pVGrpboxLayout;
        m_pVGrpboxLayout = NULL;
    }

    if (m_pVMainLayout)
    {
        m_pVMainLayout->removeWidget(m_pGrpboxDBParam);
        QLayoutItem *pItem = NULL;
        while (NULL != (pItem = m_pVMainLayout->takeAt(0)))
        {
            delete pItem;
            pItem = NULL;
        }
        delete m_pVMainLayout;
        m_pVMainLayout = NULL;
    }

    if (m_pGrpboxDBParam)
    {
        delete m_pGrpboxDBParam;
        m_pGrpboxDBParam = NULL;
    }

    //if (m_pMulLnaguageTool)
    //{
        //delete m_pMulLnaguageTool;
        //m_pMulLnaguageTool = NULL;
    //}

}

void CDBSourceParamWidget::initUI(IMDDataSource *pDataSource)
{
    if(NULL == pDataSource)
    {
        return;
    }

    m_pDataSource = pDataSource;
    //m_pDataArchivesUI = pParent;

    m_pLineDataSrcName->setText(m_pDataSource->name());
    m_pLineDataSrcDecrip->setText(m_pDataSource->description());
    m_pCheckEnable->setChecked(m_pDataSource->isEnable());
    m_pCheckPrefix->setChecked(m_pDataSource->isEnableDataTablePrefix());
    m_pLinePrefix->setText(m_pDataSource->dataTablePrefix());

    IMDDataBaseParam *pDBParam = m_pDataSource->dbParam();
    if(NULL == pDBParam)
    {
        return;
    }      

    DATABASEPARAM::DATABASE_TYPE nType = pDBParam->dbServeType();
    int dbTypeIndex = 0;

    switch(nType)
    {
    case DATABASEPARAM::DB_TYPE_KINGBASE:
        dbTypeIndex = 0;
        break;
//    case DATABASEPARAM::DB_TYPE_SQLLITE:
//        dbTypeIndex = 1;
//        break;
    case DATABASEPARAM::DB_TYPE_MYSQL:
        dbTypeIndex = 1;
        break;
    case DATABASEPARAM::DB_TYPE_SQL:
        dbTypeIndex = 2;
        break;
    default:
        break;
    }

    m_pComDatabaseType->setCurrentIndex(dbTypeIndex);

    m_pLineDatabaseName->setText(pDBParam->dbName());
    m_pLineServiceName->setText(pDBParam->dbServerName());
//    m_pLineDbDirectory->setText(pDBParam->dbPath());
    m_pLineUser->setText(pDBParam->dbUser());
    m_pLinePasswd->setText(pDBParam->dbPassword());
    m_pLineAddress->setText(pDBParam->dbServerIP());

    int iPort = pDBParam->dbServerPort();
    if (-1 == iPort || 0 == iPort)
    {
        m_pLinePort->setText("");
    }
    else
    {
        m_pLinePort->setText(QString("%1").arg(iPort));
    }
}

void CDBSourceParamWidget::save()
{
    if (m_pLbDatabaseName->text().isEmpty())
    {
        m_pLbDatabaseName->setFocus();
        return;
    }

    if(NULL == m_pDataSource)
    {
        return;
    }

    m_pDataSource->setName(m_pLineDataSrcName->text());
    m_pDataSource->setDescription(m_pLineDataSrcDecrip->text());
    m_pDataSource->setEnable(m_pCheckEnable->isChecked());    
    m_pDataSource->setEnableDataTablePrefix(m_pCheckPrefix->isChecked());
    m_pDataSource->setDataTablePrefix(m_pLinePrefix->text());

    IMDDataBaseParam *pDBParam = m_pDataSource->dbParam();
    if(NULL == pDBParam)
    {
        return;
    }

    DATABASEPARAM::DATABASE_TYPE nType = DATABASEPARAM::DB_TYPE_INVALID;

    int iCurIndex = m_pComDatabaseType->currentIndex();
    switch(iCurIndex)
    {
    case 0:
        nType = DATABASEPARAM::DB_TYPE_KINGBASE;
        break;
//    case 1:
//        nType = DATABASEPARAM::DB_TYPE_SQLLITE;
        break;
    case 1:
        nType = DATABASEPARAM::DB_TYPE_MYSQL;
        break;
    case 2:
        nType = DATABASEPARAM::DB_TYPE_SQL;
        break;
    default:
        break;
    }

    pDBParam->setdbServeType(nType);
    pDBParam->setdbServerName(m_pLineServiceName->text());
    pDBParam->setdbServerIP(m_pLineAddress->text());
    pDBParam->setdbName(m_pLineDatabaseName->text());
    pDBParam->setdbUser(m_pLineUser->text());
    pDBParam->setdbPassword(m_pLinePasswd->text());
//    pDBParam->setDbBackUpPath(m_pLineDbDirectory->text());
//    pDBParam->setDBPath(m_pLineDbDirectory->text());
    pDBParam->setdbServerPort(m_pLinePort->text().toInt());
//    m_pDataSourceManager->save(); //为什么屏蔽掉了呢
    m_pDataSourceManager->save();

    return;
}

void CDBSourceParamWidget::changeLanguage()
{
    initLanguage();
}

void CDBSourceParamWidget::initData()
{
    m_pDataSourceManager  = NULL; //! 数据源管理器
    m_pDataSource         = NULL; //! 数据源
    //m_pDataArchivesUI     = NULL; //! 数据归档界面类

    m_pLbDataSrcName                 = NULL; //! 数据源名字标签
    m_pLbDataSrcDecrip               = NULL; //! 数据源描述标签
    m_pLbDatabaseType                = NULL; //! 数据库类型标签
    m_pLbDatabaseName                = NULL; //! 数据库名字标签
    m_pLbServiceName                 = NULL; //! 数据库服务名标签
    m_pLbAddress                     = NULL; //! ip地址标签
    m_pLbPort                        = NULL; //! 端口标签
    m_pLbUser                        = NULL; //! 用户标签
    m_pLbPasswd                      = NULL; //! 密码标签

    m_pLineDataSrcName               = NULL; //! 数据源名字编辑框
    m_pLineDataSrcDecrip             = NULL; //! 数据源描述编辑框
    m_pLineDatabaseName              = NULL; //! 数据库名字编辑框
    m_pLineServiceName               = NULL; //! 数据库服务名编辑框
    m_pLineAddress                   = NULL; //! ip地址编辑框
    m_pLinePort                      = NULL; //! 端口编辑框
    m_pLineUser                      = NULL; //! 用户编辑框
    m_pLinePasswd                    = NULL; //! 密码编辑框
    m_pLinePrefix                    = NULL; //! 表前缀名编辑框

    m_pBtnConnect                    = NULL; //! 链接测试按钮
    m_pComDatabaseType               = NULL; //! 数据类型下拉框
    m_pCheckEnable                   = NULL; //! 启用选择框
    m_pCheckPrefix                   = NULL; //! 启用前缀选择框

    m_pGrpboxDBParam                 = NULL; //! 数据库参数groupbox

    m_pH1Layout                    = NULL; //! 数据源名字水平布局
    m_pH2Layout                    = NULL; //! 数据源描述水平布局
    m_pH3Layout                    = NULL; //! 数据库类型水平布局
    m_pH4Layout                    = NULL; //! 数据库名字水平布局
    m_pH5Layout                    = NULL; //! 数据库服务名水平布局
    m_pH6Layout                    = NULL; //! ip地址水平布局
    m_pH7Layout                    = NULL; //! 端口水平布局
    m_pH9Layout                    = NULL; //! 用户名水平布局
    m_pH10Layout                   = NULL; //! 密码水平布局

    m_pVGrpboxLayout               = NULL; //! 数据库参数groupbox垂直布局
    m_pVMainLayout                 = NULL; //! 全部垂直布局
}

void CDBSourceParamWidget::initLanguage()
{
}


void CDBSourceParamWidget::createUI()
{
    m_pLbDataSrcName = new QLabel(tr("数据源名"), this);
    m_pLbDataSrcName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbDataSrcDecrip = new QLabel(tr("描述"), this);
    m_pLbDataSrcDecrip->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbDatabaseType = new QLabel(tr("数据库类型"), this);
    m_pLbDatabaseType->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbDatabaseName = new QLabel(tr("数据库名"), this);
    m_pLbDatabaseName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbServiceName = new QLabel(tr("服务器名"), this);
    m_pLbServiceName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbAddress = new QLabel(tr("IP地址"), this);
    m_pLbAddress->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbPort = new QLabel(tr("端口"), this);
    m_pLbPort->setAttribute(Qt::WA_DeleteOnClose);

//    m_pLbDbDirectory = new QLabel(tr("数据库目录"), this);
//    m_pLbDbDirectory->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbUser = new QLabel(tr("用户名"), this);
    m_pLbUser->setAttribute(Qt::WA_DeleteOnClose);

    m_pLbPasswd = new QLabel(tr("密码"), this);
    m_pLbPasswd->setAttribute(Qt::WA_DeleteOnClose);

    m_pLineDataSrcName = new QLineEdit(this);
    m_pLineDataSrcName->setAttribute(Qt::WA_DeleteOnClose);

    m_pCheckEnable = new QCheckBox(tr("启用"), this);
//    m_pCheckEnable->setChecked(true);

    m_pCheckPrefix = new QCheckBox(tr("启用表前缀"), this);

    m_pLineDataSrcDecrip = new QLineEdit(this);
    m_pLineDataSrcDecrip->setAttribute(Qt::WA_DeleteOnClose);

    m_pLineDatabaseName = new QLineEdit(this);
    m_pLineDatabaseName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLineServiceName = new QLineEdit(this);
    m_pLineServiceName->setAttribute(Qt::WA_DeleteOnClose);

    m_pLineAddress = new QLineEdit(this);
    m_pLineAddress->setAttribute(Qt::WA_DeleteOnClose);

    m_pLinePort = new QLineEdit(this);
    m_pLinePort->setAttribute(Qt::WA_DeleteOnClose);

//    m_pLineDbDirectory = new QLineEdit(this);
//    m_pLineDbDirectory->setAttribute(Qt::WA_DeleteOnClose);

    m_pLineUser = new QLineEdit(this);
    m_pLineUser->setAttribute(Qt::WA_DeleteOnClose);

    m_pLinePasswd = new QLineEdit(this);
    m_pLinePasswd->setAttribute(Qt::WA_DeleteOnClose);
    m_pLinePasswd->setEchoMode(QLineEdit::Password);

    m_pLinePrefix = new QLineEdit(this);
    m_pLinePrefix->setAttribute(Qt::WA_DeleteOnClose);
    m_pLinePrefix->setVisible(false);

    m_pLineDataSrcName->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLineDataSrcDecrip->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLineDatabaseName->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLineServiceName->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLineAddress->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLinePort->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLineUser->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLinePasswd->setFixedWidth(PARAM_WIDGET_FIXED_W);
//    m_pLineDbDirectory->setFixedWidth(PARAM_WIDGET_FIXED_W);
    m_pLinePasswd->setFixedWidth(PARAM_WIDGET_FIXED_W);

    m_pBtnConnect = new QPushButton(tr("连接测试"), this);

    m_pComDatabaseType = new QComboBox(this);
    m_pComDatabaseType->addItem(tr("kingbase"));
//    m_pComDatabaseType->addItem(tr("sqlite"));
    m_pComDatabaseType->addItem(tr("mysql"));
#ifdef Q_OS_WIN
    m_pComDatabaseType->addItem(tr("sqlserver"));
#endif
    m_pComDatabaseType->setFixedWidth(PARAM_WIDGET_FIXED_W);

    m_pGrpboxDBParam = new QGroupBox(tr("数据库连接信息"), this);

    m_pH1Layout = new QHBoxLayout;
    m_pH2Layout = new QHBoxLayout;
    m_pH3Layout = new QHBoxLayout;
    m_pH4Layout = new QHBoxLayout;
    m_pH5Layout = new QHBoxLayout;
    m_pH6Layout = new QHBoxLayout;
    m_pH7Layout = new QHBoxLayout;
//    m_pH8Layout = new QHBoxLayout;
    m_pH9Layout = new QHBoxLayout;
    m_pH10Layout = new QHBoxLayout;

    m_pVGrpboxLayout = new QVBoxLayout;
    m_pVMainLayout = new QVBoxLayout;

    int maxWidth = calculateLabelWidth(m_pLbDatabaseType);

    m_pH1Layout->addWidget(m_pLbDataSrcName);
    m_pH1Layout->addWidget(m_pLineDataSrcName);
    m_pH1Layout->addWidget(m_pCheckEnable);
    m_pH1Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDataSrcName) + PARAM_LAYOUT_SPACE + 10);
    m_pH1Layout->addStretch(0);

    m_pH2Layout->addWidget(m_pLbDataSrcDecrip);
    m_pH2Layout->addWidget(m_pLineDataSrcDecrip);
    m_pH2Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDataSrcDecrip) + PARAM_LAYOUT_SPACE + 10);
    m_pH2Layout->addStretch(0);

    m_pH3Layout->addWidget(m_pLbDatabaseType);
    m_pH3Layout->addWidget(m_pComDatabaseType);
    m_pH3Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDatabaseType) + PARAM_LAYOUT_SPACE + 10);
    m_pH3Layout->addSpacing(10);
    m_pH3Layout->addWidget(m_pCheckPrefix);
    m_pH3Layout->addWidget(m_pLinePrefix);
    m_pH3Layout->addStretch(0);

    m_pH4Layout->addWidget(m_pLbDatabaseName);
    m_pH4Layout->addWidget(m_pLineDatabaseName);
    m_pH4Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDatabaseName) + PARAM_LAYOUT_SPACE + 10);
    m_pH4Layout->addWidget(m_pBtnConnect);
    m_pH4Layout->addStretch(0);

    m_pH5Layout->addWidget(m_pLbServiceName);
    m_pH5Layout->addWidget(m_pLineServiceName);
    m_pH5Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbServiceName) + PARAM_LAYOUT_SPACE);
    m_pH5Layout->addStretch(0);

    m_pH6Layout->addWidget(m_pLbAddress);
    m_pH6Layout->addWidget(m_pLineAddress);
    m_pH6Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbAddress) + PARAM_LAYOUT_SPACE);
    m_pH6Layout->addStretch(0);

    m_pH7Layout->addWidget(m_pLbPort);
    m_pH7Layout->addWidget(m_pLinePort);
    m_pH7Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbPort) + PARAM_LAYOUT_SPACE);
    m_pH7Layout->addStretch(0);

//    m_pH8Layout->addWidget(m_pLbDbDirectory);
//    m_pH8Layout->addWidget(m_pLineDbDirectory);
//    m_pH8Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDbDirectory) + PARAM_LAYOUT_SPACE);
//    m_pH8Layout->addStretch(0);

    m_pH9Layout->addWidget(m_pLbUser);
    m_pH9Layout->addWidget(m_pLineUser);
    m_pH9Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbUser) + PARAM_LAYOUT_SPACE);
    m_pH9Layout->addStretch(0);

    m_pH10Layout->addWidget(m_pLbPasswd);
    m_pH10Layout->addWidget(m_pLinePasswd);
    m_pH10Layout->setSpacing(maxWidth - calculateLabelWidth(m_pLbPasswd) + PARAM_LAYOUT_SPACE);
    m_pH10Layout->addStretch(0);

    m_pVGrpboxLayout->addLayout(m_pH5Layout);
    m_pVGrpboxLayout->addLayout(m_pH6Layout);
    m_pVGrpboxLayout->addLayout(m_pH7Layout);
//    m_pVGrpboxLayout->addLayout(m_pH8Layout);
    m_pVGrpboxLayout->addLayout(m_pH9Layout);
    m_pVGrpboxLayout->addLayout(m_pH10Layout);
    m_pVGrpboxLayout->setSpacing(PARAM_LAYOUT_SPACE);

    m_pGrpboxDBParam->setLayout(m_pVGrpboxLayout);

    m_pVMainLayout->addLayout(m_pH1Layout);
    m_pVMainLayout->addLayout(m_pH2Layout);
    m_pVMainLayout->addLayout(m_pH3Layout);
    m_pVMainLayout->addLayout(m_pH4Layout);
    m_pVMainLayout->addWidget(m_pGrpboxDBParam);
    m_pVMainLayout->addStretch(0);
    m_pVMainLayout->setSpacing(PARAM_LAYOUT_SPACE);
    m_pVMainLayout->setContentsMargins(PARAM_LAYOUT_SPACE, PARAM_LAYOUT_SPACE, PARAM_LAYOUT_SPACE, PARAM_LAYOUT_SPACE);
    setLayout(m_pVMainLayout);


    connect(m_pBtnConnect, SIGNAL(clicked()), this, SLOT(on_ConnectPushButton_clicked()));
    connect(m_pLineDataSrcName, SIGNAL(textChanged(QString)), this, SLOT(on_NameEdit_textChanged(QString)));

    connect(m_pCheckPrefix, SIGNAL(stateChanged(int)), this, SLOT(on_PrefixState_change(int)));
}

//改变数据源名称后更新左侧树状控件对应子项的名称
//暂时屏蔽掉
void CDBSourceParamWidget::on_NameEdit_textChanged(const QString &arg1)
{
    /*
    if(NULL == m_pDataArchivesUI)
    {
        return;
    }
    QTreeWidget *pQTreeWidget = m_pDataArchivesUI->getQTreeWidget();
    if(NULL == pQTreeWidget)
    {
        return;
    }
    QTreeWidgetItem *pItem = pQTreeWidget->currentItem();
    if(NULL == pItem)
    {
        return;
    }

    if(m_pDataSource)
    {
        m_pDataSource->setName(arg1);
    }
    pItem->setText(0, arg1);
    */
}

bool CDBSourceParamWidget::ConnectDB(CMDDBConnect &adoConnection, IMDDataBaseParam *DBParam, QString &strError)
{
    bool bSuccess = false;
    if(NULL == DBParam)
    {
        return bSuccess;
    }

    QString strDBIP = DBParam->dbServerIP();
    QString strDbName = DBParam->dbName();
    QString strUser = DBParam->dbUser();
    QString strPassword = DBParam->dbPassword();
//    QString strDBPath = DBParam->dbPath();
    int portID = DBParam->dbServerPort();

    if(adoConnection.isConnect())
        adoConnection.close();

    QString strDsn = "";
    DATABASEPARAM::DATABASE_TYPE nType = DBParam->dbServeType();

    switch(nType)
    {
    case DATABASEPARAM::DB_TYPE_SQL:
        strDsn = QString("DRIVER={SQL SERVER};SERVER=%1,%2;DATABASE=%3;uid=%4;pwd=%5;")\
                .arg(strDBIP).arg(portID).arg(strDbName).arg(strUser).arg(strPassword);
        return adoConnection.connectDatabase(nType, strDsn, strError);
    case DATABASEPARAM::DB_TYPE_ACCESS:   
    case DATABASEPARAM::DB_TYPE_DB2:
    case DATABASEPARAM::DB_TYPE_ORACLE:
    case DATABASEPARAM::DB_TYPE_MYSQL:
    case DATABASEPARAM::DB_TYPE_KINGBASE:
        return adoConnection.queryConnectDatabase(nType, strDbName, strDBIP, strUser, strPassword, portID, strError);
    case DATABASEPARAM::DB_TYPE_SQLLITE:
        strDbName = QCoreApplication::applicationDirPath() + "/" + strDbName;
        strDbName += ".db";
        return adoConnection.queryConnectDatabase(nType, strDbName, strDBIP, strUser, strPassword, portID, strError);
    default:
        break;
    }

    return false;
}

int CDBSourceParamWidget::calculateLabelWidth(QLabel *pLb)
{
    QFontMetrics met(pLb->font());
    return met.width(pLb->text());
}

void CDBSourceParamWidget::on_ConnectPushButton_clicked()
{    
    save();

    if(NULL == m_pDataSource)
    {
        return;
    }

    CMDDBConnect adoConnection;
    QString strTitle = "";
    QString strText = "";
    QString strError = "";
    QMessageBox message(this);
    message.addButton(tr("确认"), QMessageBox::YesRole);
    if(ConnectDB(adoConnection, m_pDataSource->dbParam(), strError))
    {
        strTitle = tr("成功");
        strText = tr("连接成功");
    }
    else
    {
        strTitle = tr("失败");
        strText += tr("连接失败");
        strText += strError;
    }

    message.setWindowTitle(strTitle);
    message.setText(strText);
    //CMDMultiLanguage::instance()->translateWidget(&message);
    message.exec();

    adoConnection.close();    
}

void CDBSourceParamWidget::on_PrefixState_change(int /*state*/)
{
    m_pLinePrefix->setVisible(m_pCheckPrefix->isChecked());
}
