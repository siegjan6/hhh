#include "MDDrawListEvent.h"
#include "MDDrawList.h"
#include "DrawListData.h"

CMDDrawListEvent::CMDDrawListEvent()
{
}
/*!
\section  绑定当前索引改变事件
*/
void CMDDrawListEvent::connectEvent()
{
    CMDDrawBasicWidgetEvent::connectEvent();

    if (parent()->parent() == NULL)
        return;
    if (parent()->parent()->framework()->isStudioMode())
        return;
    QListWidget* sender = static_cast<QListWidget*>(parent()->widget());
    connect(sender,&QListWidget::currentRowChanged,this,&CMDDrawListEvent::currentIndexChanged);  
}

void CMDDrawListEvent::currentIndexChanged(int value)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(value));
    parent()->parent()->run()->fireEvent(parent()->name(), "onCurrentIndexChanged", paras);
}
