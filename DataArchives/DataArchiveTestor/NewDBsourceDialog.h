#ifndef NEWDBSOURCEDIALOG_H
#define NEWDBSOURCEDIALOG_H

/*!

\author luochao

\section 功能
1.数据源创建界面。

*/

#include <QDialog>
#include <QMessageBox>
//#include "IMDVariableManager.h"
#include "../../include/interface/Common/IMDDataType.h"

namespace Ui {
class NewDBsourceDialog;
}

class IMDDataSource;
class IMDDataSourceManager;
class IMDDataTable;
class IMDDataArchives;
class IMDDataGroup;
class IMDDataField;
class ITranslateWidgetTool;

class CNewDBsourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CNewDBsourceDialog(QWidget *parent = 0);
    ~CNewDBsourceDialog();

    /*!
    \section 初始化属性
    */
public:
    void initUI(IMDDataSource *pDataSource, IMDDataArchives *pDataArchives);
    void changeLanguage();

    /*!
    \section 创建默认表
    */
private:
    void DefaultSetting();
    void Alm_DefaultSetting();
    void Log_DefaultSetting();
    void His_DefaultSetting();

    int calCurrTypeSize(MDDataType type);
    void createTable(IMDDataGroup *pDataGrp);
    IMDDataField *createField(const QString &description, const QString &name, MDDataType type);

    /*!
    \section 槽函数处理
    */
private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();
    void onCheckStateChange(int state);

private:
    Ui::NewDBsourceDialog *ui         ; //! 创建数据源UI
    IMDDataSource *m_pMDDataSrc       ; //! 数据源
    IMDDataArchives *m_pDataArchives  ; //! 数据归档
};

#endif // NEWDBSOURCEDIALOG_H
