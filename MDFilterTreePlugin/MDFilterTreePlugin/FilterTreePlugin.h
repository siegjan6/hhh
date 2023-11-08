#ifndef FILTERTREEPLUGIN_H
#define FILTERTREEPLUGIN_H

/*!

\author bHVvY2hhbw==
1.过滤控件内部真正实现类。
2.此界面有两个树构成，左边树为选择需要配置的字段，右边为对选择的字段进行配置条件。
3.在配置条件时，会弹出相应的配置条件值的界面。

*/

#include <QDialog>
#include <QVariant>

#include "IMDDataField.h"
#include "MDFilterTreePlugin.h"

const QString FILTER_STRING_AND = QObject::tr("and");
const QString FILTER_STRING_OR  = QObject::tr("or");

class QPushButton;
class QTreeWidget;
class QHBoxLayout;
class QVBoxLayout;
class QTreeWidgetItem;
class QMenu;
class QAction;


class ITranslateWidgetTool;
class IMDDataTable;
class CMDCustomFieldCategory;
class CMDFilterTreeData;

Q_DECLARE_METATYPE(QVariant::Type)
Q_DECLARE_METATYPE(IMDDataField*)
Q_DECLARE_METATYPE(CMDFilterTreeDataItem*)

class CFilterTreePlugin : public QDialog
{
    Q_OBJECT
public:
    explicit CFilterTreePlugin(QDialog *parent = 0);
    explicit CFilterTreePlugin(IMDDataTable *pTable, QDialog *parent = 0);
    ~CFilterTreePlugin();

    /*!
    \section 重写关闭窗体事件
    */
protected:
//    void closeEvent(QCloseEvent *event);

    /*!
    \section 加载存储序列化
    */
public:
    bool load(const QString &condition);
    bool load(CMDFilterTreeData *pFilterData);
    bool save(CMDFilterTreeData *pFilterData);

    /*!
    \section 初始化数据以及获取过滤条件配置接口。
    */
public:
    void initData(IMDDataTable *pTable);
    void initData(const QString &tableName, QList<CMDFilterTreeDataItem*> &customFieldList);
    QString getFilterCondition();

    void changeLanguage();

signals:

    /*!
    \section 槽函数
    */
private slots:
    void groupTreeDoubleClicked(QTreeWidgetItem * item, int column);
    void conditionTreeDoubleClicked(QTreeWidgetItem * item, int column);
    void conditionTreeClicked(QTreeWidgetItem * item, int column);
    void conditionTreeMenuClicked(const QPoint &pos);

    void andActionTriggered();
    void orActionTriggered();
    void groupActionTriggered();
    void modifyActionTriggered();
    void deleteActionTriggered();

    /*!
    \section 界面初始化、信号槽绑定
    */
private:
    void createUI();
    void createMenu();
    void signalsBindSlots();
    void initData();
    void initLanguage();

    /*!
    \section 字段类型转换
    */
private:
    QVariant::Type datatypeChangeVariant(int type);
    int calculateType(const QString &text);

    /*!
      \section 通过表达式加载生成条件树，相关接口
    */
    QString splicingCurrentItemText(QStringList &itemList, int &start);
    QString splicingString(QStringList &itemList, int start, int &end);
    void getParentItem(QTreeWidgetItem *pChild);
    void findChildItem(QStringList &itemList, int &start);

    /*!
    \section 树结点操作接口---创建、删除、遍历、查找等接口
    */
private:
    QTreeWidgetItem *createGroupTreeRoot(const QString &text);
    void createChildItem(QTreeWidgetItem *parent, IMDDataField *pField);
    void createChildItem(QTreeWidgetItem *parent, CMDFilterTreeDataItem *pFilterItem);
    void createChildItem(QTreeWidgetItem *parent, const QString &text, int type);
    void createChildItem(QTreeWidgetItem *parent, QTreeWidgetItem *item);
    void deleteChildItem(QTreeWidgetItem *item);
    void modifyItem(QTreeWidgetItem *item);
    void traversalTree(QTreeWidgetItem *root);
    bool findChildItem(QTreeWidgetItem *root, QTreeWidgetItem *item);

    void save(QTreeWidgetItem *root, CMDFilterTreeData *pFilterData);
    void load(QTreeWidgetItem *root, CMDFilterTreeData *pFilterData, int count);

private:
    QHBoxLayout *m_pHTopLayout       ; //! 顶部水平布局管理器
    QHBoxLayout *m_pHBottomLayout    ; //! 底部水平布局管理器
    QVBoxLayout *m_pVMainLayout      ; //! 整体水平布局管理器

    QTreeWidget *m_pGroupTree        ; //! 选择字段树
    QTreeWidget *m_pConditionTree    ; //! 条件配置树
    QTreeWidgetItem *m_pSelectItem   ; //! 条件配置树组选择结点

    QPushButton *m_pBtnOk            ; //! 确定按钮
    QPushButton *m_pBtnCancel        ; //! 取消按钮

    QMenu *m_pMenu                   ; //! 条件配置树菜单
    QAction *m_pAndAction            ; //! and菜单选项
    QAction *m_pOrAction             ; //! or菜单选项
    QAction *m_pGroupAction          ; //! 组菜单选项
    QAction *m_pModifyAction         ; //! 修改菜单选项
    QAction *m_pDeleteAction         ; //! 删除菜单选项

    QTreeWidgetItem *m_pParent       ; //! 条件树结点父结点

    ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具

    QString m_strFilterCondition;            //! 过滤条件
    QString m_strFile;                       //! 序列化文件
    bool m_iLoadFlag;
};

#endif // FILTERTREEPLUGIN_H
