#ifndef MDBATCHCHANGEVARIABLE_H
#define MDBATCHCHANGEVARIABLE_H

#include <QDialog>
#include "IMDRTDBManager.h"
namespace Ui {
class CMDBatchChangeVariableDialog;
}

enum batchOption
{
    AddVar = 0,
    ChangeProperties = 1,
};

class CMDBatchChangeVariableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDBatchChangeVariableDialog(QWidget *parent = 0);
    ~CMDBatchChangeVariableDialog();

    int        getBatchVarCnt();

    MDDataType getOriginalDataType();
    MDDataType getProjectDataType();
    QString    getDevName();
    QString    getGroupName();

    QString    getbatchVarName();
    QString    getbatchVarNameStart();
    bool       getvarNameFmt10();

    QString    getbatchVarAddr();
    QString    getbatchVarAddrStart();
    bool       getvarAddrFmt10();

    //!设置Group列表
    void setGroupList(QStringList& groupList);
    //!设置驱动管理器
    void setDriverManager(IMDDriverManager  *driverManager);
    //!设置批量选项 :增加变量+修改属性
    void setBatchOption(batchOption operation);
    void initial(IMDRTDBVariable* curVar);
private slots:
    void on_originalDataTypeBox_currentIndexChanged(int index);
    void on_projectDataTypeBox_currentIndexChanged(int index);
    void on_batchGroupListCombox_currentTextChanged(const QString &arg1);
    void on_batchDeviceListCombox_currentTextChanged(const QString &arg1);

    void onbatchVarNameFmt10clicked();
    void onbatchVarNameFmt16clicked();

    void onbatchVarAddrFmt10clicked();
    void onbatchVarAddrFmt16clicked();

    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::CMDBatchChangeVariableDialog *ui;
    IMDDriverManager  *m_driverManager;

    //!当前变量名+地址
    QString m_curVarName;
    QString m_curVarAddr;

    //!批量添加变量个数
    int        m_batchVarCnt;

    //!批量变量名
    QString    m_batchVarName;
    QString    m_batchVarNameStart;
    bool       m_varNameFmt10;
    //!批量变量地址
    QString    m_batchVarAddr;
    QString    m_batchVarAddrStart;
    bool       m_varAddrFmt10;
    //!批量变量属性
    QString    m_curDevName;
    QString    m_curGroupName;
    MDDataType m_originalDataType;
    MDDataType m_projectDataType;

    QStringList m_groupList;

    batchOption m_batchOption;
};

#endif // MDBATCHCHANGEVARIABLE_H
