#ifndef DBTABLEFIELDDIALOG_H
#define DBTABLEFIELDDIALOG_H

/*!

\author luochao

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

    /*!
    \section 槽函数处理
    */
private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();
    void resField_combox_change(const QString &text);

    /*!
    \section 将枚举类型转换成字符串
    */
private:
    QString dataTypeToString(MDDataType type);
    void initLanguage();

private:
    Ui::DbTableFieldDialog *ui  ; //! 数据表字段UI
    IMDDataField *m_pField      ; //! 字段
    IMDDataGroup *m_pDataGroup  ; //! 数据组
    //ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具
};

#endif // DBTABLEFIELDDIALOG_H
