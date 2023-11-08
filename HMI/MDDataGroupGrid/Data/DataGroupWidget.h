/*!
\author bWVpeGw=
\section 报警日志公共实现类
*/
#ifndef DATAGROUPWIDGET_H
#define DATAGROUPWIDGET_H

#include <QWidget>
#include <QDataStream>
#include <QMouseEvent>
#include <QTableView>
#include <QMenu>
#include <QTimer>
#include <QJsonObject>
#include "DataGroupData.h"
#include "DataGroupModel.h"
#include "IMDDataGroup.h"
#include "MDDatabaseRecordset.h"
#include "IDrawEnvironment.h"
#include "IMDDBQueryService.h"
#include <IMDDBQueryEngine.h>
#include <QDebug>

class IMDDataTable;
class IMDDataSource;
class CMDFilterTreeDataManager;
namespace Ui {
class CDataGroupWidget;
}
/*!
\section 数据接口类
*/
class CMDDataArchivesDBResultNotify : public QObject, public IMDDBRecordSetNotify
{
    Q_OBJECT
public:
    CMDDataArchivesDBResultNotify(){}
    ~CMDDataArchivesDBResultNotify(){}
signals:
    void receiveDatas(CMDDatabaseRecordset*);
public:
/*!
\section 查询结果通知
*/
    void NotityDBQueryResult(const QString &/*strQueryID*/, CMDDatabaseRecordset *pResult)
    {
        emit receiveDatas(pResult);
    }
} ;

/*!
\section 控件实现类
*/
class CDataGroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CDataGroupWidget(QWidget *parent = 0);
    ~CDataGroupWidget();

private:
    Ui::CDataGroupWidget *ui;

/*!
\section 初始化
*/
public:
    void onInitialization();
    void setGridType(GridType type);                    //! 获取控件类型
    void setGridName(const QString &name);              //! 获取控件名称
    void setDrawEnvrionment(IDrawEnvrionment* value);   //! 设置环境指针
private:
    void loadFields();                                  //! 初始化字段
    void loadAlarmLogFields();                          //! 加载报警或日志字段
    void initModelAndView();                            //! 初始化界面
private:
    IDrawEnvrionment* m_environment;
    CDataGroupModel* m_model;
    bool m_isDataArchives;                              //! 是否是数据归档控件，UI中数据源数据表下拉框是否可用

/*!
\section 数据组
*/
private:
    IMDDataGroup* m_pDataGroup;
    CMDDatabaseRecordset  m_recordset;                  //! 存放报警日志数据集合

/*!
\section 刷新
*/
    void refreshModel();

/*!
\section 历史查询
*/
public:
    void setQueryEngine(IMDDBQueryEngine * pEngine);
    void query();

public slots:
    void receiveDatas(CMDDatabaseRecordset *pResult) ;
private:
    CMDDataArchivesDBResultNotify *m_pQueryResult;      //! 查询结果

/*!
\section 属性配置
*/
public:
    bool openConfigDialog();                                                    //! 打开配置对话框
    void changeProperty();                                                      //! 配置属性
    QString setHeaderBackgroundColor(QColor &color);                            //! 表头背景色
    QString setScrollBarAndCorner(int horizontal, int vertical, QColor corner); //! 滚动条宽度
    CDataGroupBasicData* m_propertyData;

/*!
\section 实时触发
*/
public:
    void setNotify(bool notify);    //! 触发通知
    void dataChangedEvent();        //! 获得notify通知后，修改表格数据
private:
    bool m_isStudioMode;            //! 编辑版 = true，运行版 = fasle

/*!
\section 右键菜单
*/
    //! 报警
public:
    void createAlarmActionConnection();

private slots:
    void slotMoveMenu(QPoint);      //! 将菜单移动到鼠标的坐标位置
    void slotActionAcknowledge();       //! 知悉
    void slotActionAllAcknowledge();    //! 知悉所有
    void slotActionConfirm();       //! 确认
    void slotActionAllConfirm();    //! 确认所有
    void slotActionSuppress();       //! 抑制
    void slotActionShelve();       //! 搁置
    void slotActionAddAlarmComment();       //! 增加报警备注

private:
    QString recordIDToName(const QString& rid);

private:
    QMenu *m_popMenu;
    QAction *m_actAcknowledge;
    QAction *m_actAllAcknowledge;
    QAction *m_actConfirm;
    QAction *m_actAllConfirm;
    QAction *m_actSuppress;
    QAction *m_actShelve;
    QAction *m_actAddComment;
    int m_selectrow;

    //! 日志
public:
    void createLogActionConnection();
    void exportToCsv();
    void exportToXml();
    void exportToJson();

private slots:
    void slotLogMenu(QPoint);       //! 将菜单移动到鼠标的坐标位置
    void slotActionCsv();           //! 导出成csv格式
    void slotActionXml();           //! 导出成xml格式
    void slotActionJson();          //! 导出成json格式

private:
    QMenu *m_logMenu;
    QMenu *m_logExportMenu;
    QMenu *m_expoertMenu;
    QAction *m_actXml;
    QAction *m_actCsv;
    QAction *m_actJson;

    //! 数据归档
public:
    void createCustomConnection();

private slots:
    void slotCustomMenu(const QPoint &pos); //! 数据归档
    void slot_query_Action();
    void slot_paramenter_Action();

private:
    QMenu *m_DataArchivers_Menu;
    QAction *m_query_Action;
    QAction *m_Paramenter_Action;
/*!
\section 拷贝函数
*/
public:
    CDataGroupWidget &operator =(const CDataGroupWidget &other);

/*!
\section 序列化
*/
public:
    void serialize(QDataStream& stream, bool isOrigVersion = false);
    void deserialize(QDataStream& stream);
    void serialize_1(QDataStream& stream);

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
private:
    qint32 m_version; //! 版本号

/*!
\section 字段数据
*/
public:
    QList<CDataGroupField>* fields(){return &m_fields;} //! 字段链表
    void resetSelectFields();                           //! 选中的字段
    QList<CDataGroupField> m_fields;                    //! 报警字段
    QList<CDataGroupField> m_selectedfields;            //! 选中字段
    QString m_viewName;                                 //! 视图名称
    QString m_DBQueryPath;                              //! 数据查询子模块路径
    IMDDBQueryEngine * m_pDBQueryEngine;
};

#endif // DATAGROUPWIDGET_H
