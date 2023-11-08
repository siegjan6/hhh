/*!

\author eHVr

\section 功能

    曲线设置总窗体
*/
#ifndef MDCURVESETTINGDIALOG_H
#define MDCURVESETTINGDIALOG_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class MDCurveSettingDialog;
}
class QListWidgetItem;

class IDrawEnvrionment;
class CCurveWidget;
class IMDRTDBManagerUI;
class IMDDataArchives;
class CCurveProperty;
class CAppearanceSettingForm;
class CCurveSettingForm;
class CDrawModeSettingForm;
class CVernierSettingForm;
class CXAxisSettingForm;
class CYAxisSettingForm;
class CFeatureTagSettingForm;
class CReferenceCurveSettingForm;
class CPreviewCurveSettingForm;
class CToolsSettingForm;
class CPageSettingForm;
class CCurveStisticalForm;

class ITranslateWidgetTool;
class MDCurveSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MDCurveSettingDialog(QWidget *parent = 0);
    ~MDCurveSettingDialog();
/*!
\section
*/
public:
    void setData(CCurveProperty *curve,IDrawEnvrionment *environment, bool isRealTime);
    void setWidget(CCurveWidget *widget);
    void setVariantSettingUI(IMDRTDBManagerUI *variantUI);
    void setDataArchives(IMDDataArchives *dataArchives);

/*!
\section 初始化
*/
private:
    void initialize();
    void addItems();
    void addWidgets();   //! 添加窗体界面
    void setItemsData(); //! 设置每个项的数据

/*!
\section 窗体编辑标志位
*/
public:
    void resetModified();
signals:
    void itemModified();
private slots:
    void slotHandleModified();
/*!
\section 窗体编辑标志位
*/
protected:
    void closeEvent(QCloseEvent *e);
/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();
private slots:
    void slotShowWidget(QListWidgetItem *item);
    void slotApplyChanges();
    void slotConfirmButtonClicked();
    void slotCancelButtonClicked();

private:
    enum class WidgetName{
        Appearance = 0,
        Xaxis,
        Yaxis,
        Curve,
        ReferenceCurve,
        PreviewCurve,
        DrawMode,
        Vernier,
        FeatureTag,
        ToolsSetting,
        PageSetting,
        CurveStistical
    };
    void insertItem(const QString &name,int userRole, bool visible = true);

    void setIsRealTime(bool isRealTime);//! 是否为实时曲线
    void applyChanges();                //! 应用内容修改
private:
    Ui::MDCurveSettingDialog *ui;
    bool  m_isModified;
    bool m_isRealTime;

    CCurveWidget                    *m_widget ;
    CCurveProperty                  *m_realCurve ;         //! 后台数据
    IDrawEnvrionment                *m_environment ;
    CAppearanceSettingForm          *m_apperanceForm ;     //! 外观设置界面
    CCurveSettingForm               *m_curvesForm ;        //! 曲线设置界面
    CDrawModeSettingForm            *m_drawModeForm ;      //! 绘制模式设置界面
    CVernierSettingForm             *m_vernierForm ;       //! 游标设置界面
    CXAxisSettingForm               *m_xAxisForm ;         //! X轴设置界面
    CYAxisSettingForm               *m_yAxisForm ;         //! Y轴设置界面
    CFeatureTagSettingForm          *m_featureTagForm ;    //! 特征标签设置界面
    CReferenceCurveSettingForm      *m_referenceCurveForm ;//! 基准曲线设置界面
    CPreviewCurveSettingForm        *m_previewCurveForm;   //! 曲线预览设置界面
    CToolsSettingForm               *m_toolsSettingForm ;  //! 工具设置界面
    CPageSettingForm                *m_pageSettingForm ;   //! 页面设置界面
    CCurveStisticalForm             *m_curveSitsticalForm ;//! 曲线统计设置界面

    ITranslateWidgetTool            *m_translateTool;
};

#endif // MDCURVESETTINGDIALOG_H
