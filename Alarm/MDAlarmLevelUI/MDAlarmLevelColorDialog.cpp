#include "MDAlarmLevelColorDialog.h"
#include "ui_MDAlarmLevelColorDialog.h"
#include <QMessageBox>
#include <QColorDialog>
#include "MDAlarmConfiger.h"
#include "MDStringResource.h"

CMDAlarmLevelColorDialog::CMDAlarmLevelColorDialog(QWidget *parent) :
    CMDObjectPropertyDialog(parent),
    ui(new Ui::CMDAlarmLevelColorDialog)
{
    ui->setupUi(this);

    m_alarmConfiger = NULL;
    m_src = NULL;

    QStringList exs;
    exs << "QPushButton";
    setLanguageExceptions(&exs);
}

CMDAlarmLevelColorDialog::~CMDAlarmLevelColorDialog()
{
    delete ui;
}

void CMDAlarmLevelColorDialog::initial(void* objectUI, void *sourceProperty)
{
    CMDObjectPropertyDialog::initial(objectUI, sourceProperty);

    CMDAlarmLevel* src = (CMDAlarmLevel*)sourceProperty;
    m_src = src;
    m_destFromUI = *src;

    setButtonColor(src->color_occur().f(), ui->occurButtonF);
    setButtonColor(src->color_occur().b(), ui->occurButtonB);

    setButtonColor(src->color_confirm_not_end().f(), ui->confirmNotEndButtonF);
    setButtonColor(src->color_confirm_not_end().b(), ui->confirmNotEndButtonB);

    setButtonColor(src->color_confirm_end().f(), ui->confirmEndButtonF);
    setButtonColor(src->color_confirm_end().b(), ui->confirmEndButtonB);

    setButtonColor(src->color_end_not_confirm().f(), ui->endNotConfirmButtonF);
    setButtonColor(src->color_end_not_confirm().b(), ui->endNotConfirmButtonB);

    setButtonColor(src->color_end_confirm().f(), ui->endConfirmButtonF);
    setButtonColor(src->color_end_confirm().b(), ui->endConfirmButtonB);

    if(m_alarmConfiger)
    {
        bool extend = (MDAlarmMode::extend == m_alarmConfiger->alarmPara()->alarmMode());
        ui->label_3->setVisible(extend);
        ui->label_4->setVisible(extend);
        ui->label_5->setVisible(extend);
        ui->label_6->setVisible(extend);
        ui->label_7->setVisible(extend);
        ui->label_8->setVisible(extend);

        ui->confirmEndButtonF->setVisible(extend);
        ui->confirmEndButtonB->setVisible(extend);

        ui->endConfirmButtonF->setVisible(extend);
        ui->endConfirmButtonB->setVisible(extend);
    }
}

bool CMDAlarmLevelColorDialog::UI2Property(void *destProperty)
{
    CMDAlarmLevel* dest = (CMDAlarmLevel*)destProperty;

    dest->setColor_occur(m_destFromUI.color_occur());
    dest->setColor_confirm_not_end(m_destFromUI.color_confirm_not_end());
    dest->setColor_confirm_end(m_destFromUI.color_confirm_end());
    dest->setColor_end_not_confirm(m_destFromUI.color_end_not_confirm());
    dest->setColor_end_confirm(m_destFromUI.color_end_confirm());

    return true;
}

QColor CMDAlarmLevelColorDialog::selectColor(QColor &initialColor, QPushButton* button)
{
    QColor color = QColorDialog::getColor(initialColor,this, STRANS(STRING_SELECT_COLOR));
    if(!color.isValid())
        return initialColor;

    setButtonColor(color, button);
    return color;
}

void CMDAlarmLevelColorDialog::setButtonColor(QColor &color, QPushButton* button)
{
    button->setStyleSheet(QString("border: none; background-color: rgb(%1, %2, %3);")
                         .arg(color.red())
                         .arg(color.green())
                         .arg(color.blue()));
}

QRgb CMDAlarmLevelColorDialog::selectColor(QRgb initialColor, QPushButton* button)
{
    QColor c(initialColor);
    QColor s = selectColor(c, button);
    return s.rgb();
}

void CMDAlarmLevelColorDialog::setButtonColor(QRgb Color, QPushButton* button)
{
    QColor c(Color);
    setButtonColor(c, button);
}

void CMDAlarmLevelColorDialog::on_occurButtonF_clicked()
{
    QRgb c = selectColor(m_src->color_occur().f(), ui->occurButtonF);
    m_destFromUI.color_occur().setF(c);
}
void CMDAlarmLevelColorDialog::on_occurButtonB_clicked()
{
    QRgb c = selectColor(m_src->color_occur().b(), ui->occurButtonB);
    m_destFromUI.color_occur().setB(c);
}

void CMDAlarmLevelColorDialog::on_confirmNotEndButtonF_clicked()
{
    QRgb c = selectColor(m_src->color_confirm_not_end().f(), ui->confirmNotEndButtonF);
    m_destFromUI.color_confirm_not_end().setF(c);
}
void CMDAlarmLevelColorDialog::on_confirmNotEndButtonB_clicked()
{
    QRgb c = selectColor(m_src->color_confirm_not_end().b(), ui->confirmNotEndButtonB);
    m_destFromUI.color_confirm_not_end().setB(c);
}

void CMDAlarmLevelColorDialog::on_confirmEndButtonF_clicked()
{
    QRgb c = selectColor(m_src->color_confirm_end().f(), ui->confirmEndButtonF);
    m_destFromUI.color_confirm_end().setF(c);
}
void CMDAlarmLevelColorDialog::on_confirmEndButtonB_clicked()
{
    QRgb c = selectColor(m_src->color_confirm_end().b(), ui->confirmEndButtonB);
    m_destFromUI.color_confirm_end().setB(c);
}

void CMDAlarmLevelColorDialog::on_endNotConfirmButtonF_clicked()
{
    QRgb c = selectColor(m_src->color_end_not_confirm().f(), ui->endNotConfirmButtonF);
    m_destFromUI.color_end_not_confirm().setF(c);
}
void CMDAlarmLevelColorDialog::on_endNotConfirmButtonB_clicked()
{
    QRgb c = selectColor(m_src->color_end_not_confirm().b(), ui->endNotConfirmButtonB);
    m_destFromUI.color_end_not_confirm().setB(c);
}

void CMDAlarmLevelColorDialog::on_endConfirmButtonF_clicked()
{
    QRgb c = selectColor(m_src->color_end_confirm().f(), ui->endConfirmButtonF);
    m_destFromUI.color_end_confirm().setF(c);
}
void CMDAlarmLevelColorDialog::on_endConfirmButtonB_clicked()
{
    QRgb c = selectColor(m_src->color_end_confirm().b(), ui->endConfirmButtonB);
    m_destFromUI.color_end_confirm().setB(c);
}
