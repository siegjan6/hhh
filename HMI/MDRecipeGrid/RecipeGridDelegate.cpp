#include "RecipeGridDelegate.h"
#include "RecipeGridModel.h"
#include "RecipeGridView.h"
#include "GridItems/GridItemDef.h"
#include "ShareTool.h"
#include "IHMIForm.h"

#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QCheckBox>
#include <QGroupBox>


CRecipeGridDelegate::CRecipeGridDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    m_mainView = static_cast<CRecipeGridView*>(parent);
    m_highLight = false;
    m_clickedEditor = NULL;
}

QWidget *CRecipeGridDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CRecipeGridView* gridview = qobject_cast<CRecipeGridView*>(parent->parent());
    if( gridview && gridview->isItemFrozen(index) && gridview->tableViewType() == viewType::mainView )
        return NULL;

    const CRecipeGridModel* model = static_cast<const CRecipeGridModel*>(index.model());
    CGridBaseItem* item = model->getItem(index);
    /*!
    默认使用懒加载模式,也就是当编辑有效,某个角色数据被修改后,才创建item实例
    所以对于从未编辑过的item来说,直接按默认处理即可
    */
    if( NULL == item )
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    QWidget* widget = item->createEditorWidget(parent, option, this);
    if( NULL == widget )
        widget = QStyledItemDelegate::createEditor(parent, option, index);


    return widget;
}

void CRecipeGridDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{       
    const CRecipeGridModel* model = static_cast<const CRecipeGridModel*>(index.model());
    CGridBaseItem* item = model->getItem(index);
    if( NULL == item )
    {
        QStyledItemDelegate::paint(painter,option,index);
        return;
    }
    /*!
    此接口负责绘制dispaly状态,一般就是非edit状态
    所有我们要在field里面显示标准控件,而在开发环境中仅显示为editText好能够编辑变量名称,即默认实现即可
    */
    if( CShareTool::m_isStudioMode )
        QStyledItemDelegate::paint(painter,option,index);
    else if( !item->needCustomPaint(painter,option) )
        QStyledItemDelegate::paint(painter,option,index);

    drawFrame(painter,option,index);
    return;
}
#include "MDJsonConvert.h"
void CRecipeGridDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    if(m_mainView->currentIndex() != index || !highLight())
        return;

    QColor bgColor = option->backgroundBrush.color();
    if( option->backgroundBrush.style() == Qt::NoBrush )
        bgColor = Qt::white;
    QBrush newBrush(bgColor);
    option->palette.setBrush(QPalette::Highlight, newBrush);
    option->palette.setBrush(QPalette::HighlightedText, Qt::black);
}

void CRecipeGridDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    CRecipeGridModel* mainModel = qobject_cast<CRecipeGridModel*>(model);
    CGridBaseItem* item = mainModel->getItem(index);

    if( !item )
        QStyledItemDelegate::setModelData(editor, model, index);
    else if( !item->needCustomSetModelData(editor, model, index) )
        QStyledItemDelegate::setModelData(editor, model, index);
}

void CRecipeGridDelegate::drawFrame(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const CRecipeGridModel* model = static_cast<const CRecipeGridModel*>(index.model());
    CGridBaseItem* item = model->getItem(index);
    if( NULL == item )
        return;

    frameData frame = item->frame();
    qreal penWidth = frame.pen.width();
    if(frameType::noframe == frame.type || 0 == penWidth )
        return;

    painter->save();


    painter->setPen(frame.pen);
    QRectF rect = option.rect;
    rect.adjust(penWidth/2.f, penWidth/2.f, -penWidth/2.f, -penWidth/2.f);

    if( frameType::allframe == frame.type  )
    {
        painter->drawRect(rect);
    }
    else
    {
        if( frame.type & frameType::topframe )
            painter->drawLine(rect.topLeft(), rect.topRight());
        if( frame.type & frameType::rightframe )
            painter->drawLine(rect.topRight(), rect.bottomRight());
        if( frame.type & frameType::bottomframe )
            painter->drawLine(rect.bottomRight(), rect.bottomLeft());
        if( frame.type & frameType::leftframe )
            painter->drawLine(rect.bottomLeft(),rect.topLeft());
    }


    painter->restore();
}

void CRecipeGridDelegate::commitDataByComboBox(int)
{
    QComboBox* widget = qobject_cast<QComboBox*>(sender());

    if(widget)
    {
        qint32 selIndex = widget->currentData().toInt();
        //script
        if( selIndex != -1 && m_clickedEditor == widget )
        {
//            static int cnter = 0;

//            QModelIndex index = m_mainView->currentIndex();
//            QList<QVariant> paras;
//            paras << (qint32)index.row() << (qint32)index.column() << false;
//            qDebug("%d  combo sel  row=%d col=%d", cnter++, index.row(), index.column());
//            m_mainView->fireFormEvent(sEditFinish, paras);
        }

        emit commitData(widget);
    }
}

void CRecipeGridDelegate::commitDataByCheckBox(int)
{
    QCheckBox* widget = qobject_cast<QCheckBox*>(sender());
    if(widget)
        emit commitData(widget);
}

void CRecipeGridDelegate::commitDataByRadioButton(int)
{
    QButtonGroup* group = qobject_cast<QButtonGroup*>(sender());
    QGroupBox* widget = qobject_cast<QGroupBox*>(group->parent());
    if(widget)
    {
        QAbstractButton* button = group->checkedButton();
        if(button)
        {
            widget->setProperty("index", button->property("index"));
            group->setExclusive(true);
        }
        else
        {
            group->setExclusive(false);
            widget->setProperty("index", -1);
        }

        emit commitData(widget);
    }
}

