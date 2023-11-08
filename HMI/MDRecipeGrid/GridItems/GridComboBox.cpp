#include "GridComboBox.h"
#include "RecipeGridView.h"
#include "ComboBoxItem.h"
#include "ShareTool.h"


CGridComboBox::CGridComboBox(CComboBoxItem *item, QWidget *parent)
    :QComboBox(parent),
     m_item(item)

{
    QPalette pal(palette());
    pal.setBrush(QPalette::Light, Qt::red);
    pal.setBrush(QPalette::Midlight, Qt::red);
    pal.setBrush(QPalette::Dark, Qt::red);
    setPalette(pal);

    m_userActivate = false;

    void (CGridComboBox:: *signal)(int) = &CGridComboBox::activated;
    connect(this, signal, this, &CGridComboBox::onUserActivated);

    CRecipeGridView* mainView = qobject_cast<CRecipeGridView*>(parent->parentWidget());
    if( mainView )
    {
        connect(this, &CGridComboBox::editFinish, mainView, &CRecipeGridView::itemEditFinishEvent, Qt::QueuedConnection);
    }

}

void CGridComboBox::hidePopup()
{
    QComboBox::hidePopup();
    m_userActivate = true;
}

void CGridComboBox::mousePressEvent(QMouseEvent *event)
{
    QComboBox::mousePressEvent(event);
    event->ignore();
}

void CGridComboBox::mouseReleaseEvent(QMouseEvent *event)
{
    QComboBox::mouseReleaseEvent(event);
    event->ignore();
}

void CGridComboBox::onUserActivated(int)
{
    if(m_userActivate && !CShareTool::m_isStudioMode)
    {
        emit editFinish();
    }

    m_userActivate = false;
}
