#include "PreviewPropertyWidget.h"
#include "ui_PreviewPropertyWidget.h"
//////////////////////////////////////////////////////////////////////////
CPreviewPropertyWidget::CPreviewPropertyWidget(CFormObjectContainer *container, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPropertyWidget)
{
    ui->setupUi(this);

    m_container = container;

    init();
}
CPreviewPropertyWidget::~CPreviewPropertyWidget()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CPreviewPropertyWidget::rectChanged()
{
    CFormObject* currentObj = m_container->currentObj();
    bool isNULL = (currentObj == NULL);

    ui->lineEditLeft->setEnabled(!isNULL);
    ui->lineEditTop->setEnabled(!isNULL);
    ui->lineEditWidth->setEnabled(!isNULL);
    ui->lineEditHeight->setEnabled(!isNULL);

    if(!isNULL)
    {
        QRect r = currentObj->rect();
        ui->lineEditLeft->setText(QString::number(r.left()));
        ui->lineEditTop->setText(QString::number(r.top()));
        ui->lineEditWidth->setText(QString::number(r.width()));
        ui->lineEditHeight->setText(QString::number(r.height()));
    }
}
void CPreviewPropertyWidget::formEditingFinished()
{
    if(m_container->currentObj() == NULL)
        return;

    int l = ui->lineEditLeft->text().toInt();
    int t = ui->lineEditTop->text().toInt();
    int w = ui->lineEditWidth->text().toInt();
    int h = ui->lineEditHeight->text().toInt();

    m_container->currentObj()->setRect(QRect(l, t, w, h));
    m_container->update();
}
void CPreviewPropertyWidget::frameEditingFinished()
{
    int w = ui->lineEditFrameWidth->text().toInt();
    int h = ui->lineEditFrameHeight->text().toInt();

    m_container->setFrameSize(QSize(w, h));
}
//////////////////////////////////////////////////////////////////////////
void CPreviewPropertyWidget::init()
{
    QValidator* v1 = new QIntValidator(-9999, 9999, this);
    QValidator* v2 = new QIntValidator(5, 9999, this);

    ui->lineEditFrameWidth->setValidator(v2);
    ui->lineEditFrameHeight->setValidator(v2);
    ui->lineEditLeft->setValidator(v1);
    ui->lineEditTop->setValidator(v1);
    ui->lineEditWidth->setValidator(v2);
    ui->lineEditHeight->setValidator(v2);

    connect(m_container, &CFormObjectContainer::rectChanged,
            this, &CPreviewPropertyWidget::rectChanged);

    rectChanged();
    ui->checkBoxAutoJoin->setChecked(m_container->autoJoin());
    ui->checkBoxTransparent->setChecked(m_container->isTransparent());
    ui->lineEditFrameWidth->setText(QString::number(m_container->frameSize().width()));
    ui->lineEditFrameHeight->setText(QString::number(m_container->frameSize().height()));
}
//////////////////////////////////////////////////////////////////////////
void CPreviewPropertyWidget::on_checkBoxTransparent_clicked()
{
    m_container->setIsTransparent(ui->checkBoxTransparent->checkState() == Qt::Checked);
    m_container->update();
}
void CPreviewPropertyWidget::on_checkBoxAutoJoin_clicked()
{
    m_container->setAutoJoin(ui->checkBoxAutoJoin->checkState() == Qt::Checked);
}
void CPreviewPropertyWidget::on_lineEditFrameWidth_editingFinished()
{
    frameEditingFinished();
}
void CPreviewPropertyWidget::on_lineEditFrameHeight_editingFinished()
{
    frameEditingFinished();
}
void CPreviewPropertyWidget::on_lineEditLeft_editingFinished()
{
    formEditingFinished();
}
void CPreviewPropertyWidget::on_lineEditTop_editingFinished()
{
    formEditingFinished();
}
void CPreviewPropertyWidget::on_lineEditWidth_editingFinished()
{
    formEditingFinished();
}
void CPreviewPropertyWidget::on_lineEditHeight_editingFinished()
{
    formEditingFinished();
}
//////////////////////////////////////////////////////////////////////////
QLineEdit *CPreviewPropertyWidget::lineEditLeft()
{
    return ui->lineEditLeft;
}
QLineEdit *CPreviewPropertyWidget::lineEditTop()
{
    return ui->lineEditTop;
}
QLineEdit *CPreviewPropertyWidget::lineEditWidth()
{
    return ui->lineEditWidth;
}
QLineEdit *CPreviewPropertyWidget::lineEditHeight()
{
    return ui->lineEditHeight;
}
//////////////////////////////////////////////////////////////////////////
