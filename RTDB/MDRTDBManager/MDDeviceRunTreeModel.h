/*!
author enli 2015.01.08
\section 运行时实时数据库设备变量列表，由CMDRTDBDevicesWidget创建
*/
#ifndef CMDDEVICERUNTREEMODEL_H
#define CMDDEVICERUNTREEMODEL_H

#include <QModelIndex>
#include <QVariant>
#include "IMDTreeModel.h"
#include "IDevice.h"
#include "IDataBlock.h"
#include "IMDRTDBVariable.h"
#include "IMDDriverManager.h"
#include "MDRTDBVariableUaBase.h"

class IMDTreeItem;
class IMDDriverManager;

class CMDDeviceRunTreeModel: public IMDTreeModel
{
    Q_OBJECT
public:
    CMDDeviceRunTreeModel();
    ~CMDDeviceRunTreeModel();
public:
    //!根据index的类型,返回某列的内容
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    //!列表框的标题栏
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    //!根据父节点+行列，返回Treeindex
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    //!父节点
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    //!返回节点状态
        //NoItemFlags = 0、
        //ItemIsSelectable = 1,
        //ItemIsEditable = 2,
        //ItemIsDragEnabled = 4,
        //ItemIsDropEnabled = 8,
        //ItemIsUserCheckable = 16,
        //ItemIsEnabled = 32,
        //ItemIsTristate = 64,
        //ItemNeverHasChildren = 128
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    //!多语言切换,最初调用by tang
    void changeLanguage();
    /*
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    */
public:
    void initial(IMDDriverManager *drvierManager);
private:
    IMDTreeItem * getItem(const QModelIndex &index) const;
    IMDTreeItem *m_rootTreeItem;
    QStringList m_headerList;
    IMDDriverManager* m_driverManager;

/*!
\section 设置列表中树控件节点状态（图标、选中使能）
*/
public:
    void setTreeItemIcon(QVariant icon,MDTreeItemType itemType);
    QList<QModelIndex> getCheckedItems();
    void setCheckedable(bool enableChecked);
    void deleteTreeItem(const QModelIndex &itemIndex);
    void setCurRootTreeItem(const QString& rootTreeItemName);
    void setExpandItem(const QModelIndex &itemIndex);
private:
    QVariant m_deviceIcon;
    QVariant m_dataBlockIcon;
    QVariant m_variableIcon;
    bool m_enableChecked;
    QList<QModelIndex> m_checkedList;
};
#endif // CMDDEVICERUNTREEMODEL_H
