#include "CDrawModeSettingForm.h"
#include "ui_CDrawModeSettingForm.h"

#include "CDrawMode.h"

CDrawModeSettingForm::CDrawModeSettingForm(QWidget *parent) :
    QWidget(parent),
    m_drawMode(0),
    m_isModified(false),
    ui(new Ui::CDrawModeSettingForm)
{
    ui->setupUi(this);
}

CDrawModeSettingForm::~CDrawModeSettingForm()
{
    delete ui;
}

void CDrawModeSettingForm::setData(CDrawMode *drawMode)
{
    if(drawMode)
    {
        m_drawMode = drawMode;
        initialize();
    }
}

void CDrawModeSettingForm::applyChanges()
{
    if(m_drawMode)
    {
        if(ui->leftToRightRadioButton->isChecked())
            m_drawMode->setOrietation(CDrawMode::Orientation::LeftToRight);
        else if(ui->rightToLeftRadioButton->isChecked())
            m_drawMode->setOrietation(CDrawMode::Orientation::RigthToLeft);

        m_drawMode->setFreshTime(ui->freshTimeSpinBox->value());
    }
}
/*!
\section
*/
void CDrawModeSettingForm::resetModified()
{
    m_isModified = false;
}

void CDrawModeSettingForm::slotHandleModified()
{
    if(!m_isModified){
        m_isModified = true;
        emit itemModified();
    }
}

/*!
\section
*/
void CDrawModeSettingForm::createConnections()
{
    connect(ui->leftToRightRadioButton,&QRadioButton::clicked,this,&CDrawModeSettingForm::slotHandleModified);
    connect(ui->rightToLeftRadioButton,&QRadioButton::clicked,this,&CDrawModeSettingForm::slotHandleModified);

    connect(ui->freshTimeSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotHandleModified()));
}

void CDrawModeSettingForm::breakConnections()
{
    disconnect(ui->leftToRightRadioButton,&QRadioButton::clicked,this,&CDrawModeSettingForm::slotHandleModified);
    disconnect(ui->rightToLeftRadioButton,&QRadioButton::clicked,this,&CDrawModeSettingForm::slotHandleModified);

    disconnect(ui->freshTimeSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotHandleModified()));
}
/*!
\section
*/
void CDrawModeSettingForm::initialize()
{
    breakConnections();
    initializeUI();
    createConnections();
}

void CDrawModeSettingForm::initializeUI()
{
    if(m_drawMode)  {
        switch (m_drawMode->orietation()) {
        case CDrawMode::Orientation::LeftToRight:
            ui->leftToRightRadioButton->setChecked(true);
            break;
        case CDrawMode::Orientation::RigthToLeft:
            ui->rightToLeftRadioButton->setChecked(true);
            break;
        default:
            break;
        }

        ui->freshTimeSpinBox->setValue(m_drawMode->freshTime());
    }
}
