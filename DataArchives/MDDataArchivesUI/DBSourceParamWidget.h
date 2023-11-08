#ifndef DBSOURCEPARAMWIDGET_H
#define DBSOURCEPARAMWIDGET_H

/*!

\author bHVvY2hhbw==

\section 功能
1.数据源参数配置界面。

*/

#include <QWidget>
#include <QMessageBox>

#include "MDDBConnect.h"
#include "Utility.h"

#define PARAM_WIDGET_FIXED_H                                    25
#define PARAM_WIDGET_FIXED_W                                    220
#define PARAM_LAYOUT_SPACE                                      15

class IMDDataSource;
class IMDDataSourceManager;
class CDataArchivesUI;
class IMDDataBaseParam;
class ITranslateWidgetTool;

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QCheckBox;
class QVBoxLayout;
class QHBoxLayout;
class QGroupBox;

class CDBSourceParamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDBSourceParamWidget(QWidget *parent = 0);
    ~CDBSourceParamWidget();

    /*!
    \section 属性配置
    */
public:
    inline void setDataSourceManager(IMDDataSourceManager *pManager){m_pDataSourceManager = pManager;}

    /*!
    \section 初始化属性
    */
public:
    void initUI(IMDDataSource *pDataSource, CDataArchivesUI *pParent);

    /*!
    \section 存储
    */
public:
    void save();
    void changeLanguage();

    /*!
    \section 创建界面、数据库连接
    */
private:
    void initData();
    void initLanguage();
    void createUI();
    bool ConnectDB(MDDBConnection &adoConnection, IMDDataBaseParam *DBParam, QString &strError);
    int calculateLabelWidth(QLabel *pLb);

    /*!
    \section 控件响应处理
    */
private slots:
    void on_NameEdit_textChanged(const QString &arg1);
    void on_ConnectPushButton_clicked();
    void on_PrefixState_change(int state);

private:
    IMDDataSourceManager *m_pDataSourceManager  ; //! 数据源管理器
    IMDDataSource *m_pDataSource                ; //! 数据源
    CDataArchivesUI *m_pDataArchivesUI          ; //! 数据归档界面类

    QLabel *m_pLbDataSrcName                    ; //! 数据源名字标签
    QLabel *m_pLbDataSrcDecrip                  ; //! 数据源描述标签
    QLabel *m_pLbDatabaseType                   ; //! 数据库类型标签
    QLabel *m_pLbDatabaseName                   ; //! 数据库名字标签
    QLabel *m_pLbServiceName                    ; //! 数据库服务名标签
    QLabel *m_pLbAddress                        ; //! ip地址标签
    QLabel *m_pLbPort                           ; //! 端口标签
    QLabel *m_pLbUser                           ; //! 用户标签
    QLabel *m_pLbPasswd                         ; //! 密码标签
//    QLabel *m_pLbDbDirectory;

    QLineEdit *m_pLineDataSrcName               ; //! 数据源名字编辑框
    QLineEdit *m_pLineDataSrcDecrip             ; //! 数据源描述编辑框
    QLineEdit *m_pLineDatabaseName              ; //! 数据库名字编辑框
    QLineEdit *m_pLineServiceName               ; //! 数据库服务名编辑框
    QLineEdit *m_pLineAddress                   ; //! ip地址编辑框
    QLineEdit *m_pLinePort                      ; //! 端口编辑框
//    QLineEdit *m_pLineDbDirectory;
    QLineEdit *m_pLineUser                      ; //! 用户编辑框
    QLineEdit *m_pLinePasswd                    ; //! 密码编辑框
    QLineEdit *m_pLinePrefix                    ; //! 表前缀名编辑框

    QPushButton *m_pBtnConnect                  ; //! 链接测试按钮
    QComboBox *m_pComDatabaseType               ; //! 数据类型下拉框
    QCheckBox *m_pCheckEnable                   ; //! 启用选择框
    QCheckBox *m_pCheckPrefix                   ; //! 启用前缀选择框

    QGroupBox *m_pGrpboxDBParam                 ; //! 数据库参数groupbox

    QHBoxLayout *m_pH1Layout                    ; //! 数据源名字水平布局
    QHBoxLayout *m_pH2Layout                    ; //! 数据源描述水平布局
    QHBoxLayout *m_pH3Layout                    ; //! 数据库类型水平布局
    QHBoxLayout *m_pH4Layout                    ; //! 数据库名字水平布局
    QHBoxLayout *m_pH5Layout                    ; //! 数据库服务名水平布局
    QHBoxLayout *m_pH6Layout                    ; //! ip地址水平布局
    QHBoxLayout *m_pH7Layout                    ; //! 端口水平布局
//    QHBoxLayout *m_pH8Layout;
    QHBoxLayout *m_pH9Layout                    ; //! 用户名水平布局
    QHBoxLayout *m_pH10Layout                   ; //! 密码水平布局

    QVBoxLayout *m_pVGrpboxLayout               ; //! 数据库参数groupbox垂直布局
    QVBoxLayout *m_pVMainLayout                 ; //! 全部垂直布局

    ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具
};

#endif // DBSOURCEPARAMWIDGET_H
