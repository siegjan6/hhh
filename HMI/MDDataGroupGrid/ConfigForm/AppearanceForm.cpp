#include <QColorDialog>
#include <QFontDialog>
#include "AppearanceForm.h"
#include "ui_AppearanceForm.h"
#include <QDebug>

CAppearanceForm::CAppearanceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAppearanceForm)
{
    ui->setupUi(this);
    m_propertyData = new CDataGroupBasicData;
}

CAppearanceForm::~CAppearanceForm()
{
    delete ui;
    if(m_propertyData)
    {
        delete m_propertyData;
        m_propertyData = NULL;
    }
}

void CAppearanceForm::initialize()
{
    initUI();
    loadFields();
    creatConnection();
}

void CAppearanceForm::loadFields()
{
    if(m_propertyData->m_gridType == GridType::Alarm)
    {
        ui->frontColorComboBox->setVisible(true);
        ui->backColorComboBox->setVisible(true);
        ui->dataColorButton->setVisible(false);
        ui->tabColorButton->setVisible(false);

        for(int i = 0; i < m_fields.count(); i++)
        {
            if(m_fields.at(i).type() == MDDataType::DateTime)
                continue;

            ui->frontColorComboBox->addItem(m_fields.at(i).name());
            ui->backColorComboBox->addItem(m_fields.at(i).name());
        }

        ui->frontColorComboBox->setCurrentText(m_propertyData->m_frontColorField);
        ui->backColorComboBox->setCurrentText(m_propertyData->m_backColorField);
        ui->checkBox->setChecked(m_propertyData->m_isFieldColor);

        m_propertyData->m_allFieldsName.clear();

        for(int i = 0; i < m_fields.count(); i++)
        {
            m_propertyData->m_allFieldsName.append(m_fields.at(i).name());
        }
    }
    else
    {
        ui->checkBox->setVisible(false);
        ui->frontColorComboBox->setVisible(false);
        ui->backColorComboBox->setVisible(false);
        ui->dataColorButton->setVisible(true);
        ui->tabColorButton->setVisible(true);
    }
}

void CAppearanceForm::initUI()
{
    QIntValidator* val = new QIntValidator(1,100,this);
    ui->horizontalBarEdit->setValidator(val);
    ui->verticalBarEdit->setValidator(val);
    ui->rowHeaderWidthEdit->setValidator(val);
    ui->colHeaderWidthEdit->setValidator(val);

    ui->rowHeaderWidthEdit->setText(QString::number (m_propertyData->m_rowWidth));
    ui->colHeaderWidthEdit->setText(QString::number (m_propertyData->m_colHeight));
    ui->horizontalBarEdit->setText(QString::number (m_propertyData->m_horizontalBarHeight));
    ui->verticalBarEdit->setText(QString::number (m_propertyData->m_verticalBarWidth));

    ui->rowFontButton->setText("["+m_propertyData->m_rowFont.family() +","+ QString::number(m_propertyData->m_rowFont.pointSize())+"]");
    ui->colFontButton->setText("["+m_propertyData->m_colFont.family() +","+ QString::number(m_propertyData->m_colFont.pointSize())+"]");
    ui->dataFontButton->setText("["+m_propertyData->m_dataFont.family() +","+ QString::number(m_propertyData->m_dataFont.pointSize())+"]");

    setColor(*ui->rowColorButton,m_propertyData->m_rowColor);
    setColor(*ui->rowForeColorButton,m_propertyData->m_rowForeColor);
    setColor(*ui->colColorButton,m_propertyData->m_colColor);
    setColor(*ui->colForeColorButton,m_propertyData->m_colForeColor);
    setColor(*ui->dataColorButton,m_propertyData->m_dataColor);
    setColor (*ui->tabColorButton,m_propertyData->m_tabColor);
    setColor(*ui->cornerButton,m_propertyData->m_cornerColor);
}

void CAppearanceForm::creatConnection()
{
    connect (ui->checkBox,&QCheckBox::clicked,this,&CAppearanceForm::slotCheckBox);
    connect (ui->frontColorComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotFrontColorComboBox(QString)));
    connect (ui->backColorComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotBackColorComboBox(QString)));
    connect (ui->rowFontButton,&QPushButton::clicked,this,&CAppearanceForm::slotRowFontButton);
    connect (ui->colFontButton,&QPushButton::clicked,this,&CAppearanceForm::slotColFontButton);
    connect (ui->dataFontButton,&QPushButton::clicked,this,&CAppearanceForm::slotDataFontButton);
    connect (ui->rowColorButton,&QPushButton::clicked,this,&CAppearanceForm::slotRowColorButton);
    connect(ui->rowForeColorButton,&QPushButton::clicked,this,&CAppearanceForm::slotRowForeColorButton);
    connect (ui->colColorButton,&QPushButton::clicked,this,&CAppearanceForm::slotColColorButton);
    connect(ui->colForeColorButton,&QPushButton::clicked,this,&CAppearanceForm::slotColForeColorButton);
    connect (ui->dataColorButton,&QPushButton::clicked,this,&CAppearanceForm::slotDataColorButton);
    connect (ui->tabColorButton,&QPushButton::clicked,this,&CAppearanceForm::slotTabColorButton);
    connect(ui->cornerButton,&QPushButton::clicked,this,&CAppearanceForm::slotCornerColorButton);
    connect(ui->rowHeaderWidthEdit,&QLineEdit::editingFinished,this,&CAppearanceForm::slotRowHeaderWidthEdit);
    connect(ui->colHeaderWidthEdit,&QLineEdit::editingFinished,this,&CAppearanceForm::slotColHeaderHeightEdit);
    connect(ui->horizontalBarEdit,&QLineEdit::editingFinished,this,&CAppearanceForm::slotHorizontalBarEdit);
    connect(ui->verticalBarEdit,&QLineEdit::editingFinished,this,&CAppearanceForm::slotVerticalBarEdit);
}

void CAppearanceForm::slotCheckBox()
{
    m_propertyData->m_isFieldColor = ui->checkBox->checkState() == Qt::Checked;
    if(ui->checkBox->checkState() == Qt::Checked )
    {
        ui->frontColorComboBox->setVisible(true);
        ui->backColorComboBox->setVisible(true);
        ui->dataColorButton->setVisible(false);
        ui->tabColorButton->setVisible(false);
    }
    else
    {
        ui->frontColorComboBox->setVisible(false);
        ui->backColorComboBox->setVisible(false);
        ui->dataColorButton->setVisible(true);
        ui->tabColorButton->setVisible(true);
    }
}

void CAppearanceForm::slotFrontColorComboBox(const QString &currentText)
{
    m_propertyData->m_frontColorField = currentText;
}

void CAppearanceForm::slotBackColorComboBox(const QString &currentText)
{
    m_propertyData->m_backColorField = currentText;
}

void CAppearanceForm::slotRowFontButton()
{
    setFont(m_propertyData->m_rowFont);
    ui->rowFontButton->setText("["+m_propertyData->m_rowFont.family() +","+ QString::number(m_propertyData->m_rowFont.pointSize())+"]");
}

void CAppearanceForm::slotColFontButton()
{
    setFont(m_propertyData->m_colFont);
    ui->colFontButton->setText("["+m_propertyData->m_colFont.family() +","+ QString::number(m_propertyData->m_colFont.pointSize())+"]");
}

void CAppearanceForm::slotDataFontButton()
{
    setFont(m_propertyData->m_dataFont);
    ui->dataFontButton->setText("["+m_propertyData->m_dataFont.family() +","+ QString::number(m_propertyData->m_dataFont.pointSize())+"]");
}

void CAppearanceForm::slotRowColorButton()
{
    setButtonColor(*ui->rowColorButton,m_propertyData->m_rowColor);
}

void CAppearanceForm::slotRowForeColorButton()
{
    setButtonColor(*ui->rowForeColorButton,m_propertyData->m_rowForeColor);
}

void CAppearanceForm::slotColColorButton()
{
    setButtonColor(*ui->colColorButton,m_propertyData->m_colColor);
}

void CAppearanceForm::slotColForeColorButton()
{
    setButtonColor(*ui->colForeColorButton,m_propertyData->m_colForeColor);
}

void CAppearanceForm::slotDataColorButton()
{
    setButtonColor(*ui->dataColorButton,m_propertyData->m_dataColor);
}

void CAppearanceForm::slotTabColorButton()
{
    setButtonColor(*ui->tabColorButton,m_propertyData->m_tabColor);
}

void CAppearanceForm::slotCornerColorButton()
{
    setButtonColor(*ui->cornerButton,m_propertyData->m_cornerColor);
}

void CAppearanceForm::slotRowHeaderWidthEdit()
{
    setLineEdit(*ui->rowHeaderWidthEdit,m_propertyData->m_rowWidth);
}

void CAppearanceForm::slotColHeaderHeightEdit()
{
    setLineEdit(*ui->colHeaderWidthEdit,m_propertyData->m_colHeight);
}

void CAppearanceForm::slotHorizontalBarEdit()
{
    setLineEdit(*ui->horizontalBarEdit,m_propertyData->m_horizontalBarHeight);
}

void CAppearanceForm::slotVerticalBarEdit()
{
    setLineEdit(*ui->verticalBarEdit,m_propertyData->m_verticalBarWidth);
}

void CAppearanceForm::setPropertyData(CDataGroupBasicData *value)
{
    *m_propertyData = *value;
}

CDataGroupBasicData *CAppearanceForm::propertyData()
{
    return m_propertyData;
}

void CAppearanceForm::setFields(QList<CDataGroupField> *value)
{
    m_fields = *value;
}

void CAppearanceForm::setFont(QFont & initialFont)
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,initialFont,this);
    if(ok)
    {
        initialFont = font;
    }
}

void  CAppearanceForm::setButtonColor(QPushButton &button,QColor &initialColor)
{
    QColor gColor = QColorDialog::getColor(initialColor,this);
    if(gColor.isValid())
    {
        initialColor = gColor;
        setColor(button,gColor);
    }
}

void CAppearanceForm::setColor(QPushButton &button,QColor &color)
{
    button.setStyleSheet(QString("background-color:rgb(%1,%2,%3,%4);")
                         .arg(color.red())
                         .arg(color.green())
                         .arg(color.blue())
                         .arg(color.alpha()));
}

void CAppearanceForm::setLineEdit(QLineEdit &line,qint32 &value)
{
    bool ok = false;
    value = line.text().toInt(&ok,10);
}
