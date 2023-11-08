#ifndef DBDATAGROUPTREECREATOR_H
#define DBDATAGROUPTREECREATOR_H
#include <IMDDataArchives.h>
#include <QTreeWidgetItem>
#include <QTreeWidget>

class DBDataGroupTreeCreator
{
public:
    DBDataGroupTreeCreator(IMDDataArchives * pDataArchives, QTreeWidget * pTreeWidget);
    void build();
    QTreeWidgetItem * getHistoryRootItem();
private:
    void LoadDBDataGroupTree(QTreeWidgetItem *pRoot);
    void buildAlmTree(QTreeWidgetItem *pTreeRoot);
    void LoadAlmOrLogNote(QTreeWidgetItem *pRoot, bool isAlm);
    void LoadAlmNotfy(QTreeWidgetItem *pRoot);
    void LoadLogTable(QTreeWidgetItem *pRoot);
    void buildLogTree(QTreeWidgetItem *pTreeRoot);
    void buildHisTree(QTreeWidgetItem *pTreeRoot);
    void LoadHisGroup(QTreeWidgetItem *pRoot);
protected:
     IMDDataArchives * m_pDataArchives;
     QTreeWidget * m_pTreeWidget;
     QTreeWidgetItem * m_pHistoryRootItem;
};

#endif // DBDATAGROUPTREECREATOR_H
