#include <QPainter>
#include "FillDialog.h"
#include "ui_FillDialog.h"
#include "DynamicColorDialog.h"
#include "MDMultiLanguage.h"

CFillDialog::CFillDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFillDialog)
{
    ui->setupUi(this);

    CMDMultiLanguage::instance()->translateWidget(this);
}
CFillDialog::~CFillDialog()
{
    delete ui;
}
void CFillDialog::on_pushButtonOk_clicked()
{
    saveData();
    accept();
}
void CFillDialog::on_pushButtonCancel_clicked()
{
    reject();
}
void CFillDialog::on_pushButtonDynamicColor_clicked()
{
    CDynamicColorDialog dlg(this);
    dlg.init(m_fill.dynamicBrush());
    if (dlg.exec() == QDialog::Accepted)
        m_fill.setDynamicBrush(dlg.colors());
}
void CFillDialog::on_pushButtonBrush_clicked()
{
    if (m_fill.brush()->showDialog())
        updatePaintButton();
}
//////////////////////////////////////////////////////
void CFillDialog::init(CMDFill *fill)
{
    m_fill = *fill;
    m_fill.setContainer(fill->container());

    ui->doubleSpinBoxMax->setValue(m_fill.max());
    ui->doubleSpinBoxMin->setValue(m_fill.min());
    ui->comboBoxDirection->setCurrentIndex((int)m_fill.direction());
    updatePaintButton();
}
//////////////////////////////////////////////////////
void CFillDialog::saveData()
{
    m_fill.setMax(ui->doubleSpinBoxMax->value());
    m_fill.setMin(ui->doubleSpinBoxMin->value());
    m_fill.setDirection((CMDFill::FillDirection)ui->comboBoxDirection->currentIndex());
}
void CFillDialog::updatePaintButton()
{
    int w = ui->pushButtonBrush->width();
    int h = ui->pushButtonBrush->height();
    ui->pushButtonBrush->m_brush = m_fill.brush()->brush(QRect(0, 0, w, h));
    ui->pushButtonBrush->update();
}
//////////////////////////////////////////////////////
CPaintPushButton::CPaintPushButton(QWidget *parent):QPushButton(parent)
{

}
void CPaintPushButton::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.fillRect(QRect(0, 0, width(), height()), m_brush);
}


