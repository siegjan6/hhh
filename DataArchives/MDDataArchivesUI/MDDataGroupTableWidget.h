#ifndef MDDATAGROUPTABLEWIDGET_H
#define MDDATAGROUPTABLEWIDGET_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据组配置界面。

*/

#include <QDialog>

#define GRP_LAYOUT_SPACE                                            15

class CDataArchivesTabModel;
class IMDDataGroup;
class QPushButton;
class QTableView;
class QLabel;
class QLineEdit;
class QCheckBox;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class CMDDataArchivesTableWidget;

class IMDDataArchives;
class IMDRTDBManagerUI;
class CDBHisGrpFramework;
class CDataArchivesUI;

class ITranslateWidgetTool;

class CMDDataGroupTableWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CMDDataGroupTableWidget(QWidget *parent = 0);
    ~CMDDataGroupTableWidget();

    /*!
    \section 配置属性
    */
public:
    inline IMDDataArchives *dataArchives(){return m_pDataArchives;}
    inline void setDataArchivesUI(CDataArchivesUI *pDataArchivesUI){m_pDataArchivesUI = pDataArchivesUI;}
    void setDataGrpProperty(IMDRTDBManagerUI *pRTDBManagerUI, IMDDataArchives *pDataArchives, IMDDataGroup *pDataGrp);

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
    int calculateLabelWidth(QLabel *pLb);

    void checkDataSrc();

signals:

    /*!
    \section 槽函数处理
    */
private slots:    
    void on_cancelButton_clicked();
    void on_selectVarButton_clicked();
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void on_name_textchange(const QString &text);

private:
//    QPushButton *m_pBtnCancel;
//    QPushButton *m_pBtnOk;
    QPushButton *m_pBtnSelectVar                    ; //! 选择变量
    QPushButton *m_pBtnAdd                          ; //! 增加按钮
    QPushButton *m_pBtnRemove                       ; //! 删除按钮

    QLabel *m_pLbName                               ; //! 数据组名标签
    QLineEdit *m_pLineName                          ; //! 数据组名编辑框
    QLabel *m_pLbDescri                             ; //! 数据组描述标签
    QLineEdit *m_pLineDescri                        ; //! 数据组描述编辑框

//    QCheckBox *m_pCheckEnable;
    QLabel *m_pLbTabDescrip                         ; //! 数据组成员
    QGroupBox *m_pGrpboxTab                         ; //! groupbox

    QHBoxLayout *m_pHFirstLayout                    ; //! 数据组水平布局
    QHBoxLayout *m_pHSecondLayout                   ; //! 数据组描述水平布局
    QHBoxLayout *m_pHThirdLayout                    ; //! 操作水平布局
    QVBoxLayout *m_pVMainLayout                     ; //! 整体水平布局

    IMDDataGroup *m_pDataGrp                        ; //! 数据组
    IMDDataArchives *m_pDataArchives                ; //! 数据归档
    IMDRTDBManagerUI *m_pRTDBManagerUI              ; //! RTDBUI管理器
    CDBHisGrpFramework *m_pFramework                ; //! 数据组配置界面
    CDataArchivesUI *m_pDataArchivesUI              ; //! 数据归档窗体

    CMDDataArchivesTableWidget *m_pTableWidget      ; //! 数据归档表格界面
    ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具
};

#endif // MDDATAGROUPTABLEWIDGET_H
