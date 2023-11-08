/*!
author enli 2015.01.08
\section 编辑状态下实时数据库变量列表内容，与RunTreeMode相比，少了Device与DataBlock下变量列表，由CMDRTDBDevicesWidget创建
*/
#ifndef CMDDEVICETREEMODEL_H
#define CMDDEVICETREEMODEL_H

#include <QModelIndex>
#include <QVariant>
#include "IMDTreeModel.h"

class IMDTreeItem;
class IMDDriverManager;

class CMDDeviceTreeModel: public IMDTreeModel
{
    Q_OBJECT
public:
    CMDDeviceTreeModel();
    ~CMDDeviceTreeModel();
public:
/*!
\section 获取数据库列表信息
*/
    //!某列文本
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    //!列表头
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    //!节点索引
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    //!节点属性
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    /*
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    */
    //!多语言切换,最初调用by tang
    void changeLanguage();

public:
    //!列表头
    void initial(IMDDriverManager *drvierManager);
private:
    IMDTreeItem * getItem(const QModelIndex &index) const;
    IMDTreeItem *m_rootTreeItem;
    QStringList m_headerList;
    IMDDriverManager *m_driverManager;
/*!
\section 设置控件
*/
public:
    //!设置节点图标
    void setTreeItemIcon(QVariant icon,MDTreeItemType itemType);
    //!获取选中节点
    QList<QModelIndex> getCheckedItems();
    //!设置选中
    void setCheckedable(bool enableChecked);
    //!刷新当前节点
    void setCurRootTreeItem(const QString& rootTreeItemName);
    //!删除节点
    void deleteTreeItem(const QModelIndex &itemIndex);
    void setExpandItem(const QModelIndex &itemIndex);
private:
    QVariant m_deviceIcon;
    QVariant m_dataBlockIcon;
    QVariant m_variableIcon;
    bool m_enableChecked;
    QList<QModelIndex> m_checkedList;
};

#endif // CMDDEVICETREEMODEL_H
