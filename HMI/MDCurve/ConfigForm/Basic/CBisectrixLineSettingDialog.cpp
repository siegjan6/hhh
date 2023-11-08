#include "CBisectrixLineSettingDialog.h"
#include "ui_CBisectrixLineSettingDialog.h"

#include "CBisectrixLine.h"
#include <QPainter>
#include <QPixmap>
#include <QIcon>
#include <QColorDialog>
#include <QIntValidator>

#include <QDebug>
#include "CTranslator.h"
#include "CFormToolFunc.h"

using namespace Translator;
using namespace FormToolFunc;

CBisectrixLineSettingDialog::CBisectrixLineSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBisectrixLineSettingDialog),
    m_line(0),
    m_lineWidth(1),
    m_lineStyle(1)
{
    ui->setupUi(this);

    installDialogTranslator(m_transTool,this,QStringList("QLineEdit"));
}

CBisectrixLineSettingDialog::~CBisectrixLineSettingDialog()
{
    delete ui;
}

/*!
\section
*/
CBisectrixLine *CBisectrixLineSettingDialog::getLine(bool *ok, CBisectrixLine *initial, QWidget *parent)
{
    if(initial)
    {
        CBisectrixLineSettingDialog dlg(parent);
        dlg.setData(initial);
        int ret = dlg.exec();
        if (ok)
        {
            *ok = !!ret;
            return initial;
        }
    }
    return NULL;
}

void CBisectrixLineSettingDialog::setData(CBisectrixLine *line)
{
    if(line)
    {
        m_line = line;
        initialize();
    }
}

/*!
\section
*/
void CBisectrixLineSettingDialog::initialize()
{
    initializeData();
    initializeUI();
}

void CBisectrixLineSettingDialog::initializeData()
{
    CBisectrixLine *line = m_line;
    if(line)
    {
        m_lineWidth = line->lineWidth();
        m_lineColor = line->lineColor();
        m_lineStyle = line->lineStyle();
    }
}

void CBisectrixLineSettingDialog::initializeUI()
{
    setLineStyles();

    ui->widthSpinBox->setValue(m_lineWidth);
    setButtonColor(m_lineColor,*(ui->colorButton));
    ui->styleComboBox->setCurrentIndex(m_lineStyle-1);


    createConnections();
}

void CBisectrixLineSettingDialog::initializeUIData()
{
    ui->styleComboBox->setCurrentIndex(m_lineStyle);
    ui->widthSpinBox->setValue(m_lineWidth);
}

/*!
\section
*/
void CBisectrixLineSettingDialog::applyChanges()
{
    if(m_line)
    {
        m_line->setLineColor(m_lineColor);
        m_line->setLineWidth(m_lineWidth);
        m_line->setLineStyle((Qt::PenStyle)m_lineStyle);
    }
}


void CBisectrixLineSettingDialog::setLineStyles()
{
    ui->styleComboBox->setToolTip(tr("Line style"));
    ui->styleComboBox->setEditable(false);
    ui->styleComboBox->setIconSize(QSize(105,14));
    ui->styleComboBox->setMinimumWidth(105);

    for (int i = (int)Qt::SolidLine; i < (int)Qt::CustomDashLine; i++)
    {
        QPixmap pix(105,14);
        pix.fill(Qt::white);

        QBrush brush(Qt::black);
        QPen pen(brush,2.5,(Qt::PenStyle)i);

        QPainter painter(&pix);
        painter.setPen(pen);
        painter.drawLine(2,7,103,7);

        ui->styleComboBox->addItem(QIcon(pix),"");
    }
}

void CBisectrixLineSettingDialog::createConnections()
{
    connect(ui->okButton,&QPushButton::clicked,this,&CBisectrixLineSettingDialog::slotAccept);
    connect(ui->cancelButton,&QPushButton::clicked,this,&CBisectrixLineSettingDialog::slotCancel);
    connect(ui->colorButton,&QPushButton::clicked,this,&CBisectrixLineSettingDialog::slotSetLineColor);
    connect(ui->widthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotSetLineWidth(int)));
    connect(ui->styleComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotLineStyleChanges(int)));
}

void CBisectrixLineSettingDialog::slotLineStyleChanges(int index)
{
    slotSetLineStyle(index);
}

void CBisectrixLineSettingDialog::slotSetLineColor()
{
//    QColor color = QColorDialog::getColor(m_lineColor,this);
//    if(!color.isValid())
//        return;
//    ui->colorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
//                                   .arg(color.red())
//                                   .arg(color.green())
//                                   .arg(color.blue()));
//    m_lineColor = color;

//    QColorDialog dialog(this);
//    dialog.setCurrentColor(m_lineColor);
//    installTranslatorWidget(&dialog);
//    int ret = dialog.exec();
//    if(ret == QDialog::Accepted){
//        QColor color = dialog.selectedColor();
//        if(color.isValid()){
//            m_lineColor = color;

//            ui->colorButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
//                                           .arg(color.red())
//                                           .arg(color.green())
//                                           .arg(color.blue()));
//        }
//    }else{
//    }

    selectColor(m_lineColor,*(ui->colorButton),this);
}


void CBisectrixLineSettingDialog::slotSetLineWidth(int width)
{
    m_lineWidth = width;
}

void CBisectrixLineSettingDialog::slotSetLineStyle(int index)
{
    m_lineStyle = index+1;
}

void CBisectrixLineSettingDialog::slotAccept()
{
    applyChanges();
    accept();
}

void CBisectrixLineSettingDialog::slotCancel()
{
    reject();
}
