#include "GridCheckBox.h"
#include <QPainter>
#include "CheckBoxItem.h"
#include "RecipeGridView.h"
#include "ShareTool.h"

CGridCheckBox::CGridCheckBox(CCheckBoxItem *item, QWidget *parent) :
    QCheckBox(parent),
    m_item(item),
    m_userActivate(false)
{
    void (CGridCheckBox:: *signal)(int) = &CGridCheckBox::stateChanged;
    connect(this, signal, this, &CGridCheckBox::onUserActivated);

    CRecipeGridView* mainView = qobject_cast<CRecipeGridView*>(parent->parentWidget());
    if( mainView )
    {
        QObject::connect(this, &CGridCheckBox::editFinish, mainView, &CRecipeGridView::itemEditFinishEvent, Qt::QueuedConnection);
    }


}

void CGridCheckBox::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_item->background());

    QCheckBox::paintEvent(event);
}

void CGridCheckBox::mousePressEvent(QMouseEvent *event)
{
    QCheckBox::mousePressEvent(event);
    m_userActivate = true;
}

void CGridCheckBox::mouseReleaseEvent(QMouseEvent *event)
{
    QCheckBox::mouseReleaseEvent(event);
    m_userActivate = false;
}

void CGridCheckBox::onUserActivated(int)
{
    if( m_userActivate && !CShareTool::m_isStudioMode)
        emit editFinish();
    m_userActivate = false;
}
