#ifndef MDDATASRCTABWIDGET_H
#define MDDATASRCTABWIDGET_H

/*!

\author bHVvY2hhbw==
1.数据源配置界面。

*/

#include <QDialog>

#define DATASRC_WIDGET_INIT_W                                       850
#define DATASRC_WIDGET_INIT_H                                       460
#define SRC_FIXED_W                                                 200
#define SRC_LAYOUT_SPACE                                            15

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QCheckBox;
class QTableView;
class IMDDataTable;
class CDataArchivesTabModel;
class IMDDataArchives;
class IMDDataField;
class CDBStudio;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;
class CDataArchivesUI;
class ITranslateWidgetTool;

class CMDDataSrcTabWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CMDDataSrcTabWidget(QWidget *parent = 0);
    ~CMDDataSrcTabWidget();

    /*!
    \section 初始化属性
    */
public:
    inline void setDataArchives(IMDDataArchives *pDataArchives){m_pDataArchives = pDataArchives;}
    void initData(IMDDataTable *pTable, const QString &grpName, const QString &dataSrcName);
    inline void setDataArchivesUI(CDataArchivesUI *pParent){m_pDataArchivesUI = pParent;}
    void changeLanguage();

    /*!
    \section 存储
    */
public:
    void save();

    /*!
    \section 初始化界面
    */
private:
    void initData();
    void initUI();
    void initLanguage();
    void signalsSotConnect();
    void initTableView();
    void addDefaultField();
    void removeDefaultField();
    int calculateLabelWidth(QLabel *pLb);

signals:

    /*!
    \section 槽函数处理
    */
private slots:
    void on_tableName_editingFinished();
    void on_tableDescri_editingFinished();    
    void on_AddField_clicked();
    void on_tabView_doubleClicked(const QModelIndex &index);
    void on_removeField_clicked();
    void on_editField_clicked();
    void check_stateChange();
    void on_name_textchange(const QString &text);

private:
    bool m_iDefaultFlag                          ;  //! 启用默认字段标识
    QLineEdit *m_pLineTableName                  ;   //! 表名编辑框
    QLineEdit *m_pLineDescri                     ;   //! 描述编辑框
    QLineEdit *m_pLineDataGrp                    ;   //! 数据组编辑框
    QPushButton *m_pBtnCheckDefault              ;   //! 启用默认字段按钮
    QPushButton *m_pBtnAddField                  ;   //! 增加字段按钮
    QPushButton *m_pBtnRemoveField               ;   //! 删除字段按钮
    QPushButton *m_pBtnEditField                 ;   //! 编辑字段按钮
    QLabel *m_pLbDataSrc                         ;   //! 数据源标签
    QLineEdit *m_pLineSrcName                    ;   //! 数据源名编辑框
    QLabel *m_pLbDescri                          ;   //! 描述标签
    QLabel *m_pLbTableName                       ;   //! 表名标签
    QLabel *m_pLbDataGrpName                     ;   //! 数据组标签
    QLabel *m_pLbTabDescrip                      ;   //! 数据表描述
    QCheckBox * m_pCbEnable                      ;   //! 总使能
    QTableView *m_pTabView                       ;   //! tableview
    QGroupBox *m_pGrpboxTab                      ;   //! goupbox
    CDataArchivesTabModel *m_pTabViewModel       ;   //! 表模型
    IMDDataTable *m_pDataTable                   ;   //! 表
    IMDDataArchives *m_pDataArchives             ;   //! 数据归档
    CDataArchivesUI *m_pDataArchivesUI           ;   //! 数据归档窗体
    QHBoxLayout *m_pFirstLayout                  ;   //! 数据源水平布局
    QHBoxLayout *m_pSecondLayout                 ;   //! 表模板水平布局
    QHBoxLayout *m_pThirdLayout                  ;   //! 数据表水平布局
    QHBoxLayout *m_pFourthLayout                 ;   //! 描述水平布局
    QHBoxLayout *m_pFifthLayout                  ;   //! 操作水平布局
    QHBoxLayout *m_p6thLayout                    ;   //! 使能水平布局
    QVBoxLayout *m_pMainLayout                   ;   //! 整体垂直布局
    ITranslateWidgetTool *m_pMulLnaguageTool     ; //! 多语言翻译工具
};

#endif // MDDATASRCTABWIDGET_H
