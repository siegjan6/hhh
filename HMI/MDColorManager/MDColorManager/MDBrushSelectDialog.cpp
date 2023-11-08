#include "MDBrushSelectDialog.h"
#include "MDBrush_Widget.h"
#include "MDMultiLanguage.h"

CMDBrushSelectDialog::CMDBrushSelectDialog(QWidget *parent)
{
    p_widgetBrush = NULL;
    p_btn_ok = NULL;
    p_btn_cancel = NULL;
    p_vLayoutMain = NULL;
    p_hLayoutBtn = NULL;
    initialize();
    setUserDefineStyle();
    setWinLayout();
    sigAndSlotConnect();
    setTextLabelInfo();

    CMDMultiLanguage::instance()->translateWidget(this);
}

CMDBrushSelectDialog::~CMDBrushSelectDialog()
{
    release();
}

CMDBrushManager CMDBrushSelectDialog::brushManager()
{
    return p_widgetBrush->brushManager();
}

void CMDBrushSelectDialog::setBrushManager(CMDBrushManager brushMgr)
{
    p_widgetBrush->setBrushManager(brushMgr);
}

void CMDBrushSelectDialog::slotOkClicked()
{
    QDialog::accept();
}

void CMDBrushSelectDialog::slotCancelClicked()
{
    QDialog::rejected();
    close();
}

void CMDBrushSelectDialog::initialize()
{
    p_widgetBrush = new CMDBrush_Widget();
    p_btn_ok = new QPushButton();
    p_btn_cancel = new QPushButton();
    p_vLayoutMain = new QVBoxLayout();
    p_hLayoutBtn = new QHBoxLayout();
}

void CMDBrushSelectDialog::release()
{
    delete p_widgetBrush;
    delete p_btn_ok;
    delete p_btn_cancel;
    delete p_vLayoutMain;
   // delete p_hLayoutBtn;
}

void CMDBrushSelectDialog::setUserDefineStyle()
{
    const int btn_width = 90,btn_height = 25;

    p_btn_ok->setFixedSize(btn_width,btn_height);
    p_btn_cancel->setFixedSize(btn_width,btn_height);

 //   p_widgetBrush->setSizeIncrement(831,290);
    this->setFixedSize(866,400);
}

void CMDBrushSelectDialog::setWinLayout()
{
    p_vLayoutMain->addWidget(p_widgetBrush);

    p_hLayoutBtn->setAlignment(Qt::AlignRight);
    p_hLayoutBtn->setSpacing(0);
    p_hLayoutBtn->addWidget(p_btn_ok);
    p_hLayoutBtn->addWidget(p_btn_cancel);

    p_vLayoutMain->addItem(p_hLayoutBtn);

    this->setLayout(p_vLayoutMain);
}

void CMDBrushSelectDialog::sigAndSlotConnect()
{
    bool b = connect(p_btn_ok,SIGNAL(clicked()),this,SLOT(slotOkClicked()));
    b = connect(p_btn_cancel,SIGNAL(clicked()),this,SLOT(slotCancelClicked()));

    int i = 1;
}

void CMDBrushSelectDialog::setTextLabelInfo()
{
    this->setWindowTitle(tr("颜色编辑器"));
    p_btn_ok->setText(tr("确定"));
    p_btn_cancel->setText(tr("取消"));
}
