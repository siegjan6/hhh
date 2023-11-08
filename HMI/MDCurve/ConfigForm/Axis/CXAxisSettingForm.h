/*!

\author eHVr

\section 功能
    多X轴设置界面
*/
#ifndef CXAXISSETTINGFORM_H
#define CXAXISSETTINGFORM_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include "CXAxis.h"

class CXAxisManager;
class CGridRegion;
class CBisectrixLine;
class CCurveSettingForm;
class CReferenceCurveSettingForm;
class CFeatureTagSettingForm;

namespace Ui {
class CXAxisSettingForm;
}

class CXAxisSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CXAxisSettingForm(QWidget *parent = 0);
    ~CXAxisSettingForm();
private:
    Ui::CXAxisSettingForm *ui;

public:
    void setData(CXAxisManager *xAxisManager,CGridRegion *gridRegion);

    QList<CXAxis *> list() const;
    void setList(const QList<CXAxis *> &list);

    //! 曲线设置窗体指针

    void setCurveForm(CCurveSettingForm *curveForm);
    CCurveSettingForm *curveForm() const;
    void setReferenceForm(CReferenceCurveSettingForm *reCurveForm);
    CReferenceCurveSettingForm *referenceForm() const;
    void setFeatureTagForm(CFeatureTagSettingForm *featureTagForm);
    CFeatureTagSettingForm *featureTagForm() const;

private:

    CXAxisManager                *m_xAxisManager ;
    CGridRegion                  *m_gridRegion ;
    QList<CXAxis*>                m_list;
    CCurveSettingForm            *m_curveForm ;
    CReferenceCurveSettingForm   *m_reCurveForm;
    CFeatureTagSettingForm       *m_featureTagForm;

    QList<CXAxis*>                m_xAxisPrelist;

/*!
\section 应用内容修改
*/
public:
    void applyChanges();
private:
    void applyXAxisChanges();
    void applyGridRegionChanges();
/*!
\section 窗体编辑标志位
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
\section  初始化
*/
private:
    void initialize();
    void initializeUI();
    void initializeXAxis();
    void initializeGridRegion();
/*!
\section 信号槽
*/
private:
    void createConnections();
private slots:
    void slotSetMainBisectrix();      //! 设置主等分线
    void slotSetSecondaryBisectrix(); //! 设置次等分线

    void slotAddAxis();               //! 增加轴线
    void slotDeleteAxis();            //! 删除轴线

    void slotPropertyButtonClicked();
    void slotSetProperty(QTableWidgetItem *);             //! 响应tableWidget的双击事件,设置属性
    void slotSetNoneCurvesBoundVisible(bool boundVisible);//! 是否显示无曲线绑定的轴

    void slotMoveUpRow();             //! 上移
    void slotMoveDownRow();           //! 下移
    void slotCustomMenu(const QPoint &p);

private:
    void setAxisProperty(CXAxis *);   //! 设置x轴属性
    void updateCurveAxis();           //! 更新曲线
    void parseAxisInformation();      //! 解析X轴信息,填充到ui中
    void createMenu();                //! 创建菜单

    QString createName(const QStringList &nameList);
    QString transEnumToString(CXAxis::ValueType valueType);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    CBisectrixLine          *m_preMainlBisectrixLine ;        //! 原始主等分线
    CBisectrixLine          *m_preSecontraryBisectrixLine ;   //! 原始次等分线
    bool                     m_autoHideAxis ;                 //! 是否自动隐藏轴线
    CBisectrixLine          *m_mainBisectrixLine ;            //! 主等分线
    CBisectrixLine          *m_secontraryBisectrixLine ;      //! 次等分线
    QMenu                   *m_menu;                          //! 操作菜单

private:
    QList<bool>m_xBoundVisibleList;                //! x轴线是否绑定及绑定曲线是否可见
};

#endif // CXAXISSETTINGFORM_H
