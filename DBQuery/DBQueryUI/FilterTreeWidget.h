#ifndef FILTERTREEWIDGET_H
#define FILTERTREEWIDGET_H
#include <QTreeWidget>
#include <MDDBQueryConfigure.h>

class FilterTreeWidget  : public QTreeWidget
{
    Q_OBJECT
public:
    FilterTreeWidget(QWidget *parent = 0);
    void setDBQueryPath(const QString & dbQueryPath);
    void setDataView(DBQ::View * pView);
    //!由界面更新过滤项
    void updateFilterItemNode(QTreeWidgetItem * pItem, DBQ::ViewFilterNode * filterNode);
    void setItemTextWithFormat(QTreeWidgetItem *pItem, DBQ::ViewFilterNode * filterNode);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QString m_DBQueryPath;
    DBQ::View * m_pView;
};

#endif // FILTERTREEWIDGET_H
