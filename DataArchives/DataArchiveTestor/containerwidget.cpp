#include "containerwidget.h"
/*
ContainerWidget::ContainerWidget(QWidget *parent) :
    QWidget(parent)
{
    m_pVMainLayout = new QVBoxLayout();
    setLayout(m_pVMainLayout);
}

void ContainerWidget::AddWidget(QWidget * pChildWidget)
{
    //保持只有一个子控件
    //<删除已存在的控件>
    QLayoutItem *child = NULL;
    QWidget * pOldChildWidget = 0;

    while(child = m_pVMainLayout->takeAt(0))
    {
        pOldChildWidget = child->widget(); //包含的实际窗体
        if(pOldChildWidget)
            delete pOldChildWidget;

        delete child;
    }
    //</删除已存在的控件>

    m_pVMainLayout->addWidget(pChildWidget);
}
*/
