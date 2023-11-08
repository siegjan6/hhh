#include "MDFilterConditionSetting.h"
#include "MDFilterTreePlugin.h"

#include "IMDDataField.h"
#include "MDMultiLanguage.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QRadioButton>

CMDFilterConditionSetting::CMDFilterConditionSetting(QWidget *parent) :
    QDialog(parent)
{
    setFixedSize(320, 200);
    initData();

    m_operatorList.clear();
    m_operatorList << ">" << "<" << "=" << ">=" << "<=" << "!=" << "LIKE" << "NOT LIKE";

    setWindowTitle(tr("设置过滤条件"));
    createUI();

    m_pMulLnaguageTool = CMDMultiLanguage::createWidgetTool();
    m_pMulLnaguageTool->init(this);
    m_pMulLnaguageTool->changeLanguage();

    connect(m_pCombCondi, SIGNAL(currentIndexChanged(QString)), this, SLOT(conditionComboBoxIndexChange(QString)));
    connect(m_pBtnOk, SIGNAL(clicked()), this, SLOT(okBtnClicked()));
    connect(m_pBtnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_pRdoStatic, SIGNAL(toggled(bool)), this, SLOT(staticRadioBtnClicked(bool)));
    connect(m_pRdoDynamic, SIGNAL(toggled(bool)), this, SLOT(dynamicRadioBtnClicked(bool)));
}

CMDFilterConditionSetting::~CMDFilterConditionSetting()
{
    if (m_pMulLnaguageTool)
    {
        delete m_pMulLnaguageTool;
        m_pMulLnaguageTool = NULL;
    }
}

void CMDFilterConditionSetting::changeCurrentOperatorIndex(const QString &oper)
{
//    m_pCombCondi->setCurrentIndex(m_pCombCondi->findText(oper));
    for (int i = 0; i < m_operatorList.size(); ++i)
    {
        if (oper == m_operatorList.at(i))
        {
            m_pCombCondi->setCurrentIndex(i);
            return;
        }
    }

    m_pCombCondi->setCurrentIndex(0);
}

void CMDFilterConditionSetting::createUI()
{
   m_pHFirstLayout    = new QHBoxLayout;
   m_pHSecondLayout   = new QHBoxLayout;
   m_pHThirdLayout    = new QHBoxLayout;
   m_pHFourthLayout   = new QHBoxLayout;
   m_pHFifthLayout    = new QHBoxLayout;
   m_pVMainLayout     = new QVBoxLayout;

    m_pLbCondition    = new QLabel(tr("条件:") , this);
    m_pLbValue        = new QLabel(tr("比较值:") , this);
    m_pLbHint         = new QLabel(tr("提示:") , this);

    m_pLbCondition->setFixedWidth(FILTER_LINEEDIT_W);
    m_pLbValue->setFixedWidth(FILTER_LINEEDIT_W);
    m_pLbHint->setFixedWidth(FILTER_LINEEDIT_W);

    m_pLbValue->setAlignment(Qt::AlignRight);
    m_pLbCondition->setAlignment(Qt::AlignRight);
    m_pLbHint->setAlignment(Qt::AlignRight);

    m_pRdoStatic      = new QRadioButton(tr("静态过滤"), this);
    m_pRdoStatic->setChecked(true);
    m_pRdoDynamic     = new QRadioButton(tr("动态过滤"), this);

    m_pLineValue      = new QLineEdit(this);
    m_pLineValue->setFixedWidth(FILTER_SETTING_W);

    m_pLineHint       = new QLineEdit(this);
    m_pLineHint->setFixedWidth(FILTER_SETTING_W);

    m_pCombCondi      = new QComboBox(this);

    m_pCombCondi->setFixedWidth(FILTER_SETTING_W);
    m_pCombCondi->addItems(m_operatorList);
//    m_pCombCondi->setCurrentIndex(0);
//    m_pCombCondi->setFixedWidth(FILTER_SETTING_W);

    m_pDateEdit   = new QDateTimeEdit(this);
    m_pDateEdit->setFixedWidth(FILTER_SETTING_W);
    m_pDateEdit->setMinimumDate(QDate(1900, 1, 1));
    m_pDateEdit->setMaximumDate(QDate(2100, 1, 1));
    m_pDateEdit->setDisplayFormat("yyyy-MM-dd");
    m_pDateEdit->setCalendarPopup(true);

    m_pTimeEdit   = new QDateTimeEdit(this);
    m_pTimeEdit->setFixedWidth(FILTER_SETTING_W);
    m_pTimeEdit->setDisplayFormat("hh:mm:ss");

    m_pTimeEdit->setVisible(false);
    m_pDateEdit->setVisible(false);

    m_pLbHint->setVisible(false);
    m_pLineHint->setVisible(false);

    m_pBtnOk          = new QPushButton(tr("确定"), this);
    m_pBtnCancel      = new QPushButton(tr("取消"), this);

//    int maxWidth = calculateLabelWidth(m_pLbValue);

    m_pHFirstLayout->addWidget(m_pRdoStatic);
    m_pHFirstLayout->addWidget(m_pRdoDynamic);

    m_pHSecondLayout->addWidget(m_pLbCondition);
    m_pHSecondLayout->setSpacing(FILTER_SETTING_SPACE);
    m_pHSecondLayout->addWidget(m_pCombCondi);
    m_pHSecondLayout->addStretch(0);

    m_pHThirdLayout->addWidget(m_pLbValue);
    m_pHThirdLayout->setSpacing(FILTER_SETTING_SPACE);
    m_pHThirdLayout->addWidget(m_pLineValue);
    m_pHThirdLayout->addWidget(m_pDateEdit);
    m_pHThirdLayout->addWidget(m_pTimeEdit);
    m_pHThirdLayout->addStretch(0);

    m_pHFourthLayout->addWidget(m_pLbHint);
    m_pHFourthLayout->setSpacing(FILTER_SETTING_SPACE);
    m_pHFourthLayout->addWidget(m_pLineHint);
    m_pHFourthLayout->addStretch(0);

    m_pHFifthLayout->addStretch(0);
    m_pHFifthLayout->addWidget(m_pBtnOk);
    m_pHFifthLayout->setSpacing(FILTER_SETTING_SPACE);
    m_pHFifthLayout->addWidget(m_pBtnCancel);

    m_pVMainLayout->addLayout(m_pHFirstLayout);
    m_pVMainLayout->addLayout(m_pHSecondLayout);
    m_pVMainLayout->addLayout(m_pHThirdLayout);
    m_pVMainLayout->addLayout(m_pHFourthLayout);
    m_pVMainLayout->addLayout(m_pHFifthLayout);

    setLayout(m_pVMainLayout);
}

int CMDFilterConditionSetting::calculateLabelWidth(QLabel *pLb)
{
    QFontMetrics met(pLb->font());
    return met.width(pLb->text());
}

void CMDFilterConditionSetting::initData()
{
    m_pLbCondition     = NULL; //! 条件标签
    m_pLbValue         = NULL; //! 值标签
    m_pLbHint          = NULL; //! 提示标签
    m_pCombCondi       = NULL; //! 条件关系下拉框
    m_pLineValue       = NULL; //! 值编辑框
    m_pLineHint        = NULL; //! 提示编辑框

    m_pBtnOk           = NULL; //! 确定按钮
    m_pBtnCancel       = NULL; //! 取消按钮

    m_pDateEdit        = NULL; //! 日期编辑界面
    m_pTimeEdit        = NULL; //! 时间编辑界面

    m_pRdoStatic       = NULL; //! 静态选择
    m_pRdoDynamic      = NULL; //! 动态选择

    m_pHFirstLayout    = NULL; //! 水平布局
    m_pHSecondLayout   = NULL; //! 水平布局
    m_pHThirdLayout    = NULL; //! 水平布局
    m_pHFourthLayout   = NULL; //! 水平布局
    m_pHFifthLayout   = NULL; //! 水平布局
    m_pVMainLayout     = NULL; //! 主垂直布局

    m_operatorList.clear();             //! 条件关系列表
    m_pFilterItem = NULL; //! 当前的属性信息
}

void CMDFilterConditionSetting::setDateTimeEditVisible(bool iFlag)
{
    m_pDateEdit->setVisible(iFlag);
    m_pTimeEdit->setVisible(iFlag);
    m_pLineValue->setVisible(!iFlag);
}

void CMDFilterConditionSetting::setPropertyInfo(CMDFilterTreeDataItem *pFilterItem)
{
    m_pFilterItem = pFilterItem;
    if (NULL == m_pFilterItem)
    {
        return;
    }

    QString strTitle = CMDMultiLanguage::instance()->value(m_pFilterItem->getDescripe());
    if (strTitle.isEmpty())
    {
        strTitle = CMDMultiLanguage::instance()->value(m_pFilterItem->getName());
    }

    setWindowTitle(strTitle);
    changeCurrentOperatorIndex(m_pFilterItem->getOperator());

    CMDFilterTreeDataItem::CFilterType type = m_pFilterItem->getType();
    if (CMDFilterTreeDataItem::Filter_Dynamic == type)
    {
        dynamicRadioBtnClicked(true);
        m_pRdoDynamic->setChecked(true);
        m_pLineHint->setText(m_pFilterItem->getHintInfo());
    }
    else if (CMDFilterTreeDataItem::Filter_Enum == type)
    {

    }
    else
    {
        m_pRdoStatic->setChecked(true);
        QString strTmp = m_pFilterItem->getValue();
        strTmp = strTmp.remove("\'");
        QVariant::Type dataType = (QVariant::Type)m_pFilterItem->getDataType();
        QDateTime dateTime;
        switch (dataType)
        {
        case QVariant::DateTime:
            setFixedSize(400, 200);
            dateTime = QDateTime::fromString(strTmp, "yyyy-MM-dd hh:mm:ss");
            setDateTimeEditVisible(true);
            m_pDateEdit->setDate(dateTime.date());
            m_pTimeEdit->setTime(dateTime.time());
            break;
        default:
            setFixedSize(320, 200);
            setDateTimeEditVisible(false);
            break;
        }

        m_pLineValue->setText(strTmp);
    }
}

void CMDFilterConditionSetting::changeLanguage()
{
    m_pMulLnaguageTool->changeLanguage();
}

void CMDFilterConditionSetting::conditionComboBoxIndexChange(const QString &/*text*/)
{

}

void CMDFilterConditionSetting::okBtnClicked()
{
    if (NULL == m_pFilterItem)
    {
        return;
    }

    m_pFilterItem->setOperator(m_pCombCondi->currentText());
    if (m_pRdoStatic->isChecked())
    {
        QString strValue = "";
        if (m_pDateEdit->isVisible())
        {
            strValue = "\'" + m_pDateEdit->date().toString("yyyy-MM-dd") \
                    + " " + m_pTimeEdit->time().toString("hh:mm:ss") + "\'";
        }
        else
        {
            strValue = m_pLineValue->text();
            QVariant::Type type = (QVariant::Type)m_pFilterItem->getDataType();
            switch (type)
            {
            case QVariant::String:
                strValue = "\'" + m_pLineValue->text() + "\'";
                break;
            default:
                break;
            }
        }

        m_pFilterItem->setHintInfo("");
        m_pFilterItem->setValue(strValue);
        m_pFilterItem->setType(CMDFilterTreeDataItem::Filter_Static);
    }
    else
    {
        m_pFilterItem->setType(CMDFilterTreeDataItem::Filter_Dynamic);
        m_pFilterItem->setHintInfo(m_pLineHint->text());
        m_pFilterItem->setValue("[value]");
    }

    accept();
}

void CMDFilterConditionSetting::staticRadioBtnClicked(bool checked)
{
    if (checked && QVariant::DateTime == (QVariant::Type)m_pFilterItem->getDataType()
            && !m_pDateEdit->isVisible())
    {
        setFixedSize(400, 200);
        m_pDateEdit->setVisible(checked);
        m_pTimeEdit->setVisible(checked);
        m_pLineValue->setVisible(!checked);
    }
    else
    {
        m_pLineValue->setVisible(checked);
    }

    m_pLbValue->setVisible(checked);
    m_pLbHint->setVisible(!checked);
    m_pLineHint->setVisible(!checked);
}

void CMDFilterConditionSetting::dynamicRadioBtnClicked(bool checked)
{
    if (checked && QVariant::DateTime == (QVariant::Type)m_pFilterItem->getDataType()
            && m_pDateEdit->isVisible())
    {
        setFixedSize(320, 200);
        m_pDateEdit->setVisible(!checked);
        m_pTimeEdit->setVisible(!checked);
    }

    m_pLbHint->setVisible(checked);
    m_pLineHint->setVisible(checked);
    m_pLineValue->setVisible(!checked);
    m_pLbValue->setVisible(!checked);
}
