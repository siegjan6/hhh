/*!

\author eHVr

\section 功能

    多曲线管理设置界面
    功能：添加，删除一条或多条曲线，上下移动曲线位置,
*/
#ifndef CCURVESETTINGFORM_H
#define CCURVESETTINGFORM_H

#include <QWidget>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QMenu>
#include <QKeyEvent>
#include "CDataSource.h"

class CXAxis;
class CYAxis;
class CCurveObjectManager;
class CXAxisManager;
class CYAxisManager;
class CCurveObject;
class CDataProperty;

class IMDRTDBManagerUI;
class IMDDataArchives;
class CPreviewCurveSettingForm;

namespace Ui {
class CCurveSettingForm;
}

class CCurveSettingForm : public QWidget
{
    Q_OBJECT
public:

    explicit CCurveSettingForm(QWidget *parent = 0);
    ~CCurveSettingForm();
private:
    Ui::CCurveSettingForm *ui;

public:
    /*!
     \brief 获取后台数据指针

     \param curves 曲线管理数据
     \param xList  X链表
     \param yList  Y链表
    */
    void setData(CCurveObjectManager * curves,
                 const QList<CXAxis *> &xList, const QList<CYAxis *> &yList);

    void setDataManager(CDataProperty *data);
    void setVariantSettingUI(IMDRTDBManagerUI *variantForm);//! 获取变量设置界面
    void setDataArchives(IMDDataArchives *dataArchives);
    void setXList(const QList<CXAxis *> &xList);
    void setYList(const QList<CYAxis *> &yList);
    void setFieldsList(const QStringList &fieldsList);
    void setIsRealTime(bool isRealTime);
private:
    void setCurveProperty(CCurveObject *, int index);
private:
    bool                m_isRealTime;
    QStringList         m_fields;          //! 数据源字段选择
    CCurveObjectManager *m_curves ;        //! 曲线管理器
    IMDRTDBManagerUI    *m_variantForm ;   //! 变量选择界面
    IMDDataArchives      *m_dataArchives ;

    QList<CXAxis *>      m_xList;
    QList<CYAxis *>      m_yList;
    CDataSource          m_dataSource;
    CDataProperty       *m_dataManager;
    QString              m_filter;
    QList<CCurveObject*> m_preList;        //! 原始曲线链表

public:
    QList<CCurveObject*> m_list;           //! 曲线链表
public:
    void applyChanges();                   //! 应用修改
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
    void initializeCuves();

    void breakConnections();
    void createConnections();     //! 创建信号槽链接
    void parseCurvesInformation();//! 曲线解析

private:
    enum class TableRole{
        Name = 0,
        Variant,
        Comment,
        Visible,
        Color
    };

/*!
\section 槽函数
*/
private slots:
    void slotAddOneCurve();
    void slotAddDozenCurves();
    void slotDeleteOneCurve();
    void slotSetProperty(QTableWidgetItem *);     //! 响应item项的双击事件,设置曲线属性
    void slotSetCurveVisible(QTableWidgetItem *);
    void slotPropertyButtonClicked();             //! 响应属性按钮点击信号,设置当前曲线的属性

    void slotMoveUp();
    void slotMoveDown();
    void slotCustomMenu(const QPoint &p);
private:
    void updatePreviewCurve();
    void removeCurves(const QStringList &itemNames);
    int  findItemByName(const QString &name);
    void swapTableItem(int preIndex,int currentIndex);//! 交换选中行信息
    bool isCurveRepeat(QString str,int index = -1);
    bool isVariantContained(const QString &path, const QString &viewName, const QString &variantName);
    void createMenu();

protected:
    void keyPressEvent(QKeyEvent *event);

public:
    void setPreviewCurveForm(CPreviewCurveSettingForm *previewForm);
    CPreviewCurveSettingForm *previewCurveForm();
private:
    CPreviewCurveSettingForm *m_previewCurveForm;
    QMenu                    *m_menu;              //! 操作菜单

};

#endif // CCURVESETTINGFORM_H
