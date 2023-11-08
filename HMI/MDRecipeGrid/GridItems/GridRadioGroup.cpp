#include "GridRadioGroup.h"
#include <QVBoxLayout>
#include <QPainter>
#include "RadioButtonItem.h"
#include "RecipeGridView.h"
#include "ShareTool.h"

CGridRadioGroup::CGridRadioGroup(CRadioButtonItem* item, const CRecipeGridDelegate *delegate, QWidget *parent) :
    QGroupBox(parent),
    m_item(item),
    m_userActivate(false)
{
    m_group = new QButtonGroup(this);
    QVBoxLayout *vbox = new QVBoxLayout;

    qint32 cnter = 0;
    foreach(const CValueMapManager::ValuePair& pair, m_item->m_values)
    {
        CGridRadioButton *radio = new CGridRadioButton(pair.second, this);
        radio->installEventFilter(this);
        radio->setProperty("index", cnter);
        m_group->addButton(radio, cnter);
        radio->setChecked(false);
        vbox->addWidget(radio);
        cnter++;
    }

    setProperty("index", -1);
    vbox->addStretch(1);
    setLayout(vbox);

    void (QButtonGroup:: *signal)(int) = &QButtonGroup::buttonClicked;
    QObject::connect(m_group, signal, delegate, &CRecipeGridDelegate::commitDataByRadioButton);
    QObject::connect(m_group, signal, this, &CGridRadioGroup::onUserActivated);

    void (QButtonGroup:: *signal2)(int) = &QButtonGroup::buttonPressed;
    QObject::connect(m_group, signal2, this, &CGridRadioGroup::onButtonPressed);

    CRecipeGridView* mainView = qobject_cast<CRecipeGridView*>(parent->parentWidget());
    if( mainView )
    {
        QObject::connect(this, &CGridRadioGroup::editFinish, mainView, &CRecipeGridView::itemEditFinishEvent, Qt::QueuedConnection);
    }
}

void CGridRadioGroup::enableEdit(bool edit)
{
    setEnabled(edit);
    foreach( QAbstractButton* btn, m_group->buttons() )
        btn->setEnabled(edit);
}

void CGridRadioGroup::setCheckState(qint32 index)
{
    if(index == -1)
    {
        m_group->setExclusive(false);
        foreach(QAbstractButton* btn, m_group->buttons())
            btn->setChecked(false);
        setProperty("index", -1);
    }
    else if( index >=0 && index < m_group->buttons().size() )
    {
        m_group->setExclusive(true);
        m_group->button(index)->setChecked(true);
    }
}

void CGridRadioGroup::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_item->background());

    QGroupBox::paintEvent(event);
}

void CGridRadioGroup::mousePressEvent(QMouseEvent *event)
{
    QGroupBox::mousePressEvent(event);
    event->ignore();
}

void CGridRadioGroup::mouseReleaseEvent(QMouseEvent *event)
{
    QGroupBox::mouseReleaseEvent(event);
    event->ignore();
}

void CGridRadioGroup::onUserActivated(int)
{
    if( m_userActivate && !CShareTool::m_isStudioMode)
        emit editFinish();

    m_userActivate = false;
}

void CGridRadioGroup::onButtonPressed(int)
{
    m_userActivate = true;
}


//!------------------------CGridRadioButton----------------------------------------------------------//
void CGridRadioButton::mousePressEvent(QMouseEvent *event)
{
    QRadioButton::mousePressEvent(event);
    event->ignore();
}

void CGridRadioButton::mouseReleaseEvent(QMouseEvent *event)
{
    QRadioButton::mouseReleaseEvent(event);
    event->ignore();
}
