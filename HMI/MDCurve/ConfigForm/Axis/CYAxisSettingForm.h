/*!

\author eHVr

\section 功能

多Y轴管理设置界面

*/
#ifndef CYAXISSETTINGFORM_H
#define CYAXISSETTINGFORM_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>

class CYAxisManager;
class CGridRegion;
class CBisectrixLine;
class CYAxis;
class CCurveSettingForm;
class CReferenceCurveSettingForm;

namespace Ui {
class CYAxisSettingForm;
}

class CYAxisSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CYAxisSettingForm(QWidget *parent = 0);
    ~CYAxisSettingForm();

public:
    void setData(CYAxisManager *yAxisManager,CGridRegion *gridRegion);

    QList<CYAxis *> list() const;
    void setList(const QList<CYAxis *> &list);

    CCurveSettingForm *curveForm() const;
    void setCurveForm(CCurveSettingForm *curveForm);
    CReferenceCurveSettingForm *referenceForm() const;
    void setReferenceForm(CReferenceCurveSettingForm   *reCurveForm);
private:
    CYAxisManager                *m_yAxisManager ;
    CGridRegion                  *m_gridRegion ;
    QList<CYAxis *>               m_list;
    CCurveSettingForm            *m_curveForm ;
    CReferenceCurveSettingForm   *m_reCurveForm;

    QList<CYAxis*>                m_preList;
/*!
\section 应用内容修改
*/
public:
    void applyChanges();
private:
    void applyYAxisChanges();
    void applyGridRegionChanges();

protected:
    void keyPressEvent(QKeyEvent *event);

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
\section 初始化
*/
private:
    void initialize();
    void initializeUI();
    void initializeYAxis();
    void initializeGridRegion();
/*!
\section 信号槽
*/
private:
    void createConnections();
private slots:
    void slotSetMainBisectrix();             //! 设置主等分线
    void slotSetSecondaryBisectrix();        //! 设置次等分线
    void slotAddAxis();                      //! 增加
    void slotDeleteAxis();                   //! 删除
    void slotMoveUp();                    //! 右移
    void slotMoveDown();                     //! 左移
    void slotPropertyButtonClicked();        //! 响应属性按钮点击信号,设置当前选中轴的属性
    void slotSetProperty(QTableWidgetItem *);//! 响应表格的双击信号,设置当前选中轴的属性
    void slotSetNoneCurvesBoundVisible(bool);//! 是否显示无曲线绑定的轴
    void slotCustomMenu(const QPoint &p);

private:
    void updateCurveAxis();
    void parseAxisInformation();              //! 解析Y轴数据,填充到表格控件
    void setAxisProperty(CYAxis *y);          //! 设置给定轴的属性
    QString createName(const QStringList &nameList);
    void createMenu();

private:
    Ui::CYAxisSettingForm   *ui;
    bool             m_autoHideAxis;                  //! 是否自动隐藏
    CBisectrixLine  *m_preMainlBisectrixLine ;        //! 原始主等分线
    CBisectrixLine  *m_preSecontraryBisectrixLine ;   //! 原始次等分线
    CBisectrixLine  *m_mainBisectrixLine ;            //! 主等分线
    CBisectrixLine  *m_secontraryBisectrixLine ;      //! 次等分线
    QMenu           *m_menu;                          //! 操作菜单

private:
    QList<bool>m_yBoundVisibleList;         //! y轴线是否绑定及绑定曲线是否可见
};

#endif // CYAXISSETTINGFORM_H
