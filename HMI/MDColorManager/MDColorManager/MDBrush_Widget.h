#ifndef MDBRUSH_WIDGET_H
#define MDBRUSH_WIDGET_H
#include <QDialog>
#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMap>
#include <QPainter>
#include <QSpinBox>
#include <QList>
#include <QLineEdit>
#include <QSpacerItem>
#include <QComboBox>
#include "MDBrushManager.h"
class Color_Wheel;
class Color_Preview;
class GradientRampLump;
class Hue_Slider;
class Gradient_Slider;
class ColorExample;
class CFillPictureDialog;

class Q_DECL_EXPORT CMDBrush_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit CMDBrush_Widget(QWidget *parent = 0);
    ~CMDBrush_Widget();
    void setBrushManager(CMDBrushManager brMgr);

    CMDBrushManager brushManager()
    {
        updateData(true);
        return m_brushManager;
    }
    ColorExample* getColorExample();

//private:
public:
    CMDBrushManager m_brushManager;
    QColor color() const;

    void resetLayout(CMDBrushManager::BrushType type);
public slots:

    void slotNClicked();
    void slotLClicked();
    void slotSClicked();
    void slotRClicked();
    void slotCClicked();
    void slotPClicked();

    void update_widgets();
    void set_hsv();
    void set_rgb();
    void set_angle();
    void set_stops();
    void set_type(CMDBrushManager::BrushType type);
    void on_edit_hex_editingFinished();
    void on_edit_hex_textEdited(const QString &arg1);
    void setColor(const QColor &clr);
signals:
    void brushTypeChange();
private:
    void update_hex();
    void setColorInternal(const QColor &c);
    void updateData(bool bSave);
    void initialize();
    void release();
    void setUserDefineStyle();
    void setUserDefineContent();
    void setWinLayout();
    void sigAndSlotConnect();
    void setTextLabelInfo();

private:
    QVBoxLayout* p_vLayoutMain;
    QGridLayout* p_gridLayout;
    QHBoxLayout* p_hLayout;
    QHBoxLayout* p_hLayout_btn_brType;
    QVBoxLayout* p_vLayout;
    QHBoxLayout* p_hLayout_btn;
    Color_Wheel* p_color_wheel;
    Color_Preview* p_color_preview;
    GradientRampLump* p_linearBar;
    QLineEdit* p_edit_hex;

    QLabel* p_label_r,*p_label_g,*p_label_b,*p_label_a,*p_label_value,*p_label_hue,*p_label_saturation,*p_label_hex;
    QPushButton* p_btn_null,*p_btn_solid,*p_btn_linearGradient,*p_btn_radialGradient,*p_btn_conicalGradient,*p_btn_fillPicture;
    Hue_Slider* p_slide_hue;
    Gradient_Slider* p_slide_alpha,*p_slide_red,*p_slide_green,*p_slide_blue,*p_slide_saturation,*p_slide_value;
    QSpinBox* p_spin_alpha,*p_spin_red,*p_spin_green,*p_spin_blue,*p_spin_hue,*p_spin_saturation,*p_spin_value;
    QSpinBox* p_spin_angle;
    Gradient_Slider* p_slide_angle;
    QLabel* p_label_angle;
    QList<QSpinBox*> p_spinList;
    QList<Gradient_Slider*> p_sliderList;
    QList<QPushButton*> p_btnList;
    QList<QLabel*> p_labelList;
    QSpacerItem* p_verticalSpacer;
    QSpacerItem* p_hSpacer;
    ColorExample* p_example;

public:
    CFillPictureDialog  *m_picture;
};
#endif // MDCOLOR_WIDGET_H
