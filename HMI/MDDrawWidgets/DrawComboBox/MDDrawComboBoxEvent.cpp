#include "MDDrawComboBoxEvent.h"
#include "MDDrawComboBox.h"
#include "DrawComboBoxData.h"

CMDDrawComboBoxEvent::CMDDrawComboBoxEvent()
{
}
/*!
\section  绑定相关事件
*/
void CMDDrawComboBoxEvent::connectEvent()
{
    CMDDrawBasicWidgetEvent::connectEvent();

    if (parent()->parent() == NULL)
        return;
    if (parent()->parent()->framework()->isStudioMode())
        return;

    QComboBox* sender = static_cast<QComboBox*>(parent()->widget());
    connect(sender,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexChanged(int)));
    connect(sender,SIGNAL(currentIndexChanged(QString)),this,SLOT(textChanged(QString)));
}

void CMDDrawComboBoxEvent::writeVariable()
{
    CMDDrawComboBox* w = dynamic_cast<CMDDrawComboBox*>(parent());
    if (w != NULL)
    {
        if(w->writeVarEnabled())
        {
            w->writeVariable();
        }
    }
}

void CMDDrawComboBoxEvent::currentIndexChanged(int index)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(index));
    parent()->parent()->run()->fireEvent(parent()->name(), "onCurrentIndexChanged", paras);
    writeVariable();
}

void CMDDrawComboBoxEvent::textChanged(QString str)
{
    //script
    QList<QVariant> paras;
    paras.append(QVariant(str));
    parent()->parent()->run()->fireEvent(parent()->name(), "onTextChanged", paras);
    writeVariable();
}
