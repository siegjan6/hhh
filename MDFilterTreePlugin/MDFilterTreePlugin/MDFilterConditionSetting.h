#ifndef MDFILTERCONDITIONSETTING_H
#define MDFILTERCONDITIONSETTING_H

/*!

\author bHVvY2hhbw==

\section 功能
1.过滤条件值设置界面。

*/

#include <QDialog>
#include <QLineEdit>
#include <QDateTimeEdit>

#define FILTER_SETTING_SPACE                20
#define FILTER_SETTING_W                    135
#define FILTER_LINEEDIT_W                   60

class QLabel;
class QPushButton;
class QComboBox;
class QVBoxLayout;
class QHBoxLayout;
class QRadioButton;

class ITranslateWidgetTool;
class CMDFilterTreeDataItem;

class CMDFilterConditionSetting : public QDialog
{
    Q_OBJECT
public:
    explicit CMDFilterConditionSetting(QWidget *parent = 0);
    ~CMDFilterConditionSetting();

    /*!
    \section 外部调用接口
    */
public:
    //! 设置当前的属性信息
    void setPropertyInfo(CMDFilterTreeDataItem *pFilterItem);

    void changeLanguage();

signals:

    /*!
    \section 槽函数
    */
private slots:
    void conditionComboBoxIndexChange(const QString &text);
    void okBtnClicked();

    void staticRadioBtnClicked(bool checked);
    void dynamicRadioBtnClicked(bool checked);

    /*!
    \section 初始化界面
    */
private:
    void createUI();
    int calculateLabelWidth(QLabel *pLb);
    void initData();

    void changeCurrentOperatorIndex(const QString &oper);  //! 切换当前条件关系
    void setDateTimeEditVisible(bool iFlag);  //! 设置日期时间编辑界面是否可视

private:

    QLabel *m_pLbCondition          ; //! 条件标签
    QLabel *m_pLbValue              ; //! 值标签
    QLabel *m_pLbHint               ; //! 提示标签
    QComboBox *m_pCombCondi         ; //! 条件关系下拉框
    QLineEdit *m_pLineValue         ; //! 值编辑框
    QLineEdit *m_pLineHint          ; //! 提示编辑框

    QPushButton *m_pBtnOk           ; //! 确定按钮
    QPushButton *m_pBtnCancel       ; //! 取消按钮

    QDateTimeEdit *m_pDateEdit      ; //! 日期编辑界面
    QDateTimeEdit *m_pTimeEdit      ; //! 时间编辑界面

    QRadioButton *m_pRdoStatic      ; //! 静态选择
    QRadioButton *m_pRdoDynamic     ; //! 动态选择

    QHBoxLayout *m_pHFirstLayout    ; //! 水平布局
    QHBoxLayout *m_pHSecondLayout   ; //! 水平布局
    QHBoxLayout *m_pHThirdLayout    ; //! 水平布局
    QHBoxLayout *m_pHFourthLayout   ; //! 水平布局
    QHBoxLayout *m_pHFifthLayout    ; //! 水平布局
    QVBoxLayout *m_pVMainLayout     ; //! 主垂直布局

    QStringList m_operatorList;             //! 条件关系列表
    CMDFilterTreeDataItem *m_pFilterItem; //! 当前的属性信息

    ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具

};

#endif // MDFILTERCONDITIONSETTING_H
