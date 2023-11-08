#include "FilterTreeWidget.h"
#include "ViewFilterFieldDialog.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QLabel>

Q_DECLARE_METATYPE(DBQ::ViewFilterNode*)

FilterTreeWidget::FilterTreeWidget(QWidget *parent)
    :QTreeWidget(parent),
      m_pView(0)
{
    setFont(QFont("Times New Roman"));
}

void FilterTreeWidget::setDBQueryPath(const QString & dbQueryPath)
{
    m_DBQueryPath = dbQueryPath;
}

void FilterTreeWidget::setDataView(DBQ::View * pView)
{
    m_pView = pView;
}


//前提条件:每个节点都绑定了ViewFilterNode节点.子节点的文件不能是"and"或"or".
void FilterTreeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    //qDebug()<<"FilterTreeWidget::mouseDoubleClickEvent:"<<event->pos();
    QTreeWidgetItem * pItem = itemAt(event->pos());
    if(!pItem)
        return ;

    //根节点不允许修改 即"过滤条件节点"
    if(!pItem->parent())
        return ;

    QString itemText = pItem->text(0);
    //qDebug()<<"FilterTreeWidget::mouseDoubleClickEvent:item text = "<<itemText;
    DBQ::ViewFilterNode * currentFilter = pItem->data(0, Qt::UserRole).value<DBQ::ViewFilterNode*>();
    if(0 == currentFilter)
        return ;

    if("and" == itemText)
    {
        currentFilter->type = DBQ::VIEW_FILTER_NODE_TYPE_OR;
        pItem->setText(0, "or");
        return ;
    }

    if("or" == itemText)
    {
        currentFilter->type = DBQ::VIEW_FILTER_NODE_TYPE_AND;
        pItem->setText(0, "and");
        return ;
    }

    //即不是根节点 又不是and/or节点 只能是叶子节点了
    updateFilterItemNode(pItem, currentFilter);
}

void FilterTreeWidget::updateFilterItemNode(QTreeWidgetItem * pItem, DBQ::ViewFilterNode * filterNode)
{
    if(!filterNode)
        return ;//TODO:log
    if(filterNode->type != DBQ::VIEW_FILTER_NODE_TYPE_LEAF)
        return ;//TODO:log

    DBQ::ViewField * pField = 0;
    if(!m_pView)
        return ;

    if(!(pField=DBQ::View_GetFieldByTableFieldName(m_pView, filterNode->leftValue)))
        return ;//TODO:log

    ViewFilterFieldDialog dlg;
    dlg.setDBQueryPath(m_DBQueryPath);
    dlg.setView(m_pView);
    dlg.setFilterItem(filterNode);

    if(dlg.exec() != QDialog::Accepted) //取消
        return ;

    filterNode->type = DBQ::VIEW_FILTER_NODE_TYPE_LEAF;
    DBQ::ViewFilterNode * pNodeNew = dlg.getResult();
    if(!pNodeNew)
    {
        QMessageBox msg;
        msg.setText("更新过滤项出错");
        msg.exec();
        return ;
    }

    *filterNode = *pNodeNew;
    setItemTextWithFormat(pItem, filterNode);
    delete pNodeNew;

    return ;
}

void FilterTreeWidget::setItemTextWithFormat(QTreeWidgetItem *pItem, DBQ::ViewFilterNode * filterNode)
{
    if(0 == pItem || 0 == filterNode)
        return ;

    //为了使叶子节点对齐，都要添加label。lable显示过滤项内容.
    QString cmpStr = DBQ::FilterTree_GetCompareString(filterNode->compare);

    QString rvStr;
    if(DBQ::VIEW_FILTER_RIGHT_VALUE_TYPE_CONST == filterNode->rightValueType)
        rvStr = QString("%1 %2 %3").arg(filterNode->leftValue, cmpStr, filterNode->rightValue);
    else   //右值的类型为变量 将变量变成蓝色
    {
        cmpStr.replace('<', "&lt;"); //替换特殊字符。因为使用html格式显示字符串。如果rvStr包含了“<”或“>”会显示出错
        cmpStr.replace('>', "&gt;");

        rvStr = QString("<font color=black>%1 %2 </font><font color=blue>%3</font>")
                                 .arg(filterNode->leftValue)
                                 .arg(cmpStr)
                                 .arg(filterNode->rightValue);
    }

    QLabel *label = new QLabel(rvStr, this);
    label->setFont(QFont("Times New Roman"));

    setItemWidget(pItem, 0, label);
}
