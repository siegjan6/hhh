﻿#ifndef MDNEWDATAGROUP_H
#define MDNEWDATAGROUP_H

/*!

\author bHVvY2hhbw==

\section 功能
1.新建数据组窗体。

*/

#include <QDialog>

#define GROUP_FIXED_W                                               200
#define GROUP_LAYOUT_SPACE                                          30

class IMDDataGroup;
class QLabel;
class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class IMDDataGroupManager;
class ITranslateWidgetTool;

class CMDNewDataGroup : public QDialog
{
    Q_OBJECT

public:
    explicit CMDNewDataGroup(QWidget *parent = 0);
    ~CMDNewDataGroup();

    /*!
    \section 初始化属性
    */
public:
    void initData(IMDDataGroup *pDataGrp, IMDDataGroupManager *pDataGrpMana);
    void changeLanguage();

    /*!
    \section 槽函数处理
    */
private slots:
    void on_OkButton_clicked();

    void on_CancelButton_clicked();

    /*!
    \section 初始化界面
    */
private:
    void initData();
    void initUI();
    void initLanguage();
    int calculateLabelWidth(QLabel *pLb);

private:
    QPushButton *m_pBtnOk                   ; //! 确定按钮
    QPushButton *m_pBtnCancel               ; //! 取消按钮
    QLabel *m_pLbGrpName                    ; //! 数据组名标签
    QLabel *m_pLbDescri                     ; //! 数据组描述标签
    QLineEdit *m_pLineGrpName               ; //! 数据组名编辑框
    QLineEdit *m_pLineDescri                ; //! 数据组描述编辑框

    QHBoxLayout *m_pHFirstLayout            ; //! 数据组名水平布局
    QHBoxLayout *m_pHSecondLayout           ; //! 描述水平布局
    QHBoxLayout *m_pHThirdLayout            ; //! 操作水平布局
    QVBoxLayout *m_pVMainLayout             ; //! 整体垂直布局

    IMDDataGroup *m_pDataGrp                ; //! 数据组
    IMDDataGroupManager *m_pDataGrpMana     ; //! 数据组管理器
    ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具
};

#endif // MDNEWDATAGROUP_H
