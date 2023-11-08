#ifndef DBTABLEDIALOG_H
#define DBTABLEDIALOG_H

/*!

\author luochao

\section 功能
1.负责新建数据表窗体。

*/

#include <QDialog>
#include <QVariant>
#include <interface/Common/IMDDataType.h>

#define TABLE_FIXED_W                                               200
#define TABLE_LAYOUT_SPACE                                          30

class IMDDataGroupManager;
class IMDDataTable;
class QLabel;
class QComboBox;
class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class IMDDataSource;
class IMDDataField;
class ITranslateWidgetTool;

class CDBTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDBTableDialog(QWidget *parent = 0);
    ~CDBTableDialog();

    /*!
    \section 初始化属性
    */
public:
    void initData(IMDDataSource *pDBSource, IMDDataTable *pTable, IMDDataGroupManager *pGrpManager);
    void changeLanguage();

    /*!
    \section 槽函数处理
    */
private slots:
    void on_OkButton_clicked();

    void on_CancelButton_clicked();

    /*!
    \section 界面创建以及初始化
    */
private:
    void initData();
    void initUI();
    void initLanguage();
    int calculateLabelWidth(QLabel *pLb);

    void initDataGrpNameList(IMDDataGroupManager *pGrpManager);
    IMDDataField *createField(const QString &description, const QString &name, MDDataType type);
    int calCurrTypeSize(MDDataType type);

private:
    QPushButton *m_pBtnOk                       ;  //! 确定按钮
    QPushButton *m_pBtnCancel                   ;  //! 取消按钮
    QLabel *m_pLbGrpName                        ;  //! 数据组名标签
    QLabel *m_pLbTabName                        ;  //! 数据表标签
    QLabel *m_pLbDescri                         ;  //! 描述标签
    QComboBox *m_pCombDataGrp                   ;  //! 数据组下拉框
    QLineEdit *m_pLineTabName                   ;  //! 数据表名编辑框
    QLineEdit *m_pLineDescri                    ;  //! 描述编辑框

    QHBoxLayout *m_pHFirstLayout                ;  //! 数据组水平布局
    QHBoxLayout *m_pHSecondLayout               ;  //! 数据表名水平布局
    QHBoxLayout *m_pHThirdLayout                ;  //! 描述水平布局
    QHBoxLayout *m_pHFourthLayout               ;  //! 操作按钮水平布局
    QVBoxLayout *m_pVMainLayout                 ;  //! 整体垂直布局

    IMDDataTable *m_pDataTable                  ;  //! 数据表
    IMDDataSource *m_pDBSource                  ;  //! 数据源
    IMDDataGroupManager *m_pDataGrpManager      ;  //! 数据组管理器
};

#endif // DBTABLEDIALOG_H
