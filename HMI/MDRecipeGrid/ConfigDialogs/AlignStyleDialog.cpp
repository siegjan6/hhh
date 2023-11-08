#include "AlignStyleDialog.h"
#include "ui_AlignStyleDialog.h"
#include <QPainter>
#include <QIcon>
#include <QDebug>
#include "ShareTool.h"

CAlignStyleDialog::CAlignStyleDialog(Qt::Alignment align, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAlignStyleDialog)
{
    ui->setupUi(this);
    m_buttonManager = new QButtonGroup(this);

    m_buttonManager->addButton(ui->upLeftButton,          Qt::AlignLeft);
    m_buttonManager->addButton(ui->upMiddleButton,        Qt::AlignHCenter);
    m_buttonManager->addButton(ui->upRightButton,         Qt::AlignRight);
    m_buttonManager->addButton(ui->middleLeftButton,      Qt::AlignVCenter);
    m_buttonManager->addButton(ui->MMButton,              Qt::AlignCenter);
    m_buttonManager->addButton(ui->middleRightButton,     Qt::AlignVCenter|Qt::AlignRight);
    m_buttonManager->addButton(ui->bottomLeftButton,      Qt::AlignBottom);
    m_buttonManager->addButton(ui->bottomMiddleButton,    Qt::AlignBottom|Qt::AlignHCenter);
    m_buttonManager->addButton(ui->bottomRightButton,     Qt::AlignBottom|Qt::AlignRight);

    QObject::connect(m_buttonManager, static_cast<void (QButtonGroup:: *)(int,bool)>(&QButtonGroup::buttonToggled),
                     this, &CAlignStyleDialog::buttonCheckedHandler);

    QAbstractButton* button = m_buttonManager->button(align);
    if( button )
        button->setChecked(true);
    else
        ui->middleLeftButton->setChecked(true);

    CShareTool::installDialogTranslator(this);
}

CAlignStyleDialog::~CAlignStyleDialog()
{
    delete ui;
}

void CAlignStyleDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.save();

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255,255,255));

    QRect backgroundRect(QPoint(ui->upMiddleButton->geometry().topLeft().x(),ui->bottomLeftButton->geometry().topLeft().y()),
                        QPoint(ui->middleRightButton->geometry().bottomRight().x(),ui->bottomRightButton->geometry().bottomRight().y()) );

    painter.drawRect(backgroundRect);
    painter.setPen(1);
    QRect rect = backgroundRect;
    rect.adjust(10,10,-10,-10);
    painter.drawRect(rect);
    painter.setPen(Qt::DashLine);
    painter.drawLine(rect.x(),rect.y()+rect.height()/2,rect.x()+rect.width(),rect.y()+rect.height()/2);
    painter.drawLine(rect.x()+rect.width()/2,rect.y(),rect.x()+rect.width()/2,rect.y()+rect.height());

    QString strSample = "AaBbCc";
    QPen pen(Qt::SolidLine);
    pen.setWidth(3);
    painter.setPen(pen);
    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    painter.setFont(font);
    painter.drawText(rect, m_align, strSample);

    painter.restore();
}

void CAlignStyleDialog::on_confirmButton_clicked()
{
   accept();
}

void CAlignStyleDialog::on_cancelButton_clicked()
{
    reject();
}

void CAlignStyleDialog::buttonCheckedHandler(int id, bool /*checked*/)
{
       m_align = (Qt::Alignment)id;
       update();
}

