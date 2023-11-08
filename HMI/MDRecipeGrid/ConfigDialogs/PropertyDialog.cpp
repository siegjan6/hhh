#include "PropertyDialog.h"
#include "ui_PropertyDialog.h"
#include "ShareTool.h"

CPropertyDialog::CPropertyDialog(const PropertyPara &property, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPropertyDialog)
{
    ui->setupUi(this);

    m_property = property;
    initWidgets();

    CShareTool::installDialogTranslator(this);
}

CPropertyDialog::~CPropertyDialog()
{
    delete ui;
}

void CPropertyDialog::initWidgets()
{
    ui->checkboxShowGrid->setChecked(m_property.showGrid);
    ui->checkboxHighLight->setChecked(m_property.highLight);
    ui->checkboxLanguage->setChecked(m_property.multiLanguage);
    ui->checkboxKeyboard->setChecked(m_property.softKeyboard);
    ui->checkboxClickEdit->setChecked(m_property.clickEdit);

    QIntValidator* validator = new QIntValidator(1,1000,this);
    ui->lineeditHorHeight->setText(QString::number(m_property.horizontalHeaderHeight));
    ui->lineeditHorHeight->setValidator(validator);
    ui->lineeditVerWidth->setText(QString::number(m_property.verticalHeaderWidth));
    ui->lineeditVerWidth->setValidator(validator);
}

void CPropertyDialog::accept()
{
    m_property.showGrid = ui->checkboxShowGrid->isChecked();
    m_property.highLight = ui->checkboxHighLight->isChecked();
    m_property.multiLanguage = ui->checkboxLanguage->isChecked();
    m_property.softKeyboard = ui->checkboxKeyboard->isChecked();
    m_property.clickEdit = ui->checkboxClickEdit->isChecked();

    qint32 intValue;
    bool ok = false;
    if(!ui->lineeditHorHeight->text().isEmpty())
    {
        intValue = ui->lineeditHorHeight->text().toInt(&ok);
        if(ok)
            m_property.horizontalHeaderHeight = intValue;
    }
    if(!ui->lineeditVerWidth->text().isEmpty())
    {
        intValue = ui->lineeditVerWidth->text().toInt(&ok);
        if(ok)
            m_property.verticalHeaderWidth = intValue;
    }

    QDialog::accept();
}



