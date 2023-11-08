/*!
\author enli 2015.01.08
\section 变量基本属性框 call by CVariablePropertyDialog
*/
#ifndef VARIABLEGENERALDIALOG_H
#define VARIABLEGENERALDIALOG_H

#include <QDialog>
#include "IMDRTDBManager.h"
namespace Ui {
class CVariableGeneralDialog;
}

//class CMDSelectVariableWidget;
class CVariableGeneralDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CVariableGeneralDialog(QWidget *parent = 0);
    ~CVariableGeneralDialog();
    //!验证变量地址
    bool checkVarAddress();
private slots:
    void onNameEditTextChanged(const QString &arg1);
    void onPrecisionEditTextChanged(const QString &arg1);
    void onAddressEditTextChanged(const QString &arg1);
    void onDataTypeBoxCurrentIndexChanged(int index);
    void onOriginalDataTypeBoxCurrentIndexChanged(int index);
    void onVariableTypeBoxCurrentIndexChanged(int index);
    void onDeviceBoxCurrentIndexChanged(const QString &arg1);
	void onUnitEditTextChanged();
	void onDescriptionEditTextChanged();
    void onExtEditTextChanged();
    void onAddressButtonClicked();
    bool eventFilter(QObject *,QEvent *);
    void onAddressTableWidgetCellDoubleClicked(int row, int column);
    void onAddressTableWidgetCellClicked(int row, int column);
    void onAddressEditFinished();
private:
    Ui::CVariableGeneralDialog *ui;
public:
   void setRTDBManager (IMDRTDBManager  *rtdbManager);
   void initial(IMDRTDBVariable *curRTDBVariable);
   void updateContols();
   void setRunModel(bool isRunning);
   void setCurrentVariableGroup(IMDVariableGroup *varGroup);
private:
	IMDVariableGroup *m_curVariableGroup;
   IMDDriverManager  *m_driverManager;
   IMDRTDBManager  *m_rtdbManager;
   bool m_isRunning;
   //!IO变量时，可选的设备个数
   int    m_deviceCnt;
public:
   QString  m_name ;
   QString  m_device ;
   QString  m_address ;
   QString m_unit;
   QString m_description;
   QString m_ext;
   int m_precision;
   //!工程值类型
   MDDataType m_dataType;
   //!原始值数据类型
   MDDataType m_originalDataType;
   //!变量类型：IO,中间,虚拟,系统
   MDVariableType m_variableType;
   //CMDSelectVariableWidget* m_dlg;
   //bool m_setAddress;
};

#endif // VARIABLEGENERALDIALOG_H
