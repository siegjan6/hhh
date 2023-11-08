/*!

\author eHVr

\section 功能

 特征标签设置界面
 \a 分为实时和历史特征标签设置
*/
#ifndef CFEATURETAGSETTINGFORM_H
#define CFEATURETAGSETTINGFORM_H

#include <QWidget>
#include <QMessageBox>
#include "CDataSource.h"
#include "CTagPosition.h"

#include "Data.h"
#include <QMap>
#include <QMenu>
#include <QKeyEvent>

class IMDDataArchives;
class IMDRTDBManagerUI;
class CMDFilterTreePlugin;

class CTagManager;
class CDataProperty;
class CRealFTag;
class CFeatureTag;
class CFeatureTagItem;
class CRealTimeTagManager;
class CHistoryTagManager;
class CXAxisManager;
class CFeatureTagPropertySettingDialog;
class CMDFilterTreeData;
class CXAxis;

class QTableWidgetItem;
class CRealTagSettingDialog;

class CFeatureTagConfigDialog;
namespace Ui {
class CFeatureTagSettingForm;
}

class CFeatureTagSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CFeatureTagSettingForm(QWidget *parent = 0);
    ~CFeatureTagSettingForm();
private:
    Ui::CFeatureTagSettingForm *ui;

public:
    void setData(CHistoryTagManager *tag, CXAxisManager *xs);
    void setData(CRealTimeTagManager *tag, CXAxisManager *xs);
    void setXList(const QList<CXAxis *> &list);

    void setVariantForm(IMDRTDBManagerUI *variantForm); //! 实时变量选择UI
    void setDataArchives(IMDDataArchives *dataArchives);//! 数据查询
    void setIsRealTime(bool);
    void setDataManager(CDataProperty *data);           //! 数据管理类

public:
    void applyChanges();           //! 应用内容修改
    void applyHistoryTagChanges();
    void applyRealTimeTagChanges();
/*!
\section 窗体编辑标志
*/
public:
    void resetModified();
private slots:
    void slotHandleModified();
signals:
    void itemModified();
/*!
\section 初始化
*/
private:
    void initialize();
    void initializeUI();
    void initializeData();
    void createMenu();
/*!
\section 信号槽
*/
private:
    void createConnections();
    void breakConnections();
private slots:
    void slotAddItem();
    void slotAddDozenItem(); //! 增加标签
    void slotDeleteItem();
    void slotSetProperty();                           //! 响应属性按钮点击,设置特征标签的通用属性
    void slotSetItemProperty(QTableWidgetItem *item); //! 设置选中项的属性
    void slotSetItemVisible(QTableWidgetItem *item);
    void slotMoveUpRow();
    void slotMoveDownRow();
    void slotCustomMenu(const QPoint &p);
private:
    void setItemProperty(CFeatureTag *itemInitial);    //! 设置指定标签的属性
    void setItemProperty(CRealFTag *itemInitial);      //! 设置指定标签的属性
    void parseItemsInformation();                      //! 解析特征标签信息
    int showMessage(const QString & comment,
           QMessageBox::StandardButtons buttons = QMessageBox::Ok);//! Tips
    void swapTableItem(int preIndex,int currentIndex);             //! Mark

    QTableWidgetItem *createNameItem(const QString &name);
    QTableWidgetItem *createVisibleItem(bool visible);
    QTableWidgetItem *createCommentItem(const QString &comment);
    QTableWidgetItem *createFormatItem(const QString &format);
    void insertTableItem(int row,int column,QTableWidgetItem* item);


    void deleteHistoryTagItem(int index);
    void deleteRealTimeTagItem(int index);

    void setRealTimeTagProperty();
    void setHistoryTagPorperty();

    void addHistoryItem();
    void addRealTimeItem();
    bool isTagRepeat(const QString &str);
    int  findItemByName(const QString &name);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    bool m_isModified;
    CCurveDataManager      m_datas;             //! 数据管理类
    CHistoryTagManager     *m_historyTag;      //! 标签变量组
    CRealTimeTagManager    *m_realTimeTag;

    CTagManager            *m_tag;
    CXAxisManager          *m_xs;              //! x轴管理器
    IMDRTDBManagerUI       *m_variantForm;
    QStringList             m_fields;
    IMDDataArchives         *m_dataArchives;
    CDataProperty           *m_dataManager;
    bool                     m_isRealTime;
    QString                  m_filter;
    QMap<QString,CTagPos>    m_itemPosMap;
    QList<CRealFTag *>       m_realTagList;
    QList<CRealFTag *>       m_preRealTagList;
    CFeatureTagPropertySettingDialog    *m_settingForm;   //! 单个标签属性设置窗体

    QList<CFeatureTag *>     m_historyFeatureTagList;
    QList<CFeatureTag *>     m_prehHistoryFeatureTagList;

    CFeatureTagConfigDialog  *m_configDialog;
    QMenu                    *m_menu;                       //! 操作菜单

};
#endif // CFEATURETAGSETTINGFORM_H
