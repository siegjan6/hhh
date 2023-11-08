/*!

\author eHVr

\section 功能

    游标属性设置界面
*/
#ifndef CVERNIERSETTINGFORM_H
#define CVERNIERSETTINGFORM_H

#include <QWidget>

namespace Ui {
class CVernierSettingForm;
}

class CVernier;
class CVernierSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CVernierSettingForm(QWidget *parent = 0);
    ~CVernierSettingForm();
public:
    void setData(CVernier *vernier);
    void applyChanges();//! 应用修改内容
/*!
\section 窗体内容是否修改
*/
public:
    void resetModified();
private slots:
    void slotHandleModified();
signals:
    void itemModified();

/*!
\section  初始化窗体数据
*/
private:
    void initialize();
    void initializeData();
    void initializeUI();
/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();
private slots:
    void slotSetVernierFont();
    void slotSetFloatPoint(bool);
    void slotSetCustomString(bool);
private:
    Ui::CVernierSettingForm *ui;
    bool m_isModified;

    CVernier                *m_vernier;         //! 数据指针

    QFont                   m_vernierFont;      //! 字体
    bool                    m_visible ;         //! 显示
    bool                    m_isVernierFixed;   //! 是否固定显示
    bool                    m_isChoseNearer;    //! 是否显示最近点
    bool                    m_isVertical;       //! 是否垂直排列

    QPoint                  m_fixedPoint;       //! 固定点坐标
    QString                 m_customString;     //! 自定义显示内容
    QString                 m_timeFormat;       //! 时间格式
    int                     m_itemInternal ;    //! 各项间隔
    int                     m_overTimeLength;   //! 超出时间长度


};

#endif // CVERNIERSETTINGFORM_H
