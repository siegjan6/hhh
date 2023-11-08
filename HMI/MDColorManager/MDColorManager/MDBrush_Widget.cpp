#include "MDBrush_Widget.h"

#include "./color_dialog/Color_Wheel"
#include "./color_dialog/Gradient_Slider"
#include "./color_dialog/Color_Preview"
#include "./color_dialog/Hue_Slider"
#include "./lineargradient/GradientRampLump.h"
#include "ColorExample.h"
#include "picture_dialog/FillPictureDialog.h"
#include "MDMultiLanguage.h"

const int label_height = 25;
const int wheel_width = 230;
/*
 * 功能：
 * 	  构造函数
 * 参数：
 * 	  1.QWidget *parent: 窗体父对象，默认为0
 * 返回值：
 * 	  无
 */
CMDBrush_Widget::CMDBrush_Widget(QWidget *parent) :
    QWidget(parent)
{
    p_vLayoutMain = NULL;
    p_gridLayout = NULL;
    p_hLayout = NULL;
    p_hLayout_btn_brType = NULL;
    p_vLayout = NULL;
    p_hLayout_btn = NULL;
    p_color_wheel = NULL;
    p_color_preview = NULL;
    p_linearBar = NULL;
    p_edit_hex = NULL;

    initialize();
    setUserDefineContent();
    setUserDefineStyle();
    setTextLabelInfo();
    setWinLayout();
    sigAndSlotConnect();
    updateData(false);

    QStringList types;
    types << "QLineEdit";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
}

/*
 * 功能：
 * 	  析构函数
 * 参数：
 * 	  1.QWidget *parent: 窗体父对象，默认为0
 * 返回值：
 * 	  无
 */
CMDBrush_Widget::~CMDBrush_Widget()
{
    release();
}

void CMDBrush_Widget::setBrushManager(CMDBrushManager brMgr)
{
    m_brushManager = brMgr;
    updateData(false);
}

ColorExample *CMDBrush_Widget::getColorExample()
{
    return p_example;
}

QColor CMDBrush_Widget::color() const
{
    QColor col = p_color_wheel->color();
    col.setAlpha(p_slide_alpha->value());
    return col;
}

/*
 * 功能：
 * 	   PushButton::Click事件
 *        切換畫刷類型，控制控件顯示隱藏，界面刷新
 * 参数：
 *        無
 * 返回值：
 * 	  无
 */
void CMDBrush_Widget::slotNClicked()
{
    m_brushManager.setBrushType(CMDBrushManager::BrushType::Nullptr);
    resetLayout(CMDBrushManager::BrushType::Nullptr);
    set_type(CMDBrushManager::BrushType::Nullptr);
    m_picture->close();
}

void CMDBrush_Widget::slotLClicked()
{
    m_brushManager.setBrushType(CMDBrushManager::BrushType::LinearGradient);
    resetLayout(CMDBrushManager::BrushType::LinearGradient);
    set_type(CMDBrushManager::BrushType::LinearGradient);
    setColor(m_brushManager.color());
    m_picture->close();
}

void CMDBrush_Widget::slotSClicked()
{
    m_brushManager.setBrushType(CMDBrushManager::BrushType::Solid);
    resetLayout(CMDBrushManager::BrushType::Solid);
    set_type(CMDBrushManager::BrushType::Solid);
    setColor(m_brushManager.color());
    m_picture->close();
}

void CMDBrush_Widget::slotRClicked()
{
    m_brushManager.setBrushType(CMDBrushManager::BrushType::RadialGradient);
    resetLayout(CMDBrushManager::BrushType::RadialGradient);
    set_type(CMDBrushManager::BrushType::RadialGradient);
    setColor(m_brushManager.color());
    m_picture->close();
}

void CMDBrush_Widget::slotCClicked()
{
    m_brushManager.setBrushType(CMDBrushManager::BrushType::ConicalGradient);
    resetLayout(CMDBrushManager::BrushType::ConicalGradient);
    set_type(CMDBrushManager::BrushType::ConicalGradient);
    setColor(m_brushManager.color());
    m_picture->close();
}

void CMDBrush_Widget::slotPClicked()
{
    m_brushManager.setBrushType(CMDBrushManager::BrushType::FillPicture);
    resetLayout(CMDBrushManager::BrushType::FillPicture);
    set_type(CMDBrushManager::BrushType::FillPicture);
    m_picture->m_position = m_brushManager.m_position;
    m_picture->m_imageName = m_brushManager.m_imageName;
    m_picture->inintDialog();
    m_picture->show();
}

/*
 * 功能：
 * 	  wheel颜色发生改变触发
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void CMDBrush_Widget::update_widgets()
{
    bool blocked = signalsBlocked();
    blockSignals(true);

    QList<QWidget*> widgetList = findChildren<QWidget*>();
    for(int i = 0; i < widgetList.count();i++)
       widgetList[i]->blockSignals(true);

    QColor col = color();

    p_slide_red->setValue(col.red());
    p_spin_red->setValue(p_slide_red->value());
    p_slide_red->setFirstColor(QColor(0,col.green(),col.blue()));
    p_slide_red->setLastColor(QColor(255,col.green(),col.blue()));

    p_slide_green->setValue(col.green());
    p_spin_green->setValue(p_slide_green->value());
    p_slide_green->setFirstColor(QColor(col.red(),0,col.blue()));
    p_slide_green->setLastColor(QColor(col.red(),255,col.blue()));

    p_slide_blue->setValue(col.blue());
    p_spin_blue->setValue(p_slide_blue->value());
    p_slide_blue->setFirstColor(QColor(col.red(),col.green(),0));
    p_slide_blue->setLastColor(QColor(col.red(),col.green(),255));

    p_slide_hue->setValue(qRound(p_color_wheel->hue()*360.0));
    p_slide_hue->setColorSaturation(p_color_wheel->saturation());
    p_slide_hue->setColorValue(p_color_wheel->value());
    p_spin_hue->setValue(p_slide_hue->value());

    p_slide_saturation->setValue(qRound(p_color_wheel->saturation()*255.0));
    p_spin_saturation->setValue(p_slide_saturation->value());
    p_slide_saturation->setFirstColor(QColor::fromHsvF(p_color_wheel->hue(),0,p_color_wheel->value()));
    p_slide_saturation->setLastColor(QColor::fromHsvF(p_color_wheel->hue(),1,p_color_wheel->value()));

    p_slide_value->setValue(qRound(p_color_wheel->value()*255.0));
    p_spin_value->setValue(p_slide_value->value());
    p_slide_value->setFirstColor(QColor::fromHsvF(p_color_wheel->hue(), p_color_wheel->saturation(),0));
    p_slide_value->setLastColor(QColor::fromHsvF(p_color_wheel->hue(), p_color_wheel->saturation(),1));


    QColor apha_color = col;
    apha_color.setAlpha(0);
    p_slide_alpha->setFirstColor(apha_color);
    apha_color.setAlpha(255);
    p_slide_alpha->setLastColor(apha_color);
    p_spin_alpha->setValue(p_slide_alpha->value());

    p_edit_hex->setText(col.name());

    p_color_preview->setColor(col);

    p_linearBar->setCurrentColor(color());
    int r = col.red();
    int g = col.green();
    int b = col.blue();
    int a = col.alpha();
    p_example->setColor(QColor::fromRgb(r,g,b,a));
    p_example->setAngle(p_slide_angle->value());
    p_example->setStops(p_linearBar->gradientStops());
    p_example->update();

    blockSignals(blocked);

    QList<QWidget*> widgetList1 = findChildren<QWidget*>();
    for(int i = 0; i < widgetList1.count();i++)
       widgetList1[i]->blockSignals(false);

}

void CMDBrush_Widget::set_hsv()
{
    if ( !signalsBlocked() )
    {
        p_color_wheel->setColor(QColor::fromHsv(
               p_slide_hue->value(),
               p_slide_saturation->value(),
               p_slide_value->value()
           ));
       update_widgets();
    }
}

void CMDBrush_Widget::set_rgb()
{
    if ( !signalsBlocked() )
    {
        QColor col(
                p_slide_red->value(),
                p_slide_green->value(),
                p_slide_blue->value()
            );
        if (col.saturation() == 0)
            col = QColor::fromHsv(p_slide_hue->value(), 0, col.value());
        p_color_wheel->setColor(col);
        update_widgets();
    }
}

void CMDBrush_Widget::set_angle()
{
    if ( !signalsBlocked() )
    {
        int angle = p_slide_angle->value();
        p_example->setAngle(angle);
        update_widgets();
    }
}

void CMDBrush_Widget::set_stops()
{
    if ( !signalsBlocked() )
    {
        QGradientStops stops = p_linearBar->gradientStops();
        p_example->setStops(stops);
        update_widgets();
    }
}

void CMDBrush_Widget::set_type(CMDBrushManager::BrushType type)
{
    if ( !signalsBlocked() )
    {
        if(type == CMDBrushManager::BrushType::Nullptr)
            p_btn_null->setFocus();
        else if(type == CMDBrushManager::BrushType::Solid)
           p_btn_solid->setFocus();
        else if(type == CMDBrushManager::BrushType::LinearGradient)
           p_btn_linearGradient->setFocus();
        else if(type == CMDBrushManager::BrushType::RadialGradient)
           p_btn_radialGradient->setFocus();
        else if(type == CMDBrushManager::BrushType::ConicalGradient)
           p_btn_conicalGradient->setFocus();
        else if(type == CMDBrushManager::BrushType::FillPicture) //! 填充图片
            p_btn_fillPicture->setFocus();
        p_example->setType(type);
        update_widgets();
    }
}

void CMDBrush_Widget::on_edit_hex_editingFinished()
{
    update_hex();
}

void CMDBrush_Widget::on_edit_hex_textEdited(const QString &arg1)
{
    int cursor = p_edit_hex->cursorPosition();
    update_hex();
    p_edit_hex->setText(arg1);
    p_edit_hex->setCursorPosition(cursor);
}

void CMDBrush_Widget::setColor(const QColor &clr)
{
    p_color_preview->setComparisonColor(clr);
    setColorInternal(clr);
}

void CMDBrush_Widget::update_hex()
{
    QString xs = p_edit_hex->text().trimmed();
       xs.remove('#');

       if ( xs.isEmpty() )
           return;

       if ( xs.indexOf(QRegExp("^[0-9a-fA-f]+$")) == -1 )
       {
           QColor c(xs);
           if ( c.isValid() )
           {
               setColorInternal(c);
               return;
           }
       }

       if ( xs.size() == 3 )
       {
           p_slide_red->setValue(QString(2,xs[0]).toInt(0,16));
           p_slide_green->setValue(QString(2,xs[1]).toInt(0,16));
           p_slide_blue->setValue(QString(2,xs[2]).toInt(0,16));
       }
       else
       {
           if ( xs.size() < 6 )
           {
               xs += QString(6-xs.size(),'0');
           }
           p_slide_red->setValue(xs.mid(0,2).toInt(0,16));
           p_slide_green->setValue(xs.mid(2,2).toInt(0,16));
           p_slide_blue->setValue(xs.mid(4,2).toInt(0,16));

           if ( xs.size() == 8 )
               p_slide_alpha->setValue(xs.mid(6,2).toInt(0,16));
       }

       set_rgb();
}

void CMDBrush_Widget::setColorInternal(const QColor &c)
{
    // Note. The difference between this method and setColor, is that setColor
       // sets the official starting color of the dialog, while this is used to update
       // the current color which might not be the final selected color.
       p_color_wheel->setColor(c);
       p_slide_alpha->setValue(c.alpha());
       update_widgets();
}

void CMDBrush_Widget::updateData(bool bSave)
{
    if(!bSave)//load
    {
        //type
        resetLayout(m_brushManager.brushType());
        set_type(m_brushManager.brushType());

        //angle
        p_slide_angle->setValue(m_brushManager.angle());
        p_example->setAngle(m_brushManager.angle());
        //color
        setColor(m_brushManager.color());

        //stops
        p_linearBar->setGradientStops(m_brushManager.stops());
        p_example->setStops(m_brushManager.stops());
    }
    else
    {
        m_brushManager.setAngle(p_slide_angle->value());
        m_brushManager.setColor(color());
        m_brushManager.setStops(p_linearBar->gradientStops());
    }
}

/*
 * 功能：
 * 	  窗体类内部成员对象内存分配及初始化
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void CMDBrush_Widget::initialize()
{
    p_hLayout_btn_brType = new QHBoxLayout();
    p_example = new ColorExample();
    m_picture = new CFillPictureDialog();
    p_hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    p_verticalSpacer = new QSpacerItem(20,88,QSizePolicy::Minimum,QSizePolicy::Expanding);
    p_vLayoutMain = new QVBoxLayout();
    p_hLayout_btn = new QHBoxLayout();
    p_gridLayout = new QGridLayout();
    p_hLayout = new QHBoxLayout();
    p_vLayout = new QVBoxLayout();
    p_color_wheel = new Color_Wheel();
    p_color_preview = new Color_Preview();
    p_linearBar = new GradientRampLump();
    p_label_angle = new QLabel();p_labelList.append(p_label_angle);

    p_label_r = new QLabel();p_labelList.append(p_label_r);
    p_label_g = new QLabel();p_labelList.append(p_label_g);
    p_label_b = new QLabel();p_labelList.append(p_label_b);
    p_label_a = new QLabel();p_labelList.append(p_label_a);
    p_label_value = new QLabel();p_labelList.append(p_label_value);
    p_label_hue = new QLabel();p_labelList.append(p_label_hue);
    p_label_saturation = new QLabel();p_labelList.append(p_label_saturation);
    p_label_hex = new QLabel();p_labelList.append(p_label_hex);

    p_btn_null = new QPushButton();
    p_btn_solid = new QPushButton();
    p_btn_linearGradient = new QPushButton();
    p_btn_radialGradient = new QPushButton();
    p_btn_conicalGradient = new QPushButton();
    p_btn_fillPicture = new QPushButton();
    p_slide_alpha = new Gradient_Slider();p_sliderList.append(p_slide_alpha);
    p_slide_red = new Gradient_Slider();p_sliderList.append(p_slide_red);
    p_slide_green = new Gradient_Slider();p_sliderList.append(p_slide_green);
    p_slide_blue = new Gradient_Slider();p_sliderList.append(p_slide_blue);
    p_slide_hue = new Hue_Slider();p_sliderList.append(p_slide_hue);
    p_slide_saturation = new Gradient_Slider();p_sliderList.append(p_slide_saturation);
    p_slide_value = new Gradient_Slider();p_sliderList.append(p_slide_value);
    p_slide_angle = new Gradient_Slider();p_sliderList.append(p_slide_angle);
    p_spin_alpha = new QSpinBox();p_spinList.append(p_spin_alpha);
    p_spin_angle = new QSpinBox();p_spinList.append(p_spin_angle);
    p_spin_red = new QSpinBox();p_spinList.append(p_spin_red);
    p_spin_green = new QSpinBox();p_spinList.append(p_spin_green);
    p_spin_blue = new QSpinBox();p_spinList.append(p_spin_blue);
    p_spin_hue = new QSpinBox();p_spinList.append(p_spin_hue);
    p_spin_saturation = new QSpinBox();p_spinList.append(p_spin_saturation);
    p_spin_value = new QSpinBox();p_spinList.append(p_spin_value);
    p_edit_hex = new QLineEdit();

    setAttribute(Qt::WA_ShowModal, true);
}

/*
 * 功能：
 * 	  窗体类内部成员对象（initialize()函数中创建的对象）内存释放
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void CMDBrush_Widget::release()
{
    delete p_vLayoutMain;
//    delete p_gridLayout;
//    delete p_hLayout;
//    delete p_hLayout_btn_brType;
//    delete p_vLayout;
//    delete p_hLayout_btn;
    delete p_color_wheel;
    delete p_color_preview;
    delete p_linearBar;
    delete p_edit_hex;
    delete p_label_r;
    delete p_label_g;
    delete p_label_b;
    delete p_label_a;
    delete p_label_value;
    delete p_label_hue;
    delete p_label_saturation;
    delete p_label_hex;
    delete p_btn_null;
    delete p_btn_solid;
    delete p_btn_linearGradient;
    delete p_btn_radialGradient;
    delete p_btn_conicalGradient;
    delete p_btn_fillPicture;
    delete p_slide_hue;
    delete p_slide_alpha;
    delete p_slide_red;
    delete p_slide_green;
    delete p_slide_blue;
    delete p_slide_saturation;
    delete p_slide_value;
    delete p_spin_alpha;
    delete p_spin_red;
    delete p_spin_green;
    delete p_spin_blue;
    delete p_spin_hue;
    delete p_spin_saturation;
    delete p_spin_value;
    delete p_spin_angle;
    delete p_slide_angle;
    delete p_label_angle;
//    delete p_verticalSpacer;
//    delete p_hSpacer;
    delete p_example;
    delete m_picture;
}

/*
 * 功能：
 * 	  窗体自定义风格相关属性设置，如该窗体与整体应用风格切换有关，则相关部分需要在对应的
 * 风格设置文件中设置，由应用统一进行设置和切换。
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void CMDBrush_Widget::setUserDefineStyle()
{
    const int btn_width = 30;
    const int btn_height = 30;
    p_btn_null->setFixedSize(btn_width,btn_height);
    p_btn_null->setIcon(QIcon(":/color_widgets/png/null.png"));
    p_btn_null->setIconSize(QSize(btn_width,btn_height));

    p_btn_solid->setFixedSize(btn_width,btn_height);
    p_btn_solid->setIcon(QIcon(":/color_widgets/png/solid.png"));
    p_btn_solid->setIconSize(QSize(btn_width,btn_height));

    p_btn_linearGradient->setFixedSize(btn_width,btn_height);
    p_btn_linearGradient->setIcon(QIcon(":/color_widgets/png/lineargradient.png"));
    p_btn_linearGradient->setIconSize(QSize(btn_width,btn_height));

    p_btn_radialGradient->setFixedSize(btn_width,btn_height);
    p_btn_radialGradient->setIcon(QIcon(":/color_widgets/png/radialgradient.png"));
    p_btn_radialGradient->setIconSize(QSize(btn_width,btn_height));

    p_btn_conicalGradient->setFixedSize(btn_width,btn_height);
    p_btn_conicalGradient->setIcon(QIcon(":/color_widgets/png/conicalgradient.png"));
    p_btn_conicalGradient->setIconSize(QSize(btn_width,btn_height));

    p_btn_fillPicture->setFixedSize(btn_width,btn_height);
    p_btn_fillPicture->setIcon(QIcon(":/color_widgets/png/fillpicture.png"));
    p_btn_fillPicture->setIconSize(QSize(btn_width,btn_height));


    p_spin_alpha->setWrapping(true);
    p_spin_alpha->setMaximum(359);
    p_spin_saturation->setMaximum(255);
    p_spin_value->setMaximum(255);
    p_spin_red->setMaximum(255);
    p_spin_green->setMaximum(255);
    p_spin_blue->setMaximum(255);
    p_spin_alpha->setMaximum(255);
    p_spin_hue->setMinimum(1);
    p_spin_hue->setMaximum(359);
    p_spin_angle->setMinimum(0);
    p_spin_angle->setMaximum(360);

    p_slide_hue->setMinimum(1);
    p_slide_hue->setMaximum(359);
    p_slide_alpha->setMaximum(255);
    p_slide_red->setMaximum(255);
    p_slide_green->setMaximum(255);
    p_slide_blue->setMaximum(255);
    p_slide_saturation->setMaximum(255);
    p_slide_value->setMaximum(255);
    p_slide_angle->setMinimum(0);
    p_slide_angle->setMaximum(360);

    p_edit_hex->setFixedSize(180,label_height);

    p_color_wheel->setFixedSize(wheel_width,wheel_width);

    p_color_preview->setFixedSize(wheel_width,label_height);
    p_linearBar->setFixedSize(wheel_width,35);


    p_example->setFixedSize(300,300);
    p_example->m_isPen = false;
    for(int i = 0; i < p_labelList.count();i++)
    {
        QLabel* pLabel = p_labelList[i];
        pLabel->setAlignment(Qt::AlignCenter);
        pLabel->setFixedHeight(label_height);
        pLabel->setContentsMargins(0,0,0,0);
    }
    for(int i = 0; i < p_btnList.count();i++)
        p_btnList[i]->setFixedSize(90,label_height);

    for(int i = 0; i < p_sliderList.count();i++)
        p_sliderList[i]->setFixedSize(180,label_height);

    for(int i = 0; i < p_spinList.count();i++)
        p_spinList[i]->setFixedSize(60,label_height);

     p_vLayout->setSpacing(0);
     p_vLayout->setContentsMargins(0,0,0,0);
     setContentsMargins(0,0,0,0);
     this->setFixedSize(850,389);
     //this->resize(450,280);
     //窗体属性设置
    // setAttribute(Qt::WA_TranslucentBackground, true);
    // this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

/*
 * 功能：
 * 	  窗体内部显示相关部分设置，主要是用户提示相关的文本，在应用语言切换时需要重新载入
 * 参数：
 * 	  无
 * 返回值：1451
 * 	  无
 */
void CMDBrush_Widget::setUserDefineContent()
{
    p_label_a->setObjectName("p_label_a");
    p_label_r->setObjectName("p_label_r");
    p_label_g->setObjectName("p_label_g");
    p_label_b->setObjectName("p_label_b");
    p_label_hex->setObjectName("p_label_hex");
    p_label_hue->setObjectName("p_label_hue");
    p_label_value->setObjectName("p_label_value");
    p_label_saturation->setObjectName("p_label_saturation");
    p_label_angle->setObjectName("p_label_angle");
    p_slide_alpha->setObjectName("p_slide_alpha");
    p_slide_red->setObjectName("p_slide_red");
    p_slide_green->setObjectName("p_slide_green");
    p_slide_blue->setObjectName("p_slide_blue");
    p_slide_saturation->setObjectName("p_slide_saturation");
    p_slide_value->setObjectName("p_slide_value");
    p_slide_hue->setObjectName("p_slide_hue");
    p_spin_alpha->setObjectName("p_spin_alpha");
    p_spin_red->setObjectName("p_spin_red");
    p_spin_green->setObjectName("p_spin_green");
    p_spin_blue->setObjectName("p_spin_blue");
    p_spin_hue->setObjectName("p_spin_hux");
    p_spin_saturation->setObjectName("p_spin_staturation");
    p_spin_value->setObjectName("p_spin_value");
    p_color_wheel->setObjectName("p_color_wheel");
    m_picture->setObjectName("m_picture");
    p_edit_hex->setObjectName("p_edit_hex");
    p_color_preview->setObjectName("p_color_preview");
    p_linearBar->setObjectName("p_linearBar");
}

/*
 * 功能：
 * 	  窗体布局设置
 * 	  总体垂平布局p_vLayoutMain
 * 		|——p_frameMain
 * 			 |——垂直布局p_VLayoutFrame
 * 					|——p_label
 *                  |——水平布局p_HLayout
 *                     |——p_btn_Yes
 *                     |——p_btn_No
 * 参数：
 * 	  无
 * 返回值：
 * 	  无
 */
void CMDBrush_Widget::setWinLayout()
{
    //gridLayout
    p_gridLayout->addWidget(p_label_hue,0,0);
    p_gridLayout->addWidget(p_label_saturation,1,0);
    p_gridLayout->addWidget(p_label_value,2,0);
    p_gridLayout->addWidget(p_label_r,3,0);
    p_gridLayout->addWidget(p_label_g,4,0);
    p_gridLayout->addWidget(p_label_b,5,0);
    p_gridLayout->addWidget(p_label_a,6,0);
    p_gridLayout->addWidget(p_label_angle,7,0);
    p_gridLayout->addWidget(p_label_hex,8,0);

    p_gridLayout->addWidget(p_slide_hue,0,1);
    p_gridLayout->addWidget(p_slide_saturation,1,1);
    p_gridLayout->addWidget(p_slide_value,2,1);
    p_gridLayout->addWidget(p_slide_red,3,1);
    p_gridLayout->addWidget(p_slide_green,4,1);
    p_gridLayout->addWidget(p_slide_blue,5,1);
    p_gridLayout->addWidget(p_slide_alpha,6,1);
    p_gridLayout->addWidget(p_slide_angle,7,1);

    p_gridLayout->addWidget(p_spin_hue,0,2);
    p_gridLayout->addWidget(p_spin_saturation,1,2);
    p_gridLayout->addWidget(p_spin_value,2,2);
    p_gridLayout->addWidget(p_spin_red,3,2);
    p_gridLayout->addWidget(p_spin_green,4,2);
    p_gridLayout->addWidget(p_spin_blue,5,2);
    p_gridLayout->addWidget(p_spin_alpha,6,2);
    p_gridLayout->addWidget(p_spin_angle,7,2);

    p_gridLayout->addWidget(p_edit_hex,8,1,0,2);

    //vLayout
    p_vLayout->addWidget(p_color_wheel);
    p_vLayout->addSpacing(13);
    p_vLayout->addWidget(p_linearBar);
    p_linearBar->hide();
    p_vLayout->addWidget(p_color_preview);

    //hLayout
    p_hLayout->addLayout(p_vLayout);
    p_hLayout->addSpacing(5);
    p_hLayout->addLayout(p_gridLayout);
    p_hLayout->addWidget(p_example);
    p_hLayout->addWidget(m_picture);//temp

    //hLayout brushType
    p_hLayout_btn_brType->addWidget(p_btn_null);
    p_hLayout_btn_brType->addWidget(p_btn_solid);
    p_hLayout_btn_brType->addWidget(p_btn_linearGradient);
    p_hLayout_btn_brType->addWidget(p_btn_radialGradient);
    p_hLayout_btn_brType->addWidget(p_btn_conicalGradient);
    p_hLayout_btn_brType->addWidget(p_btn_fillPicture);
    p_hLayout_btn_brType->addItem(p_hSpacer);

    p_vLayoutMain->addLayout(p_hLayout_btn_brType);
    p_vLayoutMain->addLayout(p_hLayout);
    p_vLayoutMain->addLayout(p_hLayout_btn);
    p_vLayoutMain->addItem(p_verticalSpacer);
    this->setLayout(p_vLayoutMain);
}

/*
 * 功能：
 * 	  相关信号&槽连接函数
 * 参数：
 *    无
 * 返回值：
 * 	  无
 */
void CMDBrush_Widget::sigAndSlotConnect()
{
    connect(p_color_wheel,SIGNAL(colorSelected(QColor)),this,SLOT(update_widgets()));
    connect(p_color_wheel,SIGNAL(colorSelected(QColor)),p_example,SLOT(setColor(QColor)));

    connect(p_spin_alpha,SIGNAL(valueChanged(int)),p_slide_alpha,SLOT(setValue(int)));
    connect(p_spin_red,SIGNAL(valueChanged(int)),p_slide_red,SLOT(setValue(int)));
    connect(p_spin_green,SIGNAL(valueChanged(int)),p_slide_green,SLOT(setValue(int)));
    connect(p_spin_blue,SIGNAL(valueChanged(int)),p_slide_blue,SLOT(setValue(int)));
    connect(p_spin_hue,SIGNAL(valueChanged(int)),p_slide_hue,SLOT(setValue(int)));
    connect(p_spin_saturation,SIGNAL(valueChanged(int)),p_slide_saturation,SLOT(setValue(int)));
    connect(p_spin_value,SIGNAL(valueChanged(int)),p_slide_value,SLOT(setValue(int)));

    connect(p_slide_alpha,SIGNAL(valueChanged(int)),p_spin_alpha,SLOT(setValue(int)));
    connect(p_slide_red,SIGNAL(valueChanged(int)),p_spin_red,SLOT(setValue(int)));
    connect(p_slide_green,SIGNAL(valueChanged(int)),p_spin_green,SLOT(setValue(int)));
    connect(p_slide_blue,SIGNAL(valueChanged(int)),p_spin_blue,SLOT(setValue(int)));
    connect(p_slide_hue,SIGNAL(valueChanged(int)),p_spin_hue,SLOT(setValue(int)));
    connect(p_slide_saturation,SIGNAL(valueChanged(int)),p_spin_saturation,SLOT(setValue(int)));
    connect(p_slide_value,SIGNAL(valueChanged(int)),p_spin_value,SLOT(setValue(int)));

    connect(p_slide_alpha,SIGNAL(valueChanged(int)),this,SLOT(update_widgets()));
    connect(p_slide_red,SIGNAL(valueChanged(int)),this,SLOT(set_rgb()));
    connect(p_slide_green,SIGNAL(valueChanged(int)),this,SLOT(set_rgb()));
    connect(p_slide_blue,SIGNAL(valueChanged(int)),this,SLOT(set_rgb()));
    connect(p_slide_hue,SIGNAL(valueChanged(int)),this,SLOT(set_hsv()));
    connect(p_slide_saturation,SIGNAL(valueChanged(int)),this,SLOT(set_hsv()));
    connect(p_slide_value,SIGNAL(valueChanged(int)),this,SLOT(set_hsv()));
    connect(p_edit_hex,&QLineEdit::textEdited,this,&CMDBrush_Widget::on_edit_hex_textEdited);
    connect(p_edit_hex,&QLineEdit::editingFinished,this,&CMDBrush_Widget::on_edit_hex_editingFinished);

    connect(p_spin_angle,SIGNAL(valueChanged(int)),p_slide_angle,SLOT(setValue(int)));
    connect(p_slide_angle,SIGNAL(valueChanged(int)),p_spin_angle,SLOT(setValue(int)));
    bool b = connect(p_slide_angle,SIGNAL(valueChanged(int)),this,SLOT(set_angle()));

    b = connect(p_linearBar,&GradientRampLump::gradientStopsChange,this,&CMDBrush_Widget::set_stops);
    b = connect(p_linearBar,&GradientRampLump::colorChange,this,&CMDBrush_Widget::setColor);


    connect(p_btn_null,SIGNAL(clicked()),this,SLOT(slotNClicked()));
    connect(p_btn_solid,SIGNAL(clicked()),this,SLOT(slotSClicked()));
    connect(p_btn_linearGradient,SIGNAL(clicked()),this,SLOT(slotLClicked()));
    connect(p_btn_radialGradient,SIGNAL(clicked()),this,SLOT(slotRClicked()));
    connect(p_btn_conicalGradient,SIGNAL(clicked()),this,SLOT(slotCClicked()));
    connect(p_btn_fillPicture,SIGNAL(clicked()),this,SLOT(slotPClicked()));
}

void CMDBrush_Widget::setTextLabelInfo()
{
    p_label_r->setText(tr("红色:"));
    p_label_g->setText(tr("绿色:"));
    p_label_b->setText(tr("蓝色:"));
    p_label_a->setText(tr("透明:"));
    p_label_hue->setText(tr("色调:"));
    p_label_saturation->setText(tr("彩度:"));
    p_label_value->setText(tr("明度:"));
    p_label_hex->setText(tr("  值:"));
    p_label_angle->setText(tr("角度:"));

    p_btn_null->setToolTip(tr("空画刷"));
//    CMDMultiLanguage::instance()->translateWidget(p_btn_null);
    p_btn_null->setStyleSheet("QPushButton{border:none;}");

    p_btn_solid->setToolTip(tr("单色"));
    p_btn_solid->setStyleSheet("QPushButton{border:none;}");
    p_btn_linearGradient->setToolTip(tr("线性渐变"));
    p_btn_linearGradient->setStyleSheet("QPushButton{border:none;}");
    p_btn_radialGradient->setToolTip(tr("辐向渐变"));
    p_btn_radialGradient->setStyleSheet("QPushButton{border:none;}");
    p_btn_conicalGradient->setToolTip(tr("圆锥渐变"));
    p_btn_conicalGradient->setStyleSheet("QPushButton{border:none;}");
    p_btn_fillPicture->setToolTip(tr("图片填充"));
    p_btn_fillPicture->setStyleSheet("QPushButton{border:none;}");
}

void CMDBrush_Widget::resetLayout(CMDBrushManager::BrushType type)
{
    if(type == CMDBrushManager::BrushType::Nullptr)
    {
        p_color_wheel->hide();
        p_color_preview->hide();
        p_linearBar->hide();
        for(int i = 0; i < p_labelList.count();i++)
            p_labelList[i]->hide();
        for(int i = 0; i < p_spinList.count();i++)
            p_spinList[i]->hide();
        for(int i = 0; i < p_sliderList.count();i++)
            p_sliderList[i]->hide();
        p_edit_hex->hide();
    }
    else if(type == CMDBrushManager::BrushType::LinearGradient)
    {
        p_color_wheel->show();
        p_color_preview->hide();
        p_linearBar->show();
        for(int i = 0; i < p_labelList.count();i++)
            p_labelList[i]->show();
        for(int i = 0; i < p_spinList.count();i++)
            p_spinList[i]->show();
        for(int i = 0; i < p_sliderList.count();i++)
            p_sliderList[i]->show();
        p_edit_hex->show();
    }
    else if(type == CMDBrushManager::BrushType::Solid)
    {
        p_color_wheel->show();
        p_color_preview->show();
        p_linearBar->hide();
        for(int i = 0; i < p_labelList.count();i++)
            p_labelList[i]->show();
        for(int i = 0; i < p_spinList.count();i++)
            p_spinList[i]->show();
        for(int i = 0; i < p_sliderList.count();i++)
            p_sliderList[i]->show();
        p_edit_hex->show();
        p_slide_angle->hide();
        p_spin_angle->hide();
        p_label_angle->hide();
    }
    else if(type == CMDBrushManager::BrushType::RadialGradient)
    {
        p_color_wheel->show();
        p_color_preview->hide();
        p_linearBar->show();
        for(int i = 0; i < p_labelList.count();i++)
            p_labelList[i]->show();
        for(int i = 0; i < p_spinList.count();i++)
            p_spinList[i]->show();
        for(int i = 0; i < p_sliderList.count();i++)
            p_sliderList[i]->show();
        p_slide_angle->hide();
        p_spin_angle->hide();
        p_label_angle->hide();
        p_edit_hex->show();
    }
    else if(type == CMDBrushManager::BrushType::ConicalGradient)
    {
        p_color_wheel->show();
        p_color_preview->hide();
        p_linearBar->show();
        for(int i = 0; i < p_labelList.count();i++)
            p_labelList[i]->show();
        for(int i = 0; i < p_spinList.count();i++)
            p_spinList[i]->show();
        for(int i = 0; i < p_sliderList.count();i++)
            p_sliderList[i]->show();;
        p_slide_angle->hide();
        p_spin_angle->hide();
        p_label_angle->hide();
        p_edit_hex->show();
    }
    else if(type == CMDBrushManager::BrushType::FillPicture)
    {
        m_picture->show();
        p_color_wheel->hide();
        p_color_preview->hide();
        p_linearBar->hide();
        for(int i = 0; i < p_labelList.count();i++)
            p_labelList[i]->hide();
        for(int i = 0; i < p_spinList.count();i++)
            p_spinList[i]->hide();
        for(int i = 0; i < p_sliderList.count();i++)
            p_sliderList[i]->hide();
        p_edit_hex->hide();
    }
}
