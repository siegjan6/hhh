/*!

\author eHVr

\section 功能

    绘制模式设置界面,对绘制方向，刷新时间进行设置
*/
#ifndef CDRAWMODESETTINGFORM_H
#define CDRAWMODESETTINGFORM_H

#include <QWidget>

class CDrawMode;

namespace Ui {
class CDrawModeSettingForm;
}

class CDrawModeSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CDrawModeSettingForm(QWidget *parent = 0);
    ~CDrawModeSettingForm();
public:
    void setData(CDrawMode *drawMode);//! 设置后台数据指针
private:
    CDrawMode *m_drawMode; //! 绘制模式

public:
    void applyChanges();  //! 应用内容修改
/*!
\section 窗体编辑标志
*/
public:
    void resetModified();
private slots:
    void slotHandleModified();
signals:
    void itemModified();
private:
    bool m_isModified;
/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();
/*!
\section 初始化
*/
private:
    void initialize();
    void initializeUI();
private:
    Ui::CDrawModeSettingForm *ui;   
};

#endif // CDRAWMODESETTINGFORM_H
