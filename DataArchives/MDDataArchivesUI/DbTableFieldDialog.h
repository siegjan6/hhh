#ifndef DBTABLEFIELDDIALOG_H
#define DBTABLEFIELDDIALOG_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据表字段界面。

*/

#include <QDialog>
#include "../../include/interface/Common/IMDDataType.h"

class IMDDataField;
class IMDDataGroup;
class ITranslateWidgetTool;

namespace Ui {
class DbTableFieldDialog;
}

class CDbTableFieldDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDbTableFieldDialog(QWidget *parent = 0);
    ~CDbTableFieldDialog();

    /*!
    \section 初始化属性
    */
public:

    void initUI(IMDDataField *pField, IMDDataGroup *pGrp);
    void changeLanguage();
    // addOrModifyItem 0 - add, 1 - modify
    void setEditFlag(int addOrModifyItem = 0);

    /*!
    \section 槽函数处理
    */
private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();
    void resField_combox_change(const QString &text);

    /*!
    \section 根据绑定的表模板变量名同步其他信息
    */
    void on_SyncButton_clicked();

private:
    QString dataTypeToString(MDDataType type);
    void initLanguage();

private:
    Ui::DbTableFieldDialog *ui  ; //! 数据表字段UI
    IMDDataField *m_pField      ; //! 字段
    IMDDataGroup *m_pDataGroup  ; //! 数据组
    int m_addOrModifyItem       ; //! 新建编辑标识
    ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具
};

#endif // DBTABLEFIELDDIALOG_H
