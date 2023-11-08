#include "MDDataSrcTabWidget.h"
#include "IMDDataGroupManager.h"
#include "IMDDataSourceManager.h"
#include "IMDDataArchives.h"
#include "MDDataSourceFactory.h"
#include "IMDDataGroup.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"
#include "IMDDataItem.h"
#include "DbTableFieldDialog.h"
#include "DataArchivesTabModel.h"
#include "DataArchivesUI.h"
#include "MDMultiLanguage.h"

#include <QTreeWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTableView>
#include <QCheckBox>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#define DATASRC_CHECK_NULL_PTR  if ((NULL == m_pDataTable) || (NULL == m_pTabViewModel) \
                                || (NULL == m_pDataArchives)) \
                                { \
                                    return; \
                                }

CMDDataSrcTabWidget::CMDDataSrcTabWidget(QWidget *parent)
    : QDialog(parent)
{
    resize(DATASRC_WIDGET_INIT_W, DATASRC_WIDGET_INIT_H);
    initData();
    initUI();
    initTableView();
    initLanguage();
    signalsSotConnect();    
}

CMDDataSrcTabWidget::~CMDDataSrcTabWidget()
{
    if (m_pMulLnaguageTool)
    {
        delete m_pMulLnaguageTool;
        m_pMulLnaguageTool = NULL;
    }

    if (m_pFirstLayout)
    {
        m_pFirstLayout->removeWidget(m_pLbDataSrc);
        m_pFirstLayout->removeWidget(m_pLineSrcName);
    }

    if (m_pSecondLayout)
    {
        m_pSecondLayout->removeWidget(m_pLbDataGrpName);
        m_pSecondLayout->removeWidget(m_pLineDataGrp);
    }

    if (m_pThirdLayout)
    {
        m_pThirdLayout->removeWidget(m_pLbTableName);
        m_pThirdLayout->removeWidget(m_pLineTableName);
    }

    if (m_pFourthLayout)
    {
        m_pFourthLayout->removeWidget(m_pLbDescri);
        m_pFourthLayout->removeWidget(m_pLineDescri);
    }

    if (m_pFifthLayout)
    {
        m_pFifthLayout->removeWidget(m_pLbTabDescrip);
        m_pFifthLayout->removeWidget(m_pBtnCheckDefault);
        m_pFifthLayout->removeWidget(m_pBtnAddField);
        m_pFifthLayout->removeWidget(m_pBtnEditField);
        m_pFifthLayout->removeWidget(m_pBtnRemoveField);
    }

    if (m_p6thLayout)
    {
        m_p6thLayout->removeWidget(m_pCbEnable);
    }

//    if (m_pSixthLayout)
//    {
//        m_pSixthLayout->removeWidget(m_pTabView);
//    }

    if (m_pMainLayout)
    {
        m_pMainLayout->removeWidget(m_pTabView);
        QLayoutItem *pItem = NULL;
        while (NULL != (pItem = m_pMainLayout->takeAt(0)))
        {
            delete pItem;
            pItem = NULL;
        }
        delete m_pMainLayout;
        m_pMainLayout = NULL;
    }

    if (m_pLineTableName)
    {
        delete m_pLineTableName;
        m_pLineTableName = NULL;
    }

    if (m_pLineDescri)
    {
        delete m_pLineDescri;
        m_pLineDescri = NULL;
    }

    if (m_pLineDataGrp)
    {
        delete m_pLineDataGrp;
        m_pLineDataGrp = NULL;
    }

    if (m_pBtnCheckDefault)
    {
        delete m_pBtnCheckDefault;
        m_pBtnCheckDefault = NULL;
    }

    if (m_pBtnAddField)
    {
        delete m_pBtnAddField;
        m_pBtnAddField = NULL;
    }

    if (m_pBtnRemoveField)
    {
        delete m_pBtnRemoveField;
        m_pBtnRemoveField = NULL;
    }

    if (m_pBtnEditField)
    {
        delete m_pBtnEditField;
        m_pBtnEditField = NULL;
    }

    if (m_pLbDataSrc)
    {
        delete m_pLbDataSrc;
        m_pLbDataSrc = NULL;
    }

    if (m_pLineSrcName)
    {
        delete m_pLineSrcName;
        m_pLineSrcName = NULL;
    }

    if (m_pLbDescri)
    {
        delete m_pLbDescri;
        m_pLbDescri = NULL;
    }

    if (m_pLbTableName)
    {
        delete m_pLbTableName;
        m_pLbTableName = NULL;
    }

    if (m_pLbDataGrpName)
    {
        delete m_pLbDataGrpName;
        m_pLbDataGrpName = NULL;
    }

    if (m_pCbEnable)
    {
        delete m_pCbEnable;
        m_pCbEnable = NULL;
    }

    if (m_pTabView)
    {
        delete m_pTabView;
        m_pTabView = NULL;
    }

//    if (m_pSixthLayout)
//    {
//        delete m_pSixthLayout;
//        m_pSixthLayout = NULL;
//    }

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

void CMDDataSrcTabWidget::initData(IMDDataTable *pTable, const QString &grpName, const QString &dataSrcName)
{
    if (NULL == pTable)
    {
        return;
    }

    m_pLineDataGrp->setText(grpName);
    m_pLineSrcName->setText(dataSrcName);

    m_iDefaultFlag = false;
    m_pDataTable = pTable;
    m_pLineTableName->setText(pTable->name());
    m_pLineDescri->setText(pTable->description());
    m_pCbEnable->setChecked(pTable->isEnable());

//    check_stateChange(m_pCheckDefault->checkState());

    if(m_pTabViewModel)
    {
        m_pTabViewModel->setDataTable(pTable);
    }
}

void CMDDataSrcTabWidget::changeLanguage()
{
    initLanguage();
}

void CMDDataSrcTabWidget::save()
{
    if(NULL == m_pDataTable)
    {
        return;
    }

    m_pDataTable->setName(m_pLineTableName->text());
    m_pDataTable->setDescription(m_pLineDescri->text());
    m_pDataTable->setDataGroupName(m_pLineDataGrp->text());
    m_pDataTable->setEnable(m_pCbEnable->isChecked());
    m_pDataArchives->dataSourceManager()->save();
}

void CMDDataSrcTabWidget::initData()
{
    m_iDefaultFlag         = false;  //! 启用默认字段标识
    m_pLineTableName       = NULL;   //! 表名编辑框
    m_pLineDescri          = NULL;   //! 描述编辑框
    m_pLineDataGrp         = NULL;   //! 数据组编辑框
    m_pBtnCheckDefault     = NULL;   //! 启用默认字段按钮
    m_pBtnAddField         = NULL;   //! 增加字段按钮
    m_pBtnRemoveField      = NULL;   //! 删除字段按钮
    m_pBtnEditField        = NULL;   //! 编辑字段按钮
    m_pLbDataSrc           = NULL;   //! 数据源标签
    m_pLineSrcName         = NULL;   //! 数据源名编辑框
    m_pLbDescri            = NULL;   //! 描述标签
    m_pLbTableName         = NULL;   //! 表名标签
    m_pLbDataGrpName       = NULL;   //! 数据组标签
    m_pLbTabDescrip        = NULL;   //! 数据表描述
    m_pTabView             = NULL;   //! tableview
    m_pGrpboxTab           = NULL;   //! goupbox
    m_pTabViewModel        = NULL;   //! 表模型
    m_pDataTable           = NULL;   //! 表
    m_pDataArchives        = NULL;   //! 数据归档
    m_pDataArchivesUI      = NULL;   //! 数据归档窗体
    m_pFirstLayout         = NULL;   //! 数据源水平布局
    m_pSecondLayout        = NULL;   //! 数据组水平布局
    m_pThirdLayout         = NULL;   //! 数据表水平布局
    m_pFourthLayout        = NULL;   //! 描述水平布局
    m_pFifthLayout         = NULL;   //! 操作水平布局
    m_p6thLayout           = NULL;   //! 使能水平布局
    m_pMainLayout          = NULL;   //! 整体垂直布局

    m_pMulLnaguageTool = NULL; //! 多语言翻译工具
}

void CMDDataSrcTabWidget::initUI()
{
    //数据源
    m_pLbDataSrc = new QLabel(tr("数据源名"), this);

    m_pLineSrcName = new QLineEdit(this);
    m_pLineSrcName->setReadOnly(true);

    //删除字段
    m_pBtnRemoveField = new QPushButton(tr("删除"), this);
    //编辑字段
    m_pBtnEditField = new QPushButton(tr("编辑"), this);

    //数据组
    m_pLbDataGrpName = new QLabel(tr("模板表名"), this);

    m_pLineDataGrp = new QLineEdit(this);
    m_pLineDataGrp->setReadOnly(true);

    //表名
    m_pLbTableName = new QLabel(tr("数据表名"), this);

    m_pLineTableName = new QLineEdit(this);

    //描述
    m_pLbDescri = new QLabel(tr("描述"), this);

    m_pLineDescri = new QLineEdit(this);

    //使能
    // 总使能
    m_pCbEnable = new QCheckBox(tr("启用"), this);

    //增加字段
    m_pBtnAddField = new QPushButton(tr("增加"), this);

    //启用默认字段选择框
    m_pBtnCheckDefault = new QPushButton(tr("默认字段"), this);

    //表视图
    m_pTabView = new QTableView(this);
    m_pTabView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pTabView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pTabView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_pTabView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    m_pLbTabDescrip = new QLabel(tr("数据表字段"), this);
//    m_pGrpboxTab = new QGroupBox(this);

    m_pFirstLayout = new QHBoxLayout;
    m_pSecondLayout = new QHBoxLayout;
    m_pThirdLayout = new QHBoxLayout;
    m_pFourthLayout = new QHBoxLayout;
    m_pFifthLayout = new QHBoxLayout;
    m_p6thLayout = new QHBoxLayout;
//    m_pSixthLayout = new QHBoxLayout;
    m_pMainLayout = new QVBoxLayout;

    int maxWidth = calculateLabelWidth(m_pLbDataGrpName);

    m_pFirstLayout->addWidget(m_pLbDataSrc);
    m_pFirstLayout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDataSrc) + SRC_LAYOUT_SPACE);
    m_pFirstLayout->addWidget(m_pLineSrcName);
//    m_pFirstLayout->addStretch(0);

    m_pSecondLayout->addWidget(m_pLbDataGrpName);
    m_pSecondLayout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDataGrpName) + SRC_LAYOUT_SPACE);
    m_pSecondLayout->addWidget(m_pLineDataGrp);
//    m_pSecondLayout->addStretch(0);

    m_pThirdLayout->addWidget(m_pLbTableName);
    m_pThirdLayout->setSpacing(maxWidth - calculateLabelWidth(m_pLbTableName) + SRC_LAYOUT_SPACE);
    m_pThirdLayout->addWidget(m_pLineTableName);

    m_pFourthLayout->addWidget(m_pLbDescri);
    m_pFourthLayout->setSpacing(maxWidth - calculateLabelWidth(m_pLbDescri) + SRC_LAYOUT_SPACE);
    m_pFourthLayout->addWidget(m_pLineDescri);

    m_p6thLayout->addWidget(m_pCbEnable);
    m_p6thLayout->setSpacing(maxWidth - m_pCbEnable->width() + SRC_LAYOUT_SPACE);

    m_pFifthLayout->addWidget(m_pLbTabDescrip);
    m_pFifthLayout->addStretch(0);
    m_pFifthLayout->addWidget(m_pBtnAddField);
    m_pFifthLayout->addWidget(m_pBtnEditField);
    m_pFifthLayout->addWidget(m_pBtnRemoveField);    
    m_pFifthLayout->addWidget(m_pBtnCheckDefault);

//    m_pSixthLayout->addWidget(m_pTabView);
//    m_pGrpboxTab->setLayout(m_pSixthLayout);

    m_pMainLayout->addLayout(m_pFirstLayout);
    m_pMainLayout->addSpacing(SRC_LAYOUT_SPACE);
    m_pMainLayout->addLayout(m_pSecondLayout);
    m_pMainLayout->addSpacing(SRC_LAYOUT_SPACE);
    m_pMainLayout->addLayout(m_pThirdLayout);
    m_pMainLayout->addSpacing(SRC_LAYOUT_SPACE);
    m_pMainLayout->addLayout(m_pFourthLayout);
    m_pMainLayout->addSpacing(SRC_LAYOUT_SPACE);
    m_pMainLayout->addLayout(m_p6thLayout);
    m_pMainLayout->addSpacing(SRC_LAYOUT_SPACE);
    m_pMainLayout->addLayout(m_pFifthLayout);
    m_pMainLayout->addWidget(m_pTabView);
    m_pMainLayout->setContentsMargins(SRC_LAYOUT_SPACE, SRC_LAYOUT_SPACE, SRC_LAYOUT_SPACE, SRC_LAYOUT_SPACE);
    setLayout(m_pMainLayout);
}

void CMDDataSrcTabWidget::initLanguage()
{
    if (NULL == m_pMulLnaguageTool)
    {
        m_pMulLnaguageTool = CMDMultiLanguage::createWidgetTool();
        m_pMulLnaguageTool->init(this);
    }
    m_pMulLnaguageTool->changeLanguage();
}

void CMDDataSrcTabWidget::signalsSotConnect()
{
    connect(m_pBtnAddField, SIGNAL(clicked()), this, SLOT(on_AddField_clicked()));
    connect(m_pBtnRemoveField, SIGNAL(clicked()), this, SLOT(on_removeField_clicked()));
    connect(m_pBtnEditField, SIGNAL(clicked()), this, SLOT(on_editField_clicked()));

    connect(m_pBtnCheckDefault, SIGNAL(clicked()), this, SLOT(check_stateChange()));

    connect(m_pTabView, SIGNAL(doubleClicked(const QModelIndex&)),
            this, SLOT(on_tabView_doubleClicked(const QModelIndex &)));

    connect(m_pLineTableName, SIGNAL(editingFinished()), this, SLOT(on_tableName_editingFinished()));
    connect(m_pLineDescri, SIGNAL(editingFinished()), this, SLOT(on_tableDescri_editingFinished()));

    connect(m_pLineTableName, SIGNAL(textChanged(QString)), this, SLOT(on_name_textchange(QString)));
}

void CMDDataSrcTabWidget::initTableView()
{
    m_pTabViewModel = new CDataArchivesTabModel;

    QStringList headerList;
    headerList << tr("字段名") << tr("表模板成员")
               << tr("数据类型") << tr("主键") << tr("允许为空") << tr("描述")
               << tr("实时曲线") << tr("历史曲线") << tr("历史表格") << tr("历史过滤") << tr("排序(数字升序)") << tr("线宽") << tr("颜色") << tr("扩展")
                  ;
    m_pTabViewModel->insertColumn(0, CMDMultiLanguage::instance()->value(headerList.at(0)));
    m_pTabViewModel->insertColumn(1, CMDMultiLanguage::instance()->value(headerList.at(1)));
    m_pTabViewModel->insertColumn(2, CMDMultiLanguage::instance()->value(headerList.at(2)));
    m_pTabViewModel->insertColumn(3, CMDMultiLanguage::instance()->value(headerList.at(3)));
    m_pTabViewModel->insertColumn(4, CMDMultiLanguage::instance()->value(headerList.at(4)));
    m_pTabViewModel->insertColumn(5, CMDMultiLanguage::instance()->value(headerList.at(5)));
    m_pTabViewModel->insertColumn(6, CMDMultiLanguage::instance()->value(headerList.at(6)));
    m_pTabViewModel->insertColumn(7, CMDMultiLanguage::instance()->value(headerList.at(7)));
    m_pTabViewModel->insertColumn(8, CMDMultiLanguage::instance()->value(headerList.at(8)));
    m_pTabViewModel->insertColumn(9, CMDMultiLanguage::instance()->value(headerList.at(9)));
    m_pTabViewModel->insertColumn(10, CMDMultiLanguage::instance()->value(headerList.at(10)));
    m_pTabViewModel->insertColumn(11, CMDMultiLanguage::instance()->value(headerList.at(11)));
    m_pTabViewModel->insertColumn(12, CMDMultiLanguage::instance()->value(headerList.at(12)));
    m_pTabViewModel->insertColumn(13, CMDMultiLanguage::instance()->value(headerList.at(13)));
    //m_pTabViewModel->setDisplayInfo(this, OnFieldDisplayInfo);
//    m_pTabViewModel->setItemCount(0);

//    m_pTabView->setColumnWidth(0, 100);
//    m_pTabView->setColumnWidth(1, 100);
//    m_pTabView->setColumnWidth(2, 100);
//    m_pTabView->setColumnWidth(3, 100);

    m_pTabView->setModel(m_pTabViewModel);
}

int CMDDataSrcTabWidget::calculateLabelWidth(QLabel *pLb)
{
    QFontMetrics met(pLb->font());
    return met.width(pLb->text());
}

void CMDDataSrcTabWidget::removeDefaultField()
{
    DATASRC_CHECK_NULL_PTR

    IMDDataGroup *pDataGrp = m_pDataArchives->dataGroupManager()->getDataGroup(m_pDataTable->dataGroupName());
    if (NULL == pDataGrp)
    {
        return;
    }

    m_pDataTable->removeAllDataField();

    m_pTabViewModel->setDataTable(m_pDataTable);
    m_pDataArchives->dataSourceManager()->save();
}

void CMDDataSrcTabWidget::addDefaultField()
{
    DATASRC_CHECK_NULL_PTR

    IMDDataGroup *pDataGrp = m_pDataArchives->dataGroupManager()->getDataGroup(m_pDataTable->dataGroupName());
    if (NULL == pDataGrp)
    {
        return;
    }

    IMDDataField *pField = NULL;
    IMDDataItem *pItem = NULL;
    MDDataType type = MDDataType::Int32;
    int uSize = 0;
    int count = pDataGrp->itemCount();
    for (int i = 0; i < count; ++i)
    {
        pField = CMDDataSourceFactory::createDataField();
        pItem = pDataGrp->getItem(i);
        if (NULL == pField || NULL == pItem)
        {
            continue;
        }

        type = pItem->dataType();
        switch(type)
        {
        case MDDataType::Bool:
        case MDDataType::Byte:
        case MDDataType::SByte:
        case MDDataType::Int16:
        case MDDataType::UInt16:
        case MDDataType::Int32:
        case MDDataType::UInt32:
            uSize = 4;
            break;
        case MDDataType::Float:
        case MDDataType::Int64:
        case MDDataType::UInt64:
        case MDDataType::Double:
        case MDDataType::DateTime:
            uSize = 8;
            break;
        case MDDataType::String:
            uSize = 250;
            break;
        default:
            uSize = 250;
            break;
        }

        pField->setDataItemName(pItem->name());
        QString strTmpName = pItem->name();
        strTmpName.replace(".", "_");
        pField->setName(strTmpName);
        pField->setDataType(type);
        pField->setDataItemDataType(type);
        pField->setPrimarykey(false);
        pField->enableNull(true);
        pField->setSize(uSize);
        pField->setDescription(pItem->description());

        m_pDataTable->addDataField(pField);
    }

    m_pTabViewModel->setDataTable(m_pDataTable);
    m_pDataArchives->dataSourceManager()->save();
}

void CMDDataSrcTabWidget::on_name_textchange(const QString &text)
{
    if (NULL == m_pDataArchivesUI)
    {
        return;
    }

    QTreeWidgetItem *pItem = m_pDataArchivesUI->m_studioInterface->treeWidget()->currentItem();
    if (NULL == pItem)
    {
        return;
    }

    if(m_pDataTable)
    {
        m_pDataTable->setName(text);
    }

    pItem->setText(0, text);
}

void CMDDataSrcTabWidget::on_tableName_editingFinished()
{
    DATASRC_CHECK_NULL_PTR

    m_pDataTable->setName(m_pLineTableName->text());

    m_pTabViewModel->setDataTable(m_pDataTable);
    m_pDataArchives->dataSourceManager()->save();
}

void CMDDataSrcTabWidget::on_tableDescri_editingFinished()
{
    DATASRC_CHECK_NULL_PTR

    m_pDataTable->setDescription(m_pLineDescri->text());

    m_pTabViewModel->setDataTable(m_pDataTable);
    m_pDataArchives->dataSourceManager()->save();
}

void CMDDataSrcTabWidget::on_AddField_clicked()
{
    DATASRC_CHECK_NULL_PTR

    IMDDataField *pField = CMDDataSourceFactory::createDataField();
    CDbTableFieldDialog dlg;
    dlg.initUI(pField, m_pDataArchives->dataGroupManager()->getDataGroup(m_pDataTable->dataGroupName()));
    if(dlg.exec() == QDialog::Accepted)
    {
        if(!m_pDataTable->addDataField(pField))
        {
            delete pField;
            pField = NULL;
        }
        else
        {
            m_pTabViewModel->setDataTable(m_pDataTable);
            m_pDataArchives->dataSourceManager()->save();
        }
    }
    else
    {
        delete pField;
        pField = NULL;
    }
}

void CMDDataSrcTabWidget::on_tabView_doubleClicked(const QModelIndex &/*index*/)
{
    on_editField_clicked();
}

void CMDDataSrcTabWidget::on_removeField_clicked()
{
    DATASRC_CHECK_NULL_PTR

    IMDDataField *pField = NULL;
    QItemSelectionModel *selections = m_pTabView->selectionModel();
    QModelIndexList selected = selections->selectedRows(0);

    QMessageBox message(this);
    message.setWindowTitle(tr("提示"));
    message.addButton(CMDMultiLanguage::instance()->value(tr("确定")), QMessageBox::YesRole);
    message.addButton(CMDMultiLanguage::instance()->value(tr("取消")), QMessageBox::NoRole);
    QString strFront = tr("确定删除 ");
    QString strName = " \'" + QString::number(selected.size()) + "\'";
    QString strBack = tr("个对象吗？");
    QString selectedFieldName = "";
    if(selected.size() == 1){
        int iRow = selected[0].row();
        IMDDataField *pF = m_pDataTable->getDataField(iRow);
        if(pF)
            selectedFieldName = pF->name();
    }
    message.setText(strFront + selectedFieldName + strName + strBack);
    CMDMultiLanguage::instance()->translateWidget(&message);
    message.exec();

    if (QMessageBox::AcceptRole != message.exec())
    {
        return;
    }

    foreach (QModelIndex index, selected)
    {
        int iRows = index.row();
        pField = m_pDataTable->getDataField(iRows);
        if(pField)
        {
            m_pDataTable->removeDataField(pField);
        }
    }

    m_pTabViewModel->setDataTable(m_pDataTable);
}

void CMDDataSrcTabWidget::on_editField_clicked()
{
    DATASRC_CHECK_NULL_PTR

    QModelIndexList selection = m_pTabView->selectionModel()->selectedRows(0);
    if (selection.isEmpty())
    {
        return;
    }

    QModelIndex idIndex = selection.at(0);
    QString name = idIndex.sibling(idIndex.row(), 0).data().toString();
    IMDDataField *pField = m_pDataTable->getDataField(name);
    if(NULL == pField)
    {
        return;
    }

    CDbTableFieldDialog dlg;
    dlg.setEditFlag(1);
    dlg.initUI(pField, m_pDataArchives->dataGroupManager()->getDataGroup(m_pDataTable->dataGroupName()));
    if (QDialog::Accepted == dlg.exec())
    {
        m_pTabViewModel->setDataTable(m_pDataTable);
        m_pDataArchives->dataSourceManager()->save();
    }
}

void CMDDataSrcTabWidget::check_stateChange()
{
    m_iDefaultFlag = !m_iDefaultFlag;
    if (m_iDefaultFlag)
    {
        addDefaultField();
    }
    else
    {
        removeDefaultField();
    }
}
