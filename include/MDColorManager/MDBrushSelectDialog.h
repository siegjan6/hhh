#ifndef MDCOLOR_WIDGET_H
#define MDCOLOR_WIDGET_H
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
class CMDBrush_Widget;


#if defined(MDCOLORMANAGER_LIBRARY)
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDColorMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

/*
 * class: CMDBrushSelectDialog
 * biref: 生成CMDBrushManager的配置　对话框
 * 平均功能
        |——对话框CMDBrushSelectDialog 配置结束，根据QDialog的返回结果　赋值CMDBrushManager
 */
class MDColorMANAGERSHARED_EXPORT CMDBrushSelectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CMDBrushSelectDialog(QWidget *parent = 0);
    ~CMDBrushSelectDialog();

    CMDBrushManager brushManager();
    void setBrushManager(CMDBrushManager brushMgr);
public slots:
    void slotOkClicked();
    void slotCancelClicked();
private:
    void initialize();
    void release();
    void setUserDefineStyle();//设置大小风格样式
    void setWinLayout();//　布局
    void sigAndSlotConnect();//信号和槽链接
    void setTextLabelInfo();
//private:
public:
    CMDBrush_Widget* p_widgetBrush;
    QPushButton* p_btn_ok, *p_btn_cancel;
    QVBoxLayout* p_vLayoutMain;
    QHBoxLayout* p_hLayoutBtn;
};
#endif // MDCOLOR_WIDGET_H
