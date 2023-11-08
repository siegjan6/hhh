#include "MDDataGroupTableWidget.h"
#include "IMDDataGroupManager.h"
#include "IMDDataArchives.h"
#include "MDDataArchivesInterface/IMDDataGroup.h"
#include "IMDDataItem.h"
#include "MDDataGroupFactory.h"
#include "MDDataArchivesTableWidget.h"
#include "MDDataSourceFactory.h"
#include "IMDDataSource.h"
#include "IMDDataField.h"
#include "IMDDataSourceManager.h"

#include <QPushButton>
#include <QTableView>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QGroupBox>
#include <IMDDataTable.h>

#define GRP_LAYOUT_SPACE                                            15

CMDDataGroupTableWidget::CMDDataGroupTableWidget(QWidget *parent)
    : QDialog(parent)
{
    resize(747, 500);
    initData();
    initUI();
    signalsSotConnect();
}

CMDDataGroupTableWidget::~CMDDataGroupTableWidget()
{
    if (m_pHFirstLayout)
    {
        m_pHFirstLayout->removeWidget(m_pLbName);
        m_pHFirstLayout->removeWidget(m_pLineName);
//        m_pHFirstLayout->removeWidget(m_pCheckEnable);
    }

    if (m_pHSecondLayout)
    {
        m_pHSecondLayout->removeWidget(m_pLbDescri);
        m_pHSecondLayout->removeWidget(m_pLineDescri);
    }

    if (m_pHThirdLayout)
    {
        m_pHThirdLayout->removeWidget(m_pLbTabDescrip);
        m_pHThirdLayout->removeWidget(m_pBtnAdd);
        m_pHThirdLayout->removeWidget(m_pBtnRemove);
//        m_pHThirdLayout->removeWidget(m_pBtnOk);
//        m_pHThirdLayout->removeWidget(m_pBtnCancel);
    }

//    if (m_pHFourthLayout)
//    {
//        m_pHFourthLayout->removeWidget(m_pTableWidget);
//    }

    if (m_pVMainLayout)
    {
        m_pVMainLayout->removeWidget(m_pTableWidget);
        QLayoutItem *pItem = NULL;
        while (NULL != (pItem = m_pVMainLayout->takeAt(0)))
        {
            delete pItem;
            pItem = NULL;
        }
        delete m_pVMainLayout;
        m_pVMainLayout = NULL;
    }

    if (m_pLbName)
    {
        delete m_pLbName;
        m_pLbName = NULL;
    }

//    if (m_pCheckEnable)
//    {
//        delete m_pCheckEnable;
//        m_pCheckEnable = NULL;
//    }

    if (m_pLineDescri)
    {
        delete m_pLineDescri;
        m_pLineDescri = NULL;
    }

    if (m_pLbDescri)
    {
        delete m_pLbDescri;
        m_pLbDescri = NULL;
    }

//    if (m_pBtnCancel)
//    {
//        delete m_pBtnCancel;
//        m_pBtnCancel = NULL;
//    }

//    if (m_pBtnOk)
//    {
//        delete m_pBtnOk;
//        m_pBtnOk = NULL;
//    }

    if (m_pBtnAdd)
    {
        delete m_pBtnAdd;
        m_pBtnAdd = NULL;
    }

    if (m_pBtnRemove)
    {
        delete m_pBtnRemove;
        m_pBtnRemove = NULL;
    }

    if (m_pTableWidget)
    {
        delete m_pTableWidget;
        m_pTableWidget = NULL;
    }

//    if (m_pGrpboxTab)
//    {
//        delete m_pGrpboxTab;
//        m_pGrpboxTab = NULL;
//    }

    if (m_pLbTabDescrip)
    {
        delete m_pLbTabDescrip;
        m_pLbTabDescrip = NULL;
    }
}

void CMDDataGroupTableWidget::setDataGrpProperty(/*IMDRTDBManagerUI *pRTDBManagerUI, */IMDDataArchives *pDataArchives, IMDDataGroup *pDataGrp)
{
    if (NULL == pDataGrp || NULL == pDataArchives /*|| NULL == pRTDBManagerUI*/)
    {
        return;
    }

    m_pDataArchives = pDataArchives;
    //m_pRTDBManagerUI = pRTDBManagerUI;

    m_pDataGrp = pDataGrp;
    m_pLineName->setText(m_pDataGrp->name());
    m_pLineDescri->setText(m_pDataGrp->description());
//    m_pCheckEnable->setChecked(m_pDataGrp->isEnable());
    m_pTableWidget->initData(pDataGrp);
}

//?为什么不用qt designer?
void CMDDataGroupTableWidget::initUI()
{
    m_pLbName = new QLabel(this);
    m_pLbName->setText(tr("数据组名"));
    m_pLineName = new QLineEdit(this);

    m_pLbDescri = new QLabel(this);
    m_pLbDescri->setText(tr("描述"));
    m_pLineDescri = new QLineEdit(this);

//    m_pCheckEnable = new QCheckBox(this);
//    m_pCheckEnable->setText("启用");
//    m_pCheckEnable->setCheckable(true);
//    m_pCheckEnable->setChecked(true);

//    m_pBtnCancel = new QPushButton(("取消"), this);

//    m_pBtnOk = new QPushButton(("确定"), this);

    m_pBtnAdd = new QPushButton(tr("增加"), this);

    m_pBtnRemove = new QPushButton(tr("删除"), this);

    m_pBtnAdd->setFixedSize(75, 23);
//    m_pBtnOk->setFixedSize(75, 23);
//    m_pBtnCancel->setFixedSize(75, 23);
    m_pBtnRemove->setFixedSize(75, 23);    

    m_pTableWidget = new CMDDataArchivesTableWidget(this);
    QStringList headerList;
    headerList << tr("名称") << tr("数据类型") << tr("描述");
    m_pTableWidget->setHorizontalHeaderText(headerList);

    m_pLbTabDescrip = new QLabel(tr("数据组成名"), this);
//    m_pGrpboxTab = new QGroupBox(this);

    m_pHFirstLayout = new QHBoxLayout;
    m_pHSecondLayout = new QHBoxLayout;
    m_pHThirdLayout = new QHBoxLayout;
//    m_pHFourthLayout = new QHBoxLayout;
    m_pVMainLayout = new QVBoxLayout;

    int maxWidth = calculateLabelWidth(m_pLbName);

    m_pHFirstLayout->addWidget(m_pLbName);
    m_pHFirstLayout->setSpacing(maxWidth - calculateLabelWidth(m_pLbName) + GRP_LAYOUT_SPACE);
    m_pHFirstLayout->addWidget(m_pLineName);
//    m_pHFirstLayout->addWidget(m_pCheckEnable);

    m_pHSecondLayout->addWidget(m_pLbDescri);
    m_pHSecondLayout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDescri) + GRP_LAYOUT_SPACE);
    m_pHSecondLayout->addWidget(m_pLineDescri);

    m_pHThirdLayout->addWidget(m_pLbTabDescrip);
    m_pHThirdLayout->addStretch(0);
    m_pHThirdLayout->addWidget(m_pBtnAdd);
    m_pHThirdLayout->addWidget(m_pBtnRemove);    
//    m_pHThirdLayout->addWidget(m_pBtnOk);
//    m_pHThirdLayout->addWidget(m_pBtnCancel);

//    m_pHFourthLayout->addWidget(m_pTableWidget);
//    m_pGrpboxTab->setLayout(m_pHFourthLayout);

    m_pVMainLayout->addLayout(m_pHFirstLayout);
    m_pVMainLayout->addSpacing(GRP_LAYOUT_SPACE);
    m_pVMainLayout->addLayout(m_pHSecondLayout);
    m_pVMainLayout->addSpacing(GRP_LAYOUT_SPACE);
    m_pVMainLayout->addLayout(m_pHThirdLayout);
    m_pVMainLayout->addWidget(m_pTableWidget);
//    m_pVMainLayout->setSpacing(GRP_LAYOUT_SPACE);
    setLayout(m_pVMainLayout);
}

void CMDDataGroupTableWidget::signalsSotConnect()
{
//    connect(m_pBtnCancel, SIGNAL(clicked()), this, SLOT(close()));
//    connect(m_pBtnOk, SIGNAL(clicked()), this, SLOT(on_okButton_clicked()));
    connect(m_pBtnAdd, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
    connect(m_pBtnRemove, SIGNAL(clicked()), this, SLOT(on_removeButton_clicked()));
    connect(m_pLineName, SIGNAL(textChanged(QString)), this, SLOT(on_name_textchange(QString)));
}

int CMDDataGroupTableWidget::calculateLabelWidth(QLabel *pLb)
{
    QFontMetrics met(pLb->font());
    return met.width(pLb->text());
}

void CMDDataGroupTableWidget::save()
{
    if (NULL == m_pDataGrp || NULL == m_pDataArchives)
    {
        return;
    }

    QMessageBox message(this);
    message.addButton(tr("确认"), QMessageBox::YesRole);
    if (m_pLineName->text().isEmpty())
    {
        message.setWindowTitle(tr("提示"));
        message.setText(tr("组名不能为空！"));
        message.exec();

        return;
    }

    m_pDataGrp->setName(m_pLineName->text());
    m_pDataGrp->setDescription(m_pLineDescri->text());
    m_pDataArchives->dataGroupManager()->save();

    checkDataSrc();
}

void CMDDataGroupTableWidget::initData()
{
    m_pBtnAdd            = NULL; //! 增加按钮
    m_pBtnRemove         = NULL; //! 删除按钮

    m_pLbName            = NULL; //! 数据组名标签
    m_pLineName          = NULL; //! 数据组名编辑框
    m_pLbDescri          = NULL; //! 数据组描述标签
    m_pLineDescri        = NULL; //! 数据组描述编辑框

    m_pLbTabDescrip      = NULL; //! 数据组成员
    m_pGrpboxTab         = NULL; //! groupbox

    m_pHFirstLayout      = NULL; //! 数据组水平布局
    m_pHSecondLayout     = NULL; //! 数据组描述水平布局
    m_pHThirdLayout      = NULL; //! 操作水平布局
    m_pVMainLayout       = NULL; //! 整体水平布局

    m_pDataGrp           = NULL; //! 数据组
    m_pDataArchives      = NULL; //! 数据归档
    //m_pRTDBManagerUI     = NULL; //! RTDBUI管理器
    m_pFramework         = NULL; //! 数据组配置界面
    m_pDataArchivesUI    = NULL; //! 数据归档窗体

    m_pTableWidget       = NULL; //! 数据归档表格界面

    //m_pMulLnaguageTool = NULL; //! 多语言翻译工具
}

void CMDDataGroupTableWidget::checkDataSrc()
{
    if(NULL == m_pDataArchives)
    {
        return;
    }

    IMDDataSourceManager *pSrcMana = m_pDataArchives->dataSourceManager();
    if(NULL == pSrcMana)
    {
        return;
    }

    IMDDataSource *pDataSrc = NULL;
    IMDDataTable  *pDataTab = NULL;
    IMDDataField  *pField   = NULL;
    IMDDataItem   *pItem    = NULL;

    QString        strName  = "";

    for (int i = 0; i < pSrcMana->dataSourceCount(); ++i)
    {
        pDataSrc = pSrcMana->getDataSource(i);
        if (NULL == pDataSrc)
        {
            continue;
        }

        if (pDataSrc->isShowTable())
        {
            continue;
        }

        pDataTab = pDataSrc->getDataTable(m_pDataGrp->name());
        if (pDataTab)
        {
            pDataSrc->removeDataTable(pDataTab);
            delete pDataTab;
            pDataTab = NULL;
        }

        pDataTab = CMDDataSourceFactory::createDataTable();
        pDataTab->setName(m_pDataGrp->name());
        pDataTab->setDataGroupName(m_pDataGrp->name());
        for (int j = 0; j < m_pDataGrp->itemCount(); ++j)
        {
            pItem = m_pDataGrp->getItem(j);
            if (NULL == pItem)
            {
                continue;
            }

            pField = CMDDataSourceFactory::createDataField();
            if (NULL == pField)
            {
                continue;
            }

            strName = pItem->name();
            pField->setName(strName.replace(".", "_"));
            pField->setDataItemName(strName);
            pField->setDataType(pItem->dataType());
            pField->setDataItemDataType(pItem->dataType());
            pField->setPrimarykey(false);
            pField->enableNull(true);
            pField->setSize(255);
            pField->setDescription(pItem->description());
            pDataTab->addDataField(pField);
        }

        pDataSrc->addDataTable(pDataTab);
    }

    pSrcMana->save();
}

void CMDDataGroupTableWidget::on_cancelButton_clicked()
{
    if (m_pDataGrp)
    {
        delete m_pDataGrp;
        m_pDataGrp = NULL;
    }
}

void CMDDataGroupTableWidget::on_addButton_clicked()
{
    /*
    if ((NULL == m_pRTDBManagerUI) || (NULL == m_pDataGrp) || (NULL == m_pDataArchives))
    {
        return;
    }

    QList<IMDVariable*> varNameList;
    m_pRTDBManagerUI->selectVariables(varNameList);

    IMDDataItem *pItem = NULL;
    IMDVariable *pVarVal = NULL;
    int count = varNameList.count();

    for(int i = 0; i < count; ++i)
    {
        pVarVal = varNameList.at(i);
        if(NULL == pVarVal)
        {
            continue;
        }

        pItem = m_pDataGrp->getItem(pVarVal->name());
        if(NULL != pItem)
        {
            continue;
        }

        pItem = CMDDataGroupFactory::createDataItem();
        if(NULL == pItem)
        {
            continue;
        }

        pItem->setName(pVarVal->name());
        pItem->setDdescription(pVarVal->description());
        pItem->setFixed(false);
        pItem->setDataType(pVarVal->dataType());
        m_pDataGrp->addItem(pItem);
    }
    */

    m_pTableWidget->initData(m_pDataGrp);
}

void CMDDataGroupTableWidget::on_removeButton_clicked()
{
    if ((NULL == m_pDataGrp) || (NULL == m_pDataArchives) || (NULL == m_pTableWidget))
    {
        return;
    }

    QList<QTableWidgetItem*> selectedItems = m_pTableWidget->selectedItems();
    if (selectedItems.isEmpty())
    {
        return;
    }


    IMDDataItem *pItem = NULL;
    pItem = m_pDataGrp->getItem(selectedItems.at(0)->text());
    if(NULL == pItem)
    {
        return;
    }

    if (pItem->isFixed())
    {
        QMessageBox message(this);
        message.setWindowTitle(tr("提示"));
        message.addButton(tr("确认"), QMessageBox::YesRole);
        QString strFront = "\'" + pItem->name() + "\'";;
        QString strName = tr("为系统属性，不允许删除！");
        message.setText(strFront + strName);
        message.exec();

        return;
    }

    m_pDataGrp->removeItem(pItem);
    m_pTableWidget->initData(m_pDataGrp);
}

void CMDDataGroupTableWidget::on_name_textchange(const QString &text)
{
    //修改数据组名 并设置树形控件对应节点的名字

    /*
    if (NULL == m_pDataArchivesUI)
    {
        return;
    }

    QTreeWidgetItem *pItem = m_pDataArchivesUI->getQTreeWidget()->currentItem();
    if (NULL == pItem)
    {
        return;
    }

    if(m_pDataGrp)
    {
        m_pDataGrp->setName(text);
    }

    pItem->setText(0, text);
    */
}
