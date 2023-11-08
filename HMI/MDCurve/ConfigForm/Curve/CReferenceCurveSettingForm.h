#ifndef CREFERENCECURVESETTINGFORM_H
#define CREFERENCECURVESETTINGFORM_H

#include <QWidget>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QMenu>
#include <QKeyEvent>
class CXAxis;
class CYAxis;
class CXAxisManager;
class CYAxisManager;
class CReferenceCurveManager;
class CReferenceCurve;
class IMDRTDBManagerUI;

namespace Ui {
class CReferenceCurveSettingForm;
}

class CReferenceCurveSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CReferenceCurveSettingForm(QWidget *parent = 0);
    ~CReferenceCurveSettingForm();

private:
    Ui::CReferenceCurveSettingForm *ui;

public:
    /*!
     \brief 获取后台数据指针

     \param reCurves 基准曲线管理数据
     \param xList  X链表
     \param yList  Y链表
    */
    void setData(CReferenceCurveManager * reCurves,
                 const QList<CXAxis *> &xList, const QList<CYAxis *> &yList);
    void setXList(const QList<CXAxis *> &xList);
    void setYList(const QList<CYAxis *> &yList);
    void setVariantSettingUI(IMDRTDBManagerUI *variantForm);//! 获取变量设置界面
private:
    void setReCurveProperty(CReferenceCurve *);

private:
    CReferenceCurveManager *m_reCurves ; //! 基准曲线管理
    QList<CReferenceCurve*> m_rePreList; //! 原始基准曲线链表
    QList<CXAxis *>         m_xList;
    QList<CYAxis *>         m_yList;
    IMDRTDBManagerUI        *m_variantForm ; //! 变量选择界面
    QMenu                   *m_menu;         //! 操作菜单
public:
    QList<CReferenceCurve*> m_reList;    //! 基准曲线链表

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

public:
    void applyChanges();             //! 应用修改

/*!
\section 初始化
*/
private:
    void initialize();
    void initializeUI();
    void initializeReCuves();
    void breakConnections();
    void createConnections();        //! 创建信号槽链接
    void parseReCurvesInformation(); //! 基准曲线解析

private:
    enum class TableRole{
        Color = 0,
        Name
    };

    /*!
    \section 槽函数
    */
private slots:
    void slotAddReCurve();
    void slotDeleteReCurve();
    void slotSetProperty(QTableWidgetItem *item); //! 响应item项的双击事件,设置曲线属性
    void slotPropertyButtonClicked();             //! 响应属性按钮点击信号,设置当前曲线的属性
    void slotMoveUp();
    void slotMoveDown();
    void slotCustomMenu(const QPoint &p);
private:
    void removeCurves(const QStringList &itemNames);
    int  findItemByName(const QString &name);
    void swapTableItem(int preIndex,int currentIndex);//! 交换选中行信息
    QString createName(const QStringList &nameList);
    void createMenu();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // CREFERENCECURVESETTINGFORM_H
