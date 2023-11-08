/*!
\author bWVpeGw=
\section 字段属性配置框
*/
#ifndef COMMONFORM_H
#define COMMONFORM_H

#include <QWidget>
#include "DataGroupData.h"
#include "MDDBQueryConfigure/MDDBQueryConfigure.h"

class QLineEdit;
class IMDDataSource;
class IMDDataTable;
class IMDDataField;
class IMDDataArchives;

namespace Ui {
class CCommonForm;
}

/*!
\section 字段属性配置类
*/
class CCommonForm : public QWidget
{
    Q_OBJECT

public:
    explicit CCommonForm(QWidget *parent = 0);
    ~CCommonForm();

private:
    Ui::CCommonForm *ui;

/*!
\section 初始化
*/
public:
    void initialize();                                      //! 界面初始化
    void setDBQueryPath(const QString &path);               //! 设置数据查询子目录路径
    void initViewList(QString viewName);                    //! 初始化视图文件名称链表
    QString getViewName() const;                            //! 获取视图名称
    void setViewName(QString viewName);                     //! 设置视图名称

public:
    bool m_isDataArchives;                                  //! 数据源数据表下拉框是否可用

/*!
\section 配置表格加载字段
*/
public:
   void setFields(QList<CDataGroupField> *value);
   QList<CDataGroupField>* fields();

private:
    void loadTabHeaderProperty();
    void loadTabContentProperty();
    void setControlVisible();
    MDDataType convertViewFieldValueTypetoMDDataType(DBQ::VIEW_FIELD_TYPE fieldType);
    void setDataGroupFields(QList<DBQ::ViewField *> viewFieldsList);

/*!
\section 保存当前界面属性修改值
*/
public:
    void saveFields();
    void recreatFields(QList<DBQ::ViewField *> viewFieldsList);
/*!
\section 信号槽连接
*/
private:
    void creatConnection();

private slots:
    void slotSelectAllButton();
    void slotSelectNoneButton ();
    void slotUpButton();
    void slotDownButton();
    void slotMoveProtectButton();
    void slot_viewNameIndexChanged(const QString& viewName);

private:
    QList<CDataGroupField> m_fields;        //! 字段数据
    QList<int>lastWidth;                    //! 上一次属性配置的列宽
    QLineEdit *edit;
    QList<QLineEdit*>editList;
    QString m_DBQueryPath;                  //! 数据查询子模块路径
    QString m_viewName;                     //! 当前视图名称
    QList<DBQ::ViewField *> m_viewfields;   //! 视图字段
};

#endif // COMMONFORM_H
