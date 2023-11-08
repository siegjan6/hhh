#ifndef MDPENSELECTDIALOG_H
#define MDPENSELECTDIALOG_H

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
#include "MDPenManager.h"

#if defined(MDCOLORMANAGER_LIBRARY)
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

class CMDBrush_Widget;

/*
 * class: CMDPenSelectDialog
 * biref: 生成CMDPenManager的配置　对话框
 * 平均功能
        |——对话框QPenSelectDialog 配置结束，根据QDialog的返回结果　赋值CMDPenManager
 */
class MDColorMANAGERSHARED_EXPORT CMDPenSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMDPenSelectDialog(QWidget *parent = 0);
    ~CMDPenSelectDialog();
    CMDPenManager penManager();
    void setPenManager(CMDPenManager penMgr);

    CMDBrushManager brushManager();
    void setBrushManager(CMDBrushManager brushMgr);
public slots:
    void slotOkClicked();
    void slotCancelClicked();
    void slotComboBoxCurrentIndexChange_style(int index);
    void slotComboBoxCurrentIndexChange_capStyle(int index);
    void slotComboBoxCurrentIndexChange_joinStyle(int index);
    void slotSliderWidthChange(int val);
private:
    void initialize();
    void release();
    void setUserDefineStyle();//设置大小风格样式
    void setWinLayout();//　布局
    void sigAndSlotConnect();//信号和槽链接
    void setTextLabelInfo();
    void initComboBoxInit();
    void updateData(bool bSave);
private:
    QSpinBox *p_slider_width;
    QComboBox *p_comboBox_style;
    QComboBox * p_comboBox_joinStyle;
    QComboBox * p_comboBox_capStyle;
//    CMDBrush_Widget* p_widgetBrush;
    QPushButton* p_btn_ok, *p_btn_cancel;
    QVBoxLayout* p_vLayoutMain;
    QHBoxLayout* p_hLayoutBtn;
    QHBoxLayout* p_hLayoutComboBox;
    QLabel *p_label_penWidth;
    QLabel *p_label_penStyle;
    QLabel *p_label_penCapStyle;
    QLabel *p_label_penJoinStyle;
    QSpacerItem *p_horizontalSpacer;
    //
public:
      CMDBrush_Widget* p_widgetBrush;

private:
    CMDPenManager m_penManager;

    QLineEdit* m_edit;
};

#endif // MDPENSELECTDIALOG_H
