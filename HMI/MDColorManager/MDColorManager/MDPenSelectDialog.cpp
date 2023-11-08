#include "MDPenSelectDialog.h"
#include "MDBrush_Widget.h"
#include "ColorExample.h"
#include "MDMultiLanguage.h"

int joinToIndex(Qt::PenJoinStyle style);
Qt::PenJoinStyle indexToJoin(int index);
int capToIndex(Qt::PenCapStyle cap);
Qt::PenCapStyle indexToCap(int index);
int penStyleToIndex(Qt::PenStyle style);
Qt::PenStyle indexToPenStyle(int index);

CMDPenSelectDialog::CMDPenSelectDialog(QWidget *parent)
{
    initialize();
    setUserDefineStyle(); 
    setWinLayout();
    sigAndSlotConnect();
    setTextLabelInfo();

    CMDMultiLanguage::instance()->translateWidget(this);
}

CMDPenSelectDialog::~CMDPenSelectDialog()
{
    release();
}

CMDPenManager CMDPenSelectDialog::penManager()
{
    updateData(true);
    return m_penManager;
}

void CMDPenSelectDialog::setPenManager(CMDPenManager penMgr)
{
    m_penManager = penMgr;
    updateData(false);
}

CMDBrushManager CMDPenSelectDialog::brushManager()
{
    return p_widgetBrush->brushManager();
}

void CMDPenSelectDialog::setBrushManager(CMDBrushManager brushMgr)
{
    p_widgetBrush->setBrushManager(brushMgr);
}

void CMDPenSelectDialog::slotOkClicked()
{
    QDialog::accept();
}

void CMDPenSelectDialog::slotCancelClicked()
{
    QDialog::rejected();
    close();
}

void CMDPenSelectDialog::slotComboBoxCurrentIndexChange_style(int index)
{
    ColorExample* example = p_widgetBrush->getColorExample();
    Qt::PenStyle style = indexToPenStyle(index);
    if(example != NULL)
    {
        example->setPenStyle(style);
        example->update();
    }
}

void CMDPenSelectDialog::slotComboBoxCurrentIndexChange_capStyle(int index)
{
    ColorExample* example = p_widgetBrush->getColorExample();
    Qt::PenCapStyle  style = indexToCap(index);
    if(example != NULL)
    {
        example->setCapStyle(style);
        example->update();
    }
}

void CMDPenSelectDialog::slotComboBoxCurrentIndexChange_joinStyle(int index)
{
    ColorExample* example = p_widgetBrush->getColorExample();
    Qt::PenJoinStyle style = indexToJoin(index);
    if(example != NULL)
    {
        example->setJoinStyle(style);
        example->update();
    }
}

void CMDPenSelectDialog::slotSliderWidthChange(int val)
{
    ColorExample* example = p_widgetBrush->getColorExample();
    if(example != NULL)
    {
        example->setWidth(val);
        example->update();
    }
}

void CMDPenSelectDialog::initialize()
{
    m_edit = NULL;
    p_widgetBrush = new CMDBrush_Widget();
    p_btn_ok = new QPushButton();
    p_btn_cancel = new QPushButton();
    p_vLayoutMain = new QVBoxLayout();
    p_hLayoutBtn = new QHBoxLayout();
    p_hLayoutComboBox = new QHBoxLayout();
    p_slider_width = new QSpinBox();
    p_comboBox_capStyle = new QComboBox();
    p_comboBox_joinStyle = new QComboBox();
    p_comboBox_style = new QComboBox();
    p_label_penWidth = new QLabel();
    p_label_penStyle = new QLabel();
    p_label_penCapStyle = new QLabel();
    p_label_penJoinStyle = new QLabel();
    p_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
}

void CMDPenSelectDialog::release()
{
    delete p_vLayoutMain;
//    delete p_hLayoutBtn;
//    delete p_hLayoutComboBox;
//    delete p_horizontalSpacer;
    delete p_widgetBrush;
    delete p_btn_ok;
    delete p_btn_cancel;
    delete p_slider_width;
    delete p_comboBox_capStyle;
    delete p_comboBox_joinStyle;
    delete p_comboBox_style;
    delete p_label_penWidth;
    delete p_label_penStyle;
    delete p_label_penCapStyle;
    delete p_label_penJoinStyle;
}

void CMDPenSelectDialog::setUserDefineStyle()
{
    p_widgetBrush->getColorExample()->m_isPen = true;
    const int btn_width = 90,btn_height = 25;
    const int comboBox_width = 90,comboBox_height = 25;
    p_btn_ok->setFixedSize(btn_width,btn_height);
    p_btn_cancel->setFixedSize(btn_width,btn_height);
    p_slider_width->setFixedSize(comboBox_width,comboBox_height);
    p_comboBox_capStyle->setFixedSize(comboBox_width,comboBox_height);
    p_comboBox_joinStyle->setFixedSize(comboBox_width,comboBox_height);
    p_comboBox_style->setFixedSize(comboBox_width,comboBox_height);

 //   p_widgetBrush->setSizeIncrement(831,290);
    this->setFixedSize(866,440);
    initComboBoxInit();
}

void CMDPenSelectDialog::setWinLayout()
{
    p_hLayoutComboBox->setAlignment(Qt::AlignLeft);
    p_hLayoutComboBox->setSpacing(0);
    p_hLayoutComboBox->addWidget(p_label_penWidth);
    p_hLayoutComboBox->addWidget(p_slider_width);
    p_hLayoutComboBox->addSpacing(10);
    p_hLayoutComboBox->addWidget(p_label_penStyle);
    p_hLayoutComboBox->addWidget(p_comboBox_style);
    p_hLayoutComboBox->addSpacing(10);
    p_hLayoutComboBox->addWidget(p_label_penCapStyle);
    p_hLayoutComboBox->addWidget(p_comboBox_capStyle);
    p_hLayoutComboBox->addSpacing(10);
    p_hLayoutComboBox->addWidget(p_label_penJoinStyle);
    p_hLayoutComboBox->addWidget(p_comboBox_joinStyle);
    p_hLayoutComboBox->addSpacing(10);
    p_hLayoutComboBox->addItem(p_horizontalSpacer);

    p_hLayoutBtn->setAlignment(Qt::AlignRight);
    p_hLayoutBtn->setSpacing(0);
    p_hLayoutBtn->addWidget(p_btn_ok);
    p_hLayoutBtn->addWidget(p_btn_cancel);

    p_vLayoutMain->addItem(p_hLayoutComboBox);
    p_vLayoutMain->addWidget(p_widgetBrush);
    p_vLayoutMain->addItem(p_hLayoutBtn);

    this->setLayout(p_vLayoutMain);
}

void CMDPenSelectDialog::sigAndSlotConnect()
{
    bool b = connect(p_btn_ok,SIGNAL(clicked()),this,SLOT(slotOkClicked()));
    b = connect(p_btn_cancel,SIGNAL(clicked()),this,SLOT(slotCancelClicked()));
    b = connect(p_comboBox_style,SIGNAL(currentIndexChanged(int)),this,SLOT(slotComboBoxCurrentIndexChange_style(int)));
    b = connect(p_comboBox_capStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slotComboBoxCurrentIndexChange_capStyle(int)));
    b = connect(p_comboBox_joinStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(slotComboBoxCurrentIndexChange_joinStyle(int)));
    b = connect(p_slider_width,SIGNAL(valueChanged(int)),this,SLOT(slotSliderWidthChange(int)));
}

void CMDPenSelectDialog::setTextLabelInfo()
{
    this->setWindowTitle(tr("颜色编辑器"));
    p_btn_ok->setText(tr("确定"));
    p_btn_cancel->setText(tr("取消"));
    p_label_penWidth->setText(tr("宽度:"));
    p_label_penStyle->setText(tr("线型:"));
    p_label_penCapStyle->setText(tr("线帽:"));
    p_label_penJoinStyle->setText(tr("交点:"));
}

void CMDPenSelectDialog::initComboBoxInit()
{
    p_comboBox_style->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    p_comboBox_capStyle->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    p_comboBox_joinStyle->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    p_comboBox_style->setIconSize(QSize(70,10));
    p_comboBox_style->addItem( QIcon(":/color_widgets/png/nopen.png"),"" );
    p_comboBox_style->addItem( QIcon(":/color_widgets/png/solidline.png"),"" );
    p_comboBox_style->addItem( QIcon(":/color_widgets/png/dashline.png"),"" );
    p_comboBox_style->addItem( QIcon(":/color_widgets/png/dotline.png"),"" );
    p_comboBox_style->addItem( QIcon(":/color_widgets/png/dashdotline.png"),"" );
    p_comboBox_style->addItem( QIcon(":/color_widgets/png/dashdotdotline.png"),"" );

    p_comboBox_capStyle->setIconSize(QSize(70,10));
    p_comboBox_capStyle->addItem( QIcon(":/color_widgets/png/flatcap.png"),"" );
    p_comboBox_capStyle->addItem( QIcon(":/color_widgets/png/squarecap.png"),"" );
    p_comboBox_capStyle->addItem( QIcon(":/color_widgets/png/roundcap.png"),"" );
   // p_comboBox_capStyle->addItem( QIcon(":/color_widgets/png/null.png"),"SvgMiterJoin" ); //暂无
   // p_comboBox_capStyle->addItem( QIcon(":/color_widgets/png/null.png"),"MPenJoinStyle" ); //暂无

    p_comboBox_joinStyle->setIconSize(QSize(70,10));
    p_comboBox_joinStyle->addItem( QIcon(":/color_widgets/png/miterjoin.png"),"" );
    p_comboBox_joinStyle->addItem( QIcon(":/color_widgets/png/beveljoin.png"),"" );
    p_comboBox_joinStyle->addItem( QIcon(":/color_widgets/png/roundjoin.png"),"" );
 //   p_comboBox_joinStyle->addItem( QIcon(":/color_widgets/png/null.png"),"SvgMiterJoin" ); //暂无
  //  p_comboBox_joinStyle->addItem( QIcon(":/color_widgets/png/null.png"),"MPenJoinStyle" ); //暂无
}

void CMDPenSelectDialog::updateData(bool bSave)
{
    if(!bSave)
    {
        p_slider_width->setValue(m_penManager.width());
        p_comboBox_capStyle->setCurrentIndex(capToIndex(m_penManager.capStyle()));
        p_comboBox_joinStyle->setCurrentIndex(joinToIndex(m_penManager.joinStyle()));
        p_comboBox_style->setCurrentIndex(penStyleToIndex(m_penManager.style()));
        p_widgetBrush->setBrushManager(m_penManager.brushManager());
    }
    else
    {
        m_penManager.setWidth(p_slider_width->value());
        m_penManager.setStyle(indexToPenStyle(p_comboBox_style->currentIndex()));
        m_penManager.setCapStyle(indexToCap(p_comboBox_capStyle->currentIndex()));
        m_penManager.setJoinStyle(indexToJoin(p_comboBox_joinStyle->currentIndex()));
        m_penManager.setBrushManager(p_widgetBrush->brushManager());
    }
}

int joinToIndex(Qt::PenJoinStyle style)
{
     int index;
     switch(style)
     {
     case Qt::PenJoinStyle::MiterJoin:
         index = 0;
         break;
     case Qt::PenJoinStyle::BevelJoin:
         index = 1;
         break;
     case Qt::PenJoinStyle::RoundJoin:
         index = 2;
         break;
     case Qt::PenJoinStyle::SvgMiterJoin:
         index = 1;
         break;
     case Qt::PenJoinStyle::MPenJoinStyle:
         index = 1;
         break;
     default:
         index = 1;
         break;
     }
     return index;
}

Qt::PenJoinStyle indexToJoin(int index)
{
    Qt::PenJoinStyle joins[]={
           Qt::PenJoinStyle::MiterJoin,
           Qt::PenJoinStyle::BevelJoin,
           Qt::PenJoinStyle::RoundJoin,
           Qt::PenJoinStyle::SvgMiterJoin,
           Qt::PenJoinStyle::MPenJoinStyle
       };

     return joins[index];
}

int capToIndex(Qt::PenCapStyle cap)
{
    int index;
    switch(cap)
    {
    case Qt::PenCapStyle::FlatCap:
        index = 0;
        break;
    case Qt::PenCapStyle::SquareCap:
        index = 1;
        break;
    case Qt::PenCapStyle::RoundCap:
        index = 2;
        break;
    case Qt::PenCapStyle::MPenCapStyle:
        index = 1;
        break;
    default:
        index = -1;
        break;
    }
    return index;
}

Qt::PenCapStyle indexToCap(int index)
{
    Qt::PenCapStyle caps[]={
           Qt::PenCapStyle::FlatCap,
           Qt::PenCapStyle::SquareCap,
           Qt::PenCapStyle::RoundCap,
           Qt::PenCapStyle::MPenCapStyle
       };

     return caps[index];
}

int penStyleToIndex(Qt::PenStyle style)
{
    int index;
    switch(style)
    {
    case Qt::PenStyle::NoPen:
        index = 0;
        break;
    case Qt::PenStyle::SolidLine:
        index = 1;
        break;
    case Qt::PenStyle::DashLine:
        index = 2;
        break;
    case Qt::PenStyle::DotLine:
        index = 3;
        break;
    case Qt::PenStyle::DashDotLine:
        index = 3;
        break;
    case Qt::PenStyle::DashDotDotLine:
        index = 3;
        break;
    default:
        index = 1;
        break;
    }
    return index;
}

Qt::PenStyle indexToPenStyle(int index)
{
    Qt::PenStyle pss[]={
        Qt::PenStyle::NoPen,
        Qt::PenStyle::SolidLine,
        Qt::PenStyle::DashLine,
        Qt::PenStyle::DotLine,
        Qt::PenStyle::DashDotLine,
        Qt::PenStyle::DashDotDotLine
       };
     return pss[index];
}
