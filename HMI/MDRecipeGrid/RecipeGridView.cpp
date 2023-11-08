#include "RecipeGridView.h"
#include "RecipeGridDelegate.h"

#include <QAbstractItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <QScrollBar>
#include <QPainter>
#include <QPen>
#include <QLinkedList>
#include <algorithm>
#include <QBuffer>

#include <QJsonObject>
#include <QJsonArray>


#include "ShareTool.h"
#include "GridItems/GridItemDef.h"
#include "ConfigDialogs/ConfigDlgDef.h"
#include "FrozenSubViews/FrozenViewDef.h"
#include "PrintCloneView.h"
#include "RecipeGridStyle.h"
#include "IHMIForm.h"
#include "MDJsonConvert.h"

static const quint32 g_cInitRowNum = 10;
static const quint32 g_cInitColumnNum = 10;
static const quint32 g_cInitRowHeight = 30;
static const quint32 g_cInitColumnWidth = 80;

static const quint32 g_cInitVerHeaderWidth = 50;
static const quint32 g_cInitHorHeaderHeight = 25;

CRecipeGridView::CRecipeGridView(QWidget *parent) :
    QTableView(parent)
{
    m_environment = NULL;
    m_rowFrozen = 0;
    m_columnFrozen = 0;
    m_fixedFrozenView = NULL;
    m_rowFrozenView = NULL;
    m_columnFrozenView = NULL;
    CGridBaseItem::setDefaultFrame(m_frozenFrame);
    CPrintConfigDialog::setDefault(m_printPara);
    m_writeVersion = WriteVersion;
    m_readVersion = InvalidVersion;

    setStyle(new CRecipeGridStyle(style()));
    //! 当固定行列时,点击固定行列里的item会导致view滚动过去,激活当前item.所有设置为false
    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollMode(ScrollPerPixel);

    initHeader();
    initMenu();

    QObject::connect(this, &CRecipeGridView::clicked, this, &CRecipeGridView::clickedItem);

    viewport()->installEventFilter(this);
}

CRecipeGridView::~CRecipeGridView()
{
    qDeleteAll(m_copyItems);
}

bool CRecipeGridView::isItemFrozen(const QModelIndex &index) const
{
    if(0 == m_rowFrozen && 0 == m_columnFrozen)
        return false;

    if( index.row() < m_rowFrozen || index.column() < m_columnFrozen )
        return true;

    return false;
}

void CRecipeGridView::onInitialization(IDrawEnvrionment *environment, IHMIForm *mainform, QString jsObjName)
{
    m_environment = environment;
    m_mainForm = mainform;
    m_jsObjName = jsObjName;

    Q_ASSERT(m_environment);
    Q_ASSERT(m_mainForm);

    if( !CShareTool::m_isStudioMode )
    {
        setSelectionMode(QAbstractItemView::NoSelection);
    }

    m_model->onInitialization(environment);

    refreshGroups();
    createForzenViews();
    showAllEditor();


}

void CRecipeGridView::triggerAction(qint32 index)
{
    if( index >= m_actionList.size() )
        return;
    m_actionList.at(index)->activate(QAction::Trigger);
}

void CRecipeGridView::showEditorChanged(QModelIndex index, bool show)
{
    if(!index.isValid())
        return;

    if( show )
        openPersistentEditor(index);
    else
        closePersistentEditor(index);
}

void CRecipeGridView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    //! csript
    if( !CShareTool::m_isStudioMode && m_mainForm )
    {
        QModelIndex index = currentIndex();
        if( editor->inherits("QLineEdit") )
        {
            QLineEdit* lineedit = static_cast<QLineEdit*>(editor);
            bool cancelFlag = hint == QAbstractItemDelegate::RevertModelCache ? true : false;
            QList<QVariant>paras;
            paras << (qint32)index.row() << (qint32)index.column() << lineedit->text() << cancelFlag;
            m_mainForm->run()->fireEvent(m_jsObjName, sEditFinish, paras);
        }
    }

    QTableView::closeEditor(editor, hint);
}

void CRecipeGridView::setModify(bool modify)
{
    Q_ASSERT(m_mainForm);

    m_mainForm->setModify(modify);
}

void CRecipeGridView::invokeFrozenViewSameAction(const QModelIndex &index)
{
    if( !index.isValid() )
        return;

    if( !isItemFrozen(index) )
        return;

    QAction* action = qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    qint32 actionIndex = action->data().toInt();

    emit invokeAction(actionIndex);
}

void CRecipeGridView::createForzenViews()
{
    if(m_rowFrozenView)
    {
        delete m_rowFrozenView;
        m_rowFrozenView = NULL;
    }
    if( m_rowFrozen > 0 )
    {
        m_rowFrozenView = new CRowFrozenView(m_rowFrozen,this);
        m_rowFrozenView->init();
        m_rowFrozenView->show();
    }

    if(m_columnFrozenView)
    {
        delete m_columnFrozenView;
        m_columnFrozenView = NULL;
    }
    if( m_columnFrozen > 0 )
    {
        m_columnFrozenView = new CColumnFrozenView(m_columnFrozen,this);
        m_columnFrozenView->init();
        m_columnFrozenView->show();
    }
    if(m_fixedFrozenView)
    {
        delete m_fixedFrozenView;
        m_fixedFrozenView = NULL;
    }
    if( m_columnFrozen > 0 && m_rowFrozen > 0)
    {
        m_fixedFrozenView = new CFixedFrozenView(m_rowFrozen,m_columnFrozen,this);
        m_fixedFrozenView->init();
        m_fixedFrozenView->show();
    }
}

void CRecipeGridView::init(qint32 rowNum, qint32 columnNum)
{
    //! frozenView子类们不能再创建model和delegate.此接口是父类自己的初始化操作
    if(-1 == rowNum)
        rowNum = g_cInitRowNum;
    if(-1 == columnNum)
        columnNum = g_cInitColumnNum;

    m_model = new CRecipeGridModel(rowNum,columnNum, this);
    setModel(m_model);

    m_delegate = NULL;
    m_delegate = new CRecipeGridDelegate(this);
    setItemDelegate(m_delegate);

    setDefaultProperty(m_propertyPara);

    commonInit();
}

void CRecipeGridView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);

    if( 0 == m_rowFrozen && 0 == m_columnFrozen )
       return;

    QPainter painter(viewport());

    painter.save();

    //! 绘制固定行列功能的边框的另一半
    QPen pen(m_frozenFrame.pen);
    painter.setPen(pen);
    qreal penWidth = painter.pen().width();

    qint32 i;
    qint32 width = 0;
    qint32 height = 0;
    for (i = 0; i < m_rowFrozen; ++i)
        height += rowHeight(i);
    for (i = 0; i < m_columnFrozen; ++i)
        width += columnWidth(i);

    if( m_rowFrozen > 0)
    {
        QPointF point1(0, height);
        QPointF point2(horizontalHeader()->length() - penWidth/2.f, point1.y());
        painter.drawLine(point1, point2);
    }
    if( m_columnFrozen > 0)
    {
        QPointF point3(width, 0);
        QPointF point4(point3.x(), verticalHeader()->length() - penWidth/2.f );
        painter.drawLine(point3, point4);
    }

    painter.restore();
}

void CRecipeGridView::contextMenuEvent(QContextMenuEvent *event)
{
    //! 运行模式下,禁止右键菜单的一切功能
    if( !CShareTool::m_isStudioMode )
        return;

    QPoint point = event->globalPos();

    Q_ASSERT(m_menu);
    m_menu->exec(point);
}


void CRecipeGridView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    emit updateFrozenViewGeometry();
}

bool CRecipeGridView::eventFilter(QObject *obj, QEvent *event)
{
    //! script 触发鼠标点击脚本事件
    //! 除了CRadioGroup上的CGridRadioButton，其他editor都是在这里触发点击脚本的
    if (!CShareTool::m_isStudioMode && m_mainForm &&
        (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
       )
    {
        //qDebug("mainview  type=%d editor=%s", event->type(), obj->metaObject()->className());
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        itemClickEvent(mouseEvent->pos(), mouseEvent);
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if( mouseEvent->buttons() & Qt::LeftButton )
            m_clickPosition = mouseEvent->pos(); 
    }

    return QTableView::eventFilter(obj, event);
}

void CRecipeGridView::initHeader()
{
    QHeaderView* horizontalView = horizontalHeader();
    QHeaderView* verticalView = verticalHeader();

    //! 设置列表头宽度
    //verticalView->setMinimumWidth(50);
    //! 设置列表头字体居中
    verticalView->setDefaultAlignment(Qt::AlignCenter);
    horizontalView->setDefaultAlignment(Qt::AlignCenter);

    horizontalView->setFixedHeight(g_cInitHorHeaderHeight);
    verticalView->setFixedWidth(g_cInitVerHeaderWidth);

    verticalView->setDefaultSectionSize(g_cInitRowHeight);
    horizontalView->setDefaultSectionSize(g_cInitColumnWidth);

}

void CRecipeGridView::horizontalSectionResized(int logicalIndex, int, int newSize)
{
    if( newSize <= 0 || horizontalHeader()->sectionSize(logicalIndex) == newSize )
        return;

    setColumnWidth(logicalIndex, newSize);
    if( m_fixedFrozenView )
    {
        m_fixedFrozenView->setColumnWidth(logicalIndex, newSize);
        m_fixedFrozenView->updateGeometry();
    }
    if( m_rowFrozenView )
    {
        m_rowFrozenView->setColumnWidth(logicalIndex, newSize);
        m_rowFrozenView->updateGeometry();
    }
    if( m_columnFrozenView )
    {
        m_columnFrozenView->setColumnWidth(logicalIndex, newSize);
        m_columnFrozenView->updateGeometry();
    }
}

void CRecipeGridView::verticalSectionResized(int logicalIndex, int, int newSize)
{
    if( newSize <= 0 || verticalHeader()->sectionSize(logicalIndex) == newSize )
        return;

    setRowHeight(logicalIndex, newSize);
    if( m_fixedFrozenView )
    {
        m_fixedFrozenView->setRowHeight(logicalIndex, newSize);
        m_fixedFrozenView->updateGeometry();
    }
    if( m_rowFrozenView )
    {
        m_rowFrozenView->setRowHeight(logicalIndex, newSize);
        m_rowFrozenView->updateGeometry();
    }
    if( m_columnFrozenView )
    {
        m_columnFrozenView->setRowHeight(logicalIndex, newSize);
        m_columnFrozenView->updateGeometry();
    }
}

bool CRecipeGridView::isItemMerged(const QModelIndex &index) const
{
    if( rowSpan(index.row(), index.column()) <=1 && columnSpan(index.row(), index.column()) <= 1 )
        return false;
    else
        return true;
}

QModelIndex CRecipeGridView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

    qint32 offset;
    switch(cursorAction)
    {
        case MoveDown:
        case MovePageDown:
            if(m_rowFrozen > 0 && current.row() >= m_rowFrozen && current.row() != m_model->rowCount()-1)
            {
                verticalScrollBar()->setValue(verticalScrollBar()->minimum());
            }
            break;
        case MoveUp:
        case MovePageUp:
            if( m_rowFrozenView )
            {
                offset =  m_rowFrozenView->height() - horizontalHeader()->height() - visualRect(current).topLeft().y();
                if(current.row() > 0 &&  offset > 0)
                    verticalScrollBar()->setValue(verticalScrollBar()->value() - offset);
            }
            break;
        case MoveLeft:
        case MovePrevious:
            if(m_columnFrozenView)
            {
                offset = m_columnFrozenView->width() - verticalHeader()->width() - visualRect(current).topLeft().x();
                if(current.column() > 0 &&  offset > 0)
                    horizontalScrollBar()->setValue(horizontalScrollBar()->value()-offset);
            }
            break;
        case MoveRight:
        case MoveNext:
            if(m_columnFrozen > 0 && current.column() >= m_columnFrozen && current.column() != m_model->columnCount()-1
              )
            {
                setFocus();
                horizontalScrollBar()->setValue(horizontalScrollBar()->minimum());
            }
            break;

        default: break;
    }

    return current;
}

void CRecipeGridView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    if (index.column() >= m_columnFrozen && index.row() >= m_rowFrozen)
        QTableView::scrollTo(index, hint);
}

void CRecipeGridView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags flags)
{
    //! 当有固定行列时候,选中区域只能在view的内部
    QModelIndex tl = indexAt(QPoint(isRightToLeft() ? qMax(rect.left(), rect.right())
                                        : qMin(rect.left(), rect.right()), qMin(rect.top(), rect.bottom())));

    //! 只针对框选操作,否则会对导航键操作造成干扰
    if( QAbstractItemView::DragSelectingState != state())
    {
        QTableView::setSelection(rect, flags);
        return;
    }
    qint32 colFrozen = frozenColumnCount();
    qint32 rowFrozen = frozenRowCount();
    CRecipeGridView* parentTable = qobject_cast<CRecipeGridView*>(parentWidget());
    if( parentTable && viewType::rowFrozenView == tableViewType() )
        colFrozen = parentTable->frozenColumnCount();

    else if( parentTable && viewType::columnFrozenView == tableViewType() )
        rowFrozen = parentTable->frozenRowCount();

    qint32 i;
    qint32 offsetX = rect.right();
    if( tl.column() < colFrozen )
    {
        offsetX = 0;
        for( i = 0; i < colFrozen; ++i )
            offsetX += columnWidth(i);
    }
    qint32 offsetY = rect.bottom();
    if( tl.row() < rowFrozen )
    {
        offsetY = 0;
        for( i = 0; i < rowFrozen; ++i )
            offsetY += rowHeight(i);
    }

    QRect newRect = rect;
    newRect.setBottom(offsetY);
    newRect.setRight(offsetX);

    QTableView::setSelection(newRect, flags);
}

void CRecipeGridView::initTranslator()
{
    m_tool = CMDMultiLanguage::createWidgetTool();
    m_tool->init(this);
    m_tool->changeLanguage();
}

void CRecipeGridView::commonInit()
{
    connect( m_model, &CRecipeGridModel::showEditor, this, &CRecipeGridView::showEditorChanged);

    CRecipeGridView* parentTable = qobject_cast<CRecipeGridView*>(parentWidget());
    if(parentTable && viewType::mainView == parentTable->tableViewType() )
    {
        m_propertyPara = parentTable->m_propertyPara;
        updateProperty();
        //setShowGrid( m_propertyPara.showGrid);
        //! 适配所有行列的宽度
        qint32 i;
        for (i = 0; i < model()->rowCount(); ++i)
            setRowHeight(i, parentTable->rowHeight(i));
        for (i = 0; i < model()->columnCount(); ++i)
            setColumnWidth(i, parentTable->columnWidth(i));
        //! 适配单元格的合并状态
        SpanMap spans;
        parentTable->spanMap(spans);
        setSpanForItems(spans);
    }

    initTranslator();
}

void CRecipeGridView::mergeItem()
{
    const QModelIndexList& selectList = selectedIndexes();
    if(selectList.size() <= 1)
        return;

    QModelIndexList ascendRows(selectList);
    std::sort(ascendRows.begin(), ascendRows.end(), &CShareTool::lessThanRow );

    qint32 startRow = ascendRows.at(0).row();
    qint32 startColumn = ascendRows.at(0).column();
    qint32 size = ascendRows.size();
    qint32 endRow = ascendRows.at(size-1).row();
    qint32 endColumn = ascendRows.at(size-1).column();
    mySetSpan(startRow,startColumn,endRow-startRow+1,endColumn-startColumn+1);

    setModify(true);
}

void CRecipeGridView::splitItem()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( 1 != selectList.size() )
        return;

    const QModelIndex& index = selectList.at(0);
    if( !isItemMerged(index) )
        return;
    mySetSpan(index.row(),index.column(),1,1);

    setModify(true);
}

void CRecipeGridView::clickedItem(const QModelIndex &index)
{
    if(m_propertyPara.clickEdit && !CShareTool::m_isStudioMode )
    {
        CGridBaseItem* item = m_model->getItem(index);
        if(item && item->rowHidden() > 0)
        {
            QRect itemRect = visualRect(index);
            QSize iconsize = QSize(24,itemRect.height());
            QRect iconRect = QRect(itemRect.topLeft(), iconsize);
            if( iconRect.contains(m_clickPosition) )
            {
                expandCollapseGroup(index);
                return;
            }
        }

        if( !item || !item->canShowPersistentEditor() )
            edit(index);
        return;
    }

    expandCollapseGroup(index);
}

void CRecipeGridView::configItem()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( 1 != selectList.size() )
        return;

    const QModelIndex& index = selectList.at(0);
    CItemCfgDialog itemcfgDlg(m_model, index, m_environment, this);

    if( itemcfgDlg.exec() != QDialog::Accepted )
        return;

    setModify(true);

}

void CRecipeGridView::configMulitItems()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;
    CMulitItemCfgDialog itemsDlg(m_model, selectList, m_environment, this);

    if( itemsDlg.exec() != QDialog::Accepted )
        return;

    setModify(true);
}

void CRecipeGridView::setRecipeName()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;
    CSetRecipeNameDialog recipeNameDlg(this);
    if(recipeNameDlg.exec() != QDialog::Accepted)
        return;

    setModify(true);
}

void CRecipeGridView::setDataSource()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    const QModelIndex& index = selectList.at(0);
    CGridBaseItem* item = m_model->getItem(index);
    dataSourceType initDataSource = m_model->defaultDataSource();
    if( item )
        initDataSource = item->dataSource();
    CSetDataSourceDialog dataSourceDlg(initDataSource, this);
    if(dataSourceDlg.exec() != QDialog::Accepted)
        return;

    foreach(const QModelIndex& index, selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        if( !item && m_model->defaultDataSource() != dataSourceDlg.m_dataSource)
           item = m_model->getItem(index, true);

        if(item)
            item->setDataSource(dataSourceDlg.m_dataSource);
    }

    setModify(true);
}

void CRecipeGridView::setItemFont()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    const QModelIndex& index = selectList.at(0);
    CGridBaseItem* item = m_model->getItem(index);
    QFont oldFont = m_model->defaultFont();
    if( item )
        oldFont = item->font();

    QFontDialog fontDlg(oldFont, this);
    CShareTool::translateWidget(&fontDlg);
    if( fontDlg.exec() != QDialog::Accepted )
        return;

    QFont font = fontDlg.selectedFont();

    foreach(const QModelIndex& index, selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        if( !item && font != m_model->defaultFont() )
            item = m_model->getItem(index, true);

        if(item)
            item->setFont(font);
    }

    setModify(true);
}

void CRecipeGridView::setItemBackground()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    const QModelIndex& index = selectList.at(0);
    CGridBaseItem* item = m_model->getItem(index);
    QBrush brush;
    if(item)
        brush = item->background();
    if( Qt::NoBrush == brush.style() )
        brush.setColor(Qt::white);

    QColorDialog colorDlg(brush.color(), this);
    CShareTool::translateWidget(&colorDlg);
    if( colorDlg.exec() != QDialog::Accepted )
        return;

    QColor color = colorDlg.selectedColor();
    if(!color.isValid())
        return;

    brush.setColor(color);
    color == Qt::white ?  brush.setStyle(Qt::NoBrush) : brush.setStyle(Qt::SolidPattern);

    foreach(const QModelIndex& index, selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        if( !item && color != m_model->defaultBackgroundColor() )
            item = m_model->getItem(index, true);

        if(item)
            item->setBackground(brush);
    }

    setModify(true);
}

void CRecipeGridView::setItemFrame()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    const QModelIndex& index = selectList.at(0);
    CGridBaseItem* item = m_model->getItem(index);
    frameData frame;
    if(item)
        frame = item->frame();
    else
        CGridBaseItem::setDefaultFrame(frame);

    CFrameDialog frameDialog(frame, this);
    if(frameDialog.exec() != QDialog::Accepted)
        return;

    frameData defaultFrame;
    CGridBaseItem::setDefaultFrame(defaultFrame);
    frameData& newFrame = frameDialog.m_frameData;
    foreach(const QModelIndex& index, selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        if( !item && newFrame.type != frameType::noframe)
            item = m_model->getItem(index, true);

        if(item)
            item->setFrame(newFrame);
    }

    setModify(true);
}

void CRecipeGridView::setItemAlign()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    const QModelIndex& index = selectList.at(0);
    CGridBaseItem* item = m_model->getItem(index);
    Qt::Alignment align = m_model->defaultTextAlign();
    if(item)
        align = item->textAlignment();

    CAlignStyleDialog alignDialog(align, this);
    if(alignDialog.exec() != QDialog::Accepted)
        return;

    foreach(const QModelIndex& index, selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        if( !item && alignDialog.m_align != m_model->defaultTextAlign()  )
            item = m_model->getItem(index, true);

        if(item)
            item->setTextAlignment(alignDialog.m_align);
    }

    setModify(true);
}

void CRecipeGridView::setGroup()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;
    //! 按照row升序排列
    QModelIndexList ascendRows(selectList);
    std::sort(ascendRows.begin(), ascendRows.end(), &CShareTool::lessThanRow );

    const QModelIndex& indexFirst = ascendRows.first();
    const QModelIndex& indexLast = ascendRows.last();
    if( indexFirst.row() == indexLast.row())
        return;

    qint32 i,j;
    qint32 firstRow = indexFirst.row();
    qint32 lastRow = indexLast.row();
    //! 检查其中是否已经成组过的item,如果有则退出
    for( i = firstRow; i <= lastRow; i++ )
    {
        for( j= 0; j < m_model->columnCount(); j++ )
        {

            CGridBaseItem* item = m_model->getItem(m_model->index(i, j));
            if( item && item->rowHidden() > 0 )
                return;
        }
    }
    //! 如果被选中的item中有合并过的item,更新lastRow,确保合并的item也被加入到group中
    foreach(const QModelIndex& index, ascendRows)
    {

        if( isItemMerged(index) )
        {
            qint32 spanRow = rowSpan(index.row(),index.column()) + index.row() - 1;
            if( spanRow > lastRow )
                lastRow = spanRow;
        }
    }

    for(i = firstRow+1;i <= lastRow; i++)
        mySectionHidden(Qt::Vertical, i,true);

    CGridBaseItem* item = m_model->getItem(indexFirst, true);
    item->setRowHidden(lastRow - firstRow);
    item->setExpand(false);

    emit updateFrozenViewGeometry();

    setModify(true);
}

void CRecipeGridView::expandCollapseGroup(const QModelIndex &index, bool manualExpand)
{
    CGridBaseItem* item = m_model->getItem(index);
    if(NULL == item || item->rowHidden() <= 0)
        return;

   bool expandFlag = manualExpand ? item->expand() : !item->expand();
   item->setExpand(expandFlag);
    for(quint32 i = index.row()+1;i <= index.row()+item->rowHidden(); i++)
        mySectionHidden(Qt::Vertical, i, !expandFlag);

    emit updateFrozenViewGeometry();

    setModify(true);
}

void CRecipeGridView::splitGroup()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( 1 != selectList.size() )
        return;

    const QModelIndex& index = selectList.at(0);
    CGridBaseItem* item = m_model->getItem(index);
    if(NULL == item || item->rowHidden() <= 0)
        return;

    for(quint32 i = index.row()+1;i <= index.row()+item->rowHidden(); i++)
        mySectionHidden(Qt::Vertical, i,false);

    item->setExpand(true);
    item->setRowHidden(0);

    emit updateFrozenViewGeometry();

    setModify(true);
}

void CRecipeGridView::configRecipeTemplate()
{
    CSetRecipeTemplateDialog templDlg(this);
    if(templDlg.exec() != QDialog::Accepted)
        return;

}

void CRecipeGridView::addRowColumn()
{
    CInsertRCDialog insertRCDialog(this);
    if(insertRCDialog.exec() != QDialog::Accepted)
        return;

    if(0 == insertRCDialog.m_count )
        return;

    if(insertRCDialog.m_row)
        m_model->insertRows(m_model->rowCount(), insertRCDialog.m_count);
    else
        m_model->insertColumns(m_model->columnCount(), insertRCDialog.m_count);

    setModify(true);
}

void CRecipeGridView::deleteRowColumn()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    CDeleteRCDialog deleteDialog(this);
    if(deleteDialog.exec() != QDialog::Accepted)
        return;

    QMap<qint32, qint32> deleteMap;
    foreach(const QModelIndex& index, selectList)
    {
        if(deleteDialog.m_row)
            deleteMap.insert(index.row(), 0);
        else
            deleteMap.insert(index.column(), 0);
    }
    const QList<qint32>& keys = deleteMap.keys();
    QList<qint32>::const_iterator iter = keys.begin();
    typedef QPair<qint32, qint32> indexPair;
    //! 如果删除大量的行列,并且其中有多个连续的行列的话,使用批量删除的api效率更高.所以需要这个list
    //! 将连续的子序列的范围存储到list中,单独的不连续行/列,其所在的pair的second值以-1为标记
    QLinkedList<indexPair> list;
    qint32 counter = *iter;
    list.push_front(indexPair(counter, -1));
    while( iter != keys.end()-1 )
    {
       indexPair& prePair = list.front();
       qint32 nextIndex = *(iter+1);
        //! 如果是连续的数字,计数器加一然后继续
        if( ++counter != nextIndex )
        {
            //! 开始新的一段连续序列
            list.push_front(indexPair(nextIndex, -1));
            counter = nextIndex;
        }
        //! 记录下此段连续序列的右边界
        else if(prePair.first != nextIndex )
             prePair.second = nextIndex;

        ++iter;
    }
    //! 确定最后一个元素的归属
    qint32 lastIndex = *iter;
    indexPair& prePair = list.front();
    if( counter != lastIndex )
        list.push_front(indexPair(lastIndex, -1));
    else if(prePair.first != lastIndex )
         prePair.second = lastIndex;

   foreach(const indexPair& pair, list)
   {
       qint32 index1 = pair.first;
       qint32 index2 = pair.second;
       if(deleteDialog.m_row)
           index2 == -1 ? m_model->removeRow(index1) : m_model->removeRows(index1, index2-index1+1);

       else
           index2 == -1 ? m_model->removeColumn(index1) : m_model->removeColumns(index1, index2-index1+1);
   }

   update();
   setModify(true);
}

void CRecipeGridView::insertRowColumn()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    CInsertRCDialog insertRCDialog(this);
    if(insertRCDialog.exec() != QDialog::Accepted)
        return;

    if(0 == insertRCDialog.m_count )
        return;

    const QModelIndex& index = selectList.at(0);
    if(insertRCDialog.m_row)
        m_model->insertRows(index.row(), insertRCDialog.m_count);
    else
        m_model->insertColumns(index.column(), insertRCDialog.m_count);

    setModify(true);

}

void CRecipeGridView::frozenRowColumn()
{
    CFrozenSetDialog frozenDlg(m_rowFrozen, m_columnFrozen, m_frozenFrame, this);
    if( frozenDlg.exec() != QDialog::Accepted )
        return;

    m_frozenFrame = frozenDlg.m_frameData;
    m_rowFrozen = frozenDlg.m_rowFrozen;
    m_columnFrozen = frozenDlg.m_columnFrozen;

    createForzenViews();

    setModify(true);
}

void CRecipeGridView::setRowHightcolumnWidth()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    QHeaderView* horizontalView = horizontalHeader();
    QHeaderView* verticalView = verticalHeader();

    const QModelIndex& sampleIndex = selectList.at(0);
    CSetHWDialog setHWDialog(verticalView->sectionSize(sampleIndex.row()), this);
    if(setHWDialog.exec() != QDialog::Accepted)
        return;
    if(0 == setHWDialog.m_number )
        return;
    //! 剔除重复的行列索引
    QSet<qint32> selectSet;
    foreach(const QModelIndex& index, selectList)
    {
        qint32 tmpIndex = setHWDialog.m_row ? index.row() : index.column();
        selectSet.insert(tmpIndex);
    }
    foreach(qint32 index, selectSet)
    {
        if( setHWDialog.m_row )
            verticalView->resizeSection(index, setHWDialog.m_number);
        else
            horizontalView->resizeSection(index, setHWDialog.m_number);
    }

    setModify(true);
}

void CRecipeGridView::printConfig()
{
    CPrintConfigDialog printCfgDlg(m_printPara, this);
    if(printCfgDlg.exec() != QDialog::Accepted)
        return;

    m_printPara = printCfgDlg.m_printPara;
    setModify(true);
}

void CRecipeGridView::printPreview()
{
#if defined(QT_DEBUG) && defined(Q_OS_WIN)
    CShareTool::trWarningMessageBox(tr("windows平台下的debug版本暂时不支持打印"), this);
    return;
#endif

    CPrintCloneView* printView = new CPrintCloneView(this);
    printView->init();
    QPrinter printer;
    printer.setCopyCount(1);
    //! windows的debug版本这里会崩溃退出,release版本没有问题.qt自身bug，目前无法解决。
    QPrintPreviewDialog previewDlg(&printer, this);
    CShareTool::translateWidget(&previewDlg);
    connect(&previewDlg, &QPrintPreviewDialog::paintRequested, printView, &CPrintCloneView::doPrint);
    previewDlg.setWindowFlags( previewDlg.windowFlags() | Qt::WindowMaximizeButtonHint);
    previewDlg.setWindowState(Qt::WindowMaximized);

    previewDlg.exec();

    printView->deleteLater();

}

void CRecipeGridView::print()
{
#if defined(QT_DEBUG) && defined(Q_OS_WIN)
    CShareTool::trWarningMessageBox(tr("windows平台下的debug版本暂时不支持打印"), this);
    return;
#endif

    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    CShareTool::translateWidget(&dlg);
    if (dlg.exec() != QDialog::Accepted)
        return;

    CPrintCloneView* printView = new CPrintCloneView(this);
    printView->init();
    printView->doPrint(&printer);

    printView->deleteLater();
}

void CRecipeGridView::copyRegion()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    QModelIndexList ascendRows(selectList);
    std::sort(ascendRows.begin(), ascendRows.end(), &CShareTool::lessThanRow );
    if( !CShareTool::isRegion(ascendRows) )
    {
        CShareTool::trWarningMessageBox(tr("当前选择的不是一个连续的区域"), this);
        return;
    }

    didCopyRegion(ascendRows);

}

void CRecipeGridView::pasteRegion()
{
    if( m_copyIndexs.count() == 0 || m_copyItems.count() == 0 )
        return;

    qint32 firstRow = m_copyIndexs.first().row();
    qint32 lastRow = m_copyIndexs.last().row();
    qint32 firstColumn = m_copyIndexs.first().column();
    qint32 lastColumn = m_copyIndexs.last().column();
    const QModelIndex& current = currentIndex();
    qint32 curRow = current.row();
    qint32 curColumn = current.column();

    //! 如果黏贴的items超出现有table范围了,则增加相应的行列,使得所有的items都可以被黏贴成功
    //! 经过测试,setItem()后qt已经帮我做好了适当的行列增加了
    //! 开始黏贴
    qint32 row,col;
    qint32 cnter = 0;
    for( row = curRow; row <= curRow + lastRow - firstRow; row++ )
    {
       for( col = curColumn; col <= curColumn + lastColumn - firstColumn; col++ )
       {
           CGridBaseItem* destItem = m_model->getItem(m_model->index(row, col));
           const CGridBaseItem* item = m_copyItems.at(cnter);
           if( item )
           {
               CGridBaseItem* newItem = item->clone();
               qDebug("1...expend=%d rowhidden=%d", newItem->expand(), newItem->rowHidden());
               //! 保留原有的item的成组信息
               if( destItem && destItem->rowHidden() > 0 )
               {
                   newItem->setExpand(destItem->expand());
                   newItem->setRowHidden(destItem->rowHidden());
               }

               qDebug("2...expend=%d rowhidden=%d", newItem->expand(), newItem->rowHidden());
               m_model->setItem(row, col, newItem);
           }

           cnter++;
       }
    }

    setModify(true);
}

void CRecipeGridView::replacePasteRegion()
{
    if( m_copyIndexs.count() == 0 || m_copyItems.count() == 0 )
        return;

    CReplaceTextDialog replaceDlg(this);
    if(replaceDlg.exec() != QDialog::Accepted)
        return;
    if( replaceDlg.m_src.isEmpty() )
        return;

    const QModelIndex& current = currentIndex();
    qint32 curRow = current.row();
    qint32 curColumn = current.column();

    didReplacePasteRegion(curRow, curColumn, replaceDlg.m_src, replaceDlg.m_replace);

    setModify(true);
}

void CRecipeGridView::clearText()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    foreach(const QModelIndex& index, selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        if( item )
        {
            item->setData(QString(""), Qt::DisplayRole);
            item->setData(QString(""), Qt::EditRole);
        }
    }

    setModify(true);
}

void CRecipeGridView::setTableViewProperty()
{
    CPropertyDialog propertyDlg(m_propertyPara, this);
    if(propertyDlg.exec() != QDialog::Accepted)
        return;
    m_propertyPara = propertyDlg.m_property;
    updateProperty();

    setModify(true);
}

void CRecipeGridView::itemEditFinishEvent()
{
    if( CShareTool::m_isStudioMode )
        return;

    Q_ASSERT(m_mainForm);

    QModelIndex index = currentIndex();
    if(!index.isValid())
        index = indexAt(m_clickPosition);
    if( !index.isValid() )
        return;

    QString text = m_model->data(index).toString();
    QList<QVariant>paras;
    paras << index.row() << index.column() << text << false;
    //qDebug("edited finish row=%d col=%d", index.row(), index.column());
    m_mainForm->run()->fireEvent(m_jsObjName, sEditFinish, paras);
}

void CRecipeGridView::itemClickEvent(QPoint pt, QMouseEvent *event)
{
    if( CShareTool::m_isStudioMode )
        return;

    QModelIndex index = indexAt(pt);
    if( index.isValid() )
    {
        bool updn;
        QString jsCommand;
        updn = event->type() == QEvent::MouseButtonPress ? true : false;

        if( event->button() == Qt::LeftButton )
            jsCommand = sLeftButtonClicked;
        else if(  event->button() == Qt::RightButton )
            jsCommand = sRightButtonClicked;

        QList<QVariant> paras;
        paras << (qint32)index.row() << (qint32)index.column() << updn;//1:按下 0:弹起
        m_mainForm->run()->fireEvent(m_jsObjName, jsCommand, paras);
        //qDebug("js cmd = %s name = %s...row=%d col=%d updn = %d", qPrintable(jsCommand), qPrintable(m_jsObjName), index.row(), index.column(), updn);
    }
}

void CRecipeGridView::didCopyRegion(const QModelIndexList &selectList)
{
    //! 释放items的内存
    qDeleteAll(m_copyItems);
    m_copyItems.clear();
    m_copyIndexs.clear();
    foreach(const QModelIndex& index, selectList)
    {
        CGridBaseItem* item = m_model->getItem(index);
        CGridBaseItem* cloneItem = item ? item->clone() : NULL;
        m_copyItems.append(cloneItem);
    }

    m_copyIndexs = selectList;
}

void CRecipeGridView::didReplacePasteRegion(qint32 curRow, qint32 curColumn, QString strSrc, QString strDes)
{
    if( m_copyIndexs.count() == 0 || m_copyItems.count() == 0 )
        return;

    qint32 firstRow = m_copyIndexs.first().row();
    qint32 lastRow = m_copyIndexs.last().row();
    qint32 firstColumn = m_copyIndexs.first().column();
    qint32 lastColumn = m_copyIndexs.last().column();

    qint32 row,col;
    qint32 cnter = 0;
    for( row = curRow; row <= curRow + lastRow - firstRow; row++ )
    {
       for( col = curColumn; col <= curColumn + lastColumn - firstColumn; col++ )
       {
           const CGridBaseItem* item = m_copyItems.at(cnter);
           cnter++;
           if(NULL == item)
               continue;

           QString newText = item->text();
           newText.replace(strSrc, strDes);
           CGridBaseItem* newItem = item->clone();
           //! 开发环境或者运行环境下的普通单元格才能替换
           if( CShareTool::m_isStudioMode || item->type() == itemType::baseItem )
           {
               if( newText.compare(item->text()) )
               {
                   newItem->setData(newText, Qt::DisplayRole);
                   newItem->setData(newText, Qt::EditRole);
               }
           }

           //! 保留原有的item的成组信息
           QModelIndex idx = m_model->index(row, col);
           CGridBaseItem* oldItem = m_model->getItem(idx);
           if( oldItem && oldItem->rowHidden() > 0 )
           {
               newItem->setExpand(oldItem->expand());
               newItem->setRowHidden(oldItem->rowHidden());
           }

           m_model->setMyItem(row, col, newItem);
       }
    }
}

void CRecipeGridView::initMenu()
{
    if(!CShareTool::m_isStudioMode)
        return;

    m_menu = new QMenu(this);

    //! 为每个action加入索引,方便随时获取
    qint32 actionIndexCnter = 0;

    QMenu* menu1 = new QMenu(tr("数据源"),this);
    m_menu->addMenu(menu1);
    QAction* action1_1 = new QAction(tr("单元格"), this);
    action1_1->setData(actionIndexCnter++);
    m_actionList.append(action1_1);
    menu1->addAction(action1_1);
    QObject::connect(action1_1, &QAction::triggered,this,&CRecipeGridView::configItem);
    QAction* action1_2 = new QAction(tr("多单元格"), this);
    action1_2->setData(actionIndexCnter++);
    m_actionList.append(action1_2);
    menu1->addAction(action1_2);
    QObject::connect(action1_2, &QAction::triggered,this,&CRecipeGridView::configMulitItems);
    QAction* action1_3 = new QAction(tr("设置配方"), this);
    action1_3->setData(actionIndexCnter++);
    m_actionList.append(action1_3);
    menu1->addAction(action1_3);
    QObject::connect(action1_3, &QAction::triggered,this,&CRecipeGridView::setRecipeName);
    QAction* action1_4 = new QAction(tr("数据源类型"), this);
    action1_4->setData(actionIndexCnter++);
    m_actionList.append(action1_4);
    menu1->addAction(action1_4);
    QObject::connect(action1_4, &QAction::triggered,this,&CRecipeGridView::setDataSource);

    QMenu* menu2 = new QMenu(tr("单元格属性"),this);
    m_menu->addMenu(menu2);
    QAction* action2_1 = new QAction(tr("字体"), this);
    action2_1->setData(actionIndexCnter++);
    m_actionList.append(action2_1);
    menu2->addAction(action2_1);
    QObject::connect(action2_1, &QAction::triggered,this,&CRecipeGridView::setItemFont);
    QAction* action2_2 = new QAction(tr("背景"), this);
    action2_2->setData(actionIndexCnter++);
    m_actionList.append(action2_2);
    menu2->addAction(action2_2);
    QObject::connect(action2_2, &QAction::triggered,this,&CRecipeGridView::setItemBackground);
    QAction* action2_3 = new QAction(tr("边框"), this);
    action2_3->setData(actionIndexCnter++);
    m_actionList.append(action2_3);
    menu2->addAction(action2_3);
    QObject::connect(action2_3, &QAction::triggered,this,&CRecipeGridView::setItemFrame);
    QAction* action2_4 = new QAction(tr("对齐方式"), this);
    action2_4->setData(actionIndexCnter++);
    m_actionList.append(action2_4);
    menu2->addAction(action2_4);
    QObject::connect(action2_4, &QAction::triggered,this,&CRecipeGridView::setItemAlign);
    QAction* action2_5 = new QAction(tr("合并单元格"), this);
    action2_5->setData(actionIndexCnter++);
    m_actionList.append(action2_5);
    menu2->addAction(action2_5);
    QObject::connect(action2_5, &QAction::triggered,this,&CRecipeGridView::mergeItem);
    QAction* action2_6 = new QAction(tr("拆分单元格"), this);
    action2_6->setData(actionIndexCnter++);
    m_actionList.append(action2_6);
    menu2->addAction(action2_6);
    QObject::connect(action2_6, &QAction::triggered,this,&CRecipeGridView::splitItem);

    QMenu* menu3 = new QMenu(tr("行列管理"),this);
    m_menu->addMenu(menu3);
    QAction* action3_1 = new QAction(tr("添加行/列"), this);
    action3_1->setData(actionIndexCnter++);
    m_actionList.append(action3_1);
    menu3->addAction(action3_1);
    QObject::connect(action3_1, &QAction::triggered,this,&CRecipeGridView::addRowColumn);
    QAction* action3_2 = new QAction(tr("删除行/列"), this);
    action3_2->setData(actionIndexCnter++);
    m_actionList.append(action3_2);
    menu3->addAction(action3_2);
    QObject::connect(action3_2, &QAction::triggered,this,&CRecipeGridView::deleteRowColumn);
    QAction* action3_3 = new QAction(tr("插入行/列"), this);
    action3_3->setData(actionIndexCnter++);
    m_actionList.append(action3_3);
    menu3->addAction(action3_3);
    QObject::connect(action3_3, &QAction::triggered,this,&CRecipeGridView::insertRowColumn);
    QAction* action3_4 = new QAction(tr("固定行/列"), this);
    action3_4->setData(actionIndexCnter++);
    m_actionList.append(action2_4);
    menu3->addAction(action3_4);
    QObject::connect(action3_4, &QAction::triggered,this,&CRecipeGridView::frozenRowColumn);
    QAction* action3_5 = new QAction(tr("行高/列宽"), this);
    action3_5->setData(actionIndexCnter++);
    m_actionList.append(action3_5);
    menu3->addAction(action3_5);
    QObject::connect(action3_5, &QAction::triggered,this,&CRecipeGridView::setRowHightcolumnWidth);

    QMenu* menu4 = new QMenu(tr("组"),this);
    m_menu->addMenu(menu4);
    QAction* action4_1 = new QAction(tr("成组"), this);
    m_actionList.append(action4_1);
    menu4->addAction(action4_1);
    QObject::connect(action4_1, &QAction::triggered,this,&CRecipeGridView::setGroup);
    QAction* action4_2 = new QAction(tr("解组"), this);
    action4_2->setData(actionIndexCnter++);
    m_actionList.append(action4_2);
    menu4->addAction(action4_2);
    QObject::connect(action4_2, &QAction::triggered,this,&CRecipeGridView::splitGroup);

    QMenu* menu5 = new QMenu(tr("配方模板"),this);
    m_menu->addMenu(menu5);
    QAction* action5_1 = new QAction(tr("设置模板"),this);
    action5_1->setData(actionIndexCnter++);
    m_actionList.append(action5_1);
    menu5->addAction(action5_1);
    QObject::connect(action5_1, &QAction::triggered,this,&CRecipeGridView::configRecipeTemplate);

    QMenu* menu6 = new QMenu(tr("多语言"),this);
    m_menu->addMenu(menu6);
    QAction* action6_1 = new QAction(tr("启用"),this);
    action6_1->setData(actionIndexCnter++);
    m_actionList.append(action6_1);
    menu6->addAction(action6_1);
    QObject::connect(action6_1, &QAction::triggered,this,&CRecipeGridView::enableMultiLanguage);
    QAction* action6_2 = new QAction(tr("禁用"),this);
    action6_2->setData(actionIndexCnter++);
    m_actionList.append(action6_2);
    menu6->addAction(action6_2);
    QObject::connect(action6_2, &QAction::triggered,this,&CRecipeGridView::disableMultiLanguage);
    action6_1->setCheckable(true);
    action6_2->setCheckable(true);
    m_enableMultiLanguage = action6_1;
    m_disableMultiLanguage = action6_2;
    QActionGroup* group = new QActionGroup(menu6);
    group->addAction(m_enableMultiLanguage);
    group->addAction(m_disableMultiLanguage);
    multiLanguage() ? m_enableMultiLanguage->setChecked(true) : m_disableMultiLanguage->setChecked(true);

    QMenu* menu7 = new QMenu(tr("打印"),this);
    m_menu->addMenu(menu7);
    QAction* action7_1 = new QAction(tr("配置"), this);
    action7_1->setData(actionIndexCnter++);
    m_actionList.append(action7_1);
    menu7->addAction(action7_1);
    QObject::connect(action7_1, &QAction::triggered,this,&CRecipeGridView::printConfig);
    QAction* action7_2 = new QAction(tr("预览"), this);
    action7_2->setData(actionIndexCnter++);
    m_actionList.append(action7_2);
    menu7->addAction(action7_2);
    QObject::connect(action7_2, &QAction::triggered,this,&CRecipeGridView::printPreview);
    QAction* action7_3 = new QAction(tr("打印"), this);
    action7_3->setData(actionIndexCnter++);
    m_actionList.append(action7_3);
    menu7->addAction(action7_3);
    QObject::connect(action7_3, &QAction::triggered,this,&CRecipeGridView::print);

    QAction* action8 = new QAction(tr("复制区域"), this);
    action8->setData(actionIndexCnter++);
    m_actionList.append(action8);
    m_menu->addAction(action8);
    QObject::connect(action8, &QAction::triggered,this,&CRecipeGridView::copyRegion);
    QMenu* menu9 = new QMenu(tr("粘贴区域"),this);
    m_menu->addMenu(menu9);
    QAction* action9_1 = new QAction(tr("粘贴"), this);
    action9_1->setData(actionIndexCnter++);
    m_actionList.append(action9_1);
    menu9->addAction(action9_1);
    QObject::connect(action9_1, &QAction::triggered,this,&CRecipeGridView::pasteRegion);
    QAction* action9_2 = new QAction(tr("替换粘贴"), this);
    action9_2->setData(actionIndexCnter++);
    m_actionList.append(action9_2);
    menu9->addAction(action9_2);
    QObject::connect(action9_2, &QAction::triggered,this,&CRecipeGridView::replacePasteRegion);
    QAction* action10 = new QAction(tr("清除文本"), this);
    action10->setData(actionIndexCnter++);
    m_actionList.append(action10);
    m_menu->addAction(action10);
    QObject::connect(action10, &QAction::triggered,this,&CRecipeGridView::clearText);
    QAction* action11 = new QAction(tr("属性"), this);
    action11->setData(actionIndexCnter++);
    m_actionList.append(action11);
    m_menu->addAction(action11);
    QObject::connect(action11, &QAction::triggered,this,&CRecipeGridView::setTableViewProperty);

}

void CRecipeGridView::showAllEditor()
{
    if(CShareTool::m_isStudioMode)
        return;

    for( qint32 row = 0; row < m_model->rowCount(); ++row )
    {
        for( qint32 col = 0; col < m_model->columnCount(); ++col )
        {
            CGridBaseItem* theItem = m_model->getItem(m_model->index(row, col));

            if( theItem && theItem->canShowPersistentEditor() )
            {
                QModelIndex index = m_model->index(row,col);
                //! 如果不是固定行列范围内的item,直接显示editor即可
                openPersistentEditor(index);
                emit showEditorFromFrozenView(index);
            }
        }
    }


}

void CRecipeGridView::spanMap(SpanMap &spanMap)
{
    QSet<IndexPair> spanedItems;
    for( qint32 row = 0; row < m_model->rowCount(); row++ )
    {
        for( qint32 col = 0; col < m_model->columnCount(); col++ )
        {
            qint32 spanRow = rowSpan(row, col);
            qint32 spanColumn = columnSpan(row, col);
            //! 不记录span均为1的正常单元格
            if( spanRow > 1 || spanColumn > 1  )
            {
                IndexPair index(row,col);
                if( spanedItems.contains(index) )
                {
                    spanedItems.remove(index);
                    continue;
                }
                spanMap.insert(IndexPair(row,col), SpanPair(spanRow, spanColumn));
                for( qint32 row2 = row; row2 < row+spanRow; row2++ )
                {
                    for( qint32 col2 = col; col2 < col+spanColumn; col2++ )
                    {
                        if( row2 != row || col2 != col )
                            spanedItems.insert(IndexPair(row2,col2));
                    }
                }
            }
        }
    }
}

void CRecipeGridView::setSpanForItems(const SpanMap &spanMap)
{
    foreach( const IndexPair& index, spanMap.keys() )
    {
        SpanPair span = spanMap.value(index);
        qDebug("span--%d %d %d %d", index.first, index.second, span.first, span.second);
        mySetSpan(index.first, index.second, span.first, span.second);
    }
}

void CRecipeGridView::mySetSpan(int row, int column, int rowSpan, int columnSpan)
{
    QModelIndex index = m_model->index(row, column);
    if( !index.isValid() )
        return;

    setSpan(row, column, rowSpan, columnSpan);
    if( isItemFrozen(index) )
        emit setSpanChanged(row, column, rowSpan, columnSpan);
}

void CRecipeGridView::diffSectionSize(Qt::Orientation orientation, SectionSizeMap& sections)
{
    QHeaderView* headerView;
    qint32 count,defaultSize;
    if( Qt::Horizontal == orientation )
    {
        headerView = horizontalHeader();
        count = model()->columnCount();
    }
    else
    {
        headerView =  verticalHeader();
        count = model()->rowCount();
    }
    defaultSize = headerView->defaultSectionSize();

    for( qint32 i = 0;  i < count; i++ )
    {
        qint32 size = headerView->sectionSize(i);
        if( defaultSize != size )
        {
            //! 因为成组功能导致某行列隐藏了,其宽度是0.所以要先显示,更新值,在隐藏回去.
            if( 0 == size && headerView->isSectionHidden(i) )
            {
                mySectionHidden(Qt::Vertical, i, false);
                size = headerView->sectionSize(i);
                mySectionHidden(Qt::Vertical, i, true);
            }
            if( size > 0 )
                sections.insert(i, size);
        }

    }
}

void CRecipeGridView::refreshGroups()
{
    for( qint32 row = 0; row < m_model->rowCount(); row++ )
    {
        for( qint32 col = 0; col < m_model->columnCount(); col++ )
        {
            CGridBaseItem* item = m_model->getItem(m_model->index(row, col));
            if( item && item->rowHidden() > 0 )
            {
                QModelIndex index = m_model->index(row, col);
                expandCollapseGroup(index, true);
            }
        }
    }
}

void CRecipeGridView::serialize(QJsonObject &json)
{
    //! tableview的json
    QJsonObject& viewjson = json;

    if( g_cInitRowNum != m_model->rowCount() )
        viewjson.insert("RowCount", (qint32)m_model->rowCount());
    if( g_cInitColumnNum != m_model->columnCount() )
        viewjson.insert("ColumnCount", (qint32)m_model->columnCount());
    //! 打印参数
    PrintPara defaultPrint;
    CPrintConfigDialog::setDefault(defaultPrint);
    if( defaultPrint.printHorizontalView != m_printPara.printHorizontalView )
        viewjson.insert("PrintHorizontal", m_printPara.printHorizontalView);
    if( defaultPrint.printVerticalView != m_printPara.printVerticalView )
        viewjson.insert("PrintVertical", m_printPara.printVerticalView);
    if( defaultPrint.printScale != m_printPara.printScale )
        viewjson.insert("PrintScale", m_printPara.printScale);
    if( defaultPrint.printTop != m_printPara.printTop )
        viewjson.insert("PrintTop", m_printPara.printTop);
    if( defaultPrint.printBottom != m_printPara.printBottom )
        viewjson.insert("PrintBottom", m_printPara.printBottom);
    if( defaultPrint.printLeft != m_printPara.printLeft )
        viewjson.insert("PrintLeft", m_printPara.printLeft);
    if( defaultPrint.printRight != m_printPara.printRight )
        viewjson.insert("PrintRight", m_printPara.printRight);
    if( defaultPrint.printHeader != m_printPara.printHeader )
        viewjson.insert("PrintHeader", m_printPara.printHeader);
    if( defaultPrint.printFooter != m_printPara.printFooter )
        viewjson.insert("PrintFooter", m_printPara.printFooter);
    if( defaultPrint.printLeftHeader != m_printPara.printLeftHeader )
        viewjson.insert("PrintLeftHeader", m_printPara.printLeftHeader);
    if( defaultPrint.printMiddleHeader != m_printPara.printMiddleHeader )
        viewjson.insert("PrintMiddleHeader", m_printPara.printMiddleHeader);
    if( defaultPrint.printRightHeader != m_printPara.printRightHeader )
        viewjson.insert("PrintRightHeader", m_printPara.printRightHeader);
    if( defaultPrint.printLeftFooter != m_printPara.printLeftFooter )
        viewjson.insert("PrintLeftFooter", m_printPara.printLeftFooter);
    if( defaultPrint.printMiddleFooter != m_printPara.printMiddleFooter )
        viewjson.insert("PrintMiddleFooter", m_printPara.printMiddleFooter);
    if( defaultPrint.printRightFooter != m_printPara.printRightFooter )
        viewjson.insert("PrintRightFooter", m_printPara.printRightFooter);
    if( defaultPrint.printDateFormat != m_printPara.printDateFormat )
        viewjson.insert("PrintDateFormat", m_printPara.printDateFormat);
    if( defaultPrint.printTimeFormat != m_printPara.printTimeFormat )
        viewjson.insert("PrintTimeFormat", m_printPara.printTimeFormat);
    //! gridView视图相关的基本属性
    PropertyPara defaultProperty;
    setDefaultProperty(defaultProperty);
    if( defaultProperty.showGrid != m_propertyPara.showGrid )
        viewjson.insert("ShowGrid", m_propertyPara.showGrid);
    if( defaultProperty.highLight != m_propertyPara.highLight )
        viewjson.insert("HighLight", m_propertyPara.highLight);
    if( defaultProperty.multiLanguage != m_propertyPara.multiLanguage )
        viewjson.insert("MultiLanguage", m_propertyPara.multiLanguage);
    if( defaultProperty.softKeyboard != m_propertyPara.softKeyboard )
        viewjson.insert("SoftKeyboard", m_propertyPara.softKeyboard);
    if( defaultProperty.clickEdit != m_propertyPara.clickEdit )
        viewjson.insert("ClickEdit", m_propertyPara.clickEdit);
    if( defaultProperty.verticalHeaderWidth != m_propertyPara.verticalHeaderWidth )
        viewjson.insert("VerHeaderWidth", m_propertyPara.verticalHeaderWidth);
    if( defaultProperty.horizontalHeaderHeight != m_propertyPara.horizontalHeaderHeight )
        viewjson.insert("HorHeaderHeight", m_propertyPara.horizontalHeaderHeight);
    //! 合并单元格相关
    SpanMap spans;
    spanMap(spans);
    QJsonArray spanArray;
    foreach( const IndexPair& index, spans.keys() )
    {
        spanArray.append(index.first);
        spanArray.append(index.second);
        spanArray.append(spans.value(index).first);
        spanArray.append(spans.value(index).second);
    }
    if( spanArray.size() > 0 )
        viewjson.insert("SpanArray", spanArray);
    //! 保存与默认值不同的行列宽度
    SectionSizeMap horizontalSections;
    SectionSizeMap verticalSections;
    diffSectionSize(Qt::Horizontal, horizontalSections);
    diffSectionSize(Qt::Vertical, verticalSections);
    QJsonArray sectionHorArray;
    foreach( qint32 index, horizontalSections.keys() )
    {
        sectionHorArray.append(index);
        sectionHorArray.append(horizontalSections.value(index));
    }
    if( sectionHorArray.size() > 0 )
        viewjson.insert("SectionHorArray", sectionHorArray);
    QJsonArray sectionVerArray;
    foreach( qint32 index, verticalSections.keys() )
    {
        sectionVerArray.append(index);
        sectionVerArray.append(verticalSections.value(index));
    }
    if( sectionVerArray.size() > 0 )
        viewjson.insert("SectionVerArray", sectionVerArray);
    //! 固定行列相关
    if( m_rowFrozen > 0 )
        viewjson.insert("FrozenRow", m_rowFrozen);
    if( m_columnFrozen > 0 )
        viewjson.insert("FrozenColumn", m_columnFrozen);
    if( m_rowFrozen > 0 || m_columnFrozen > 0 )
    {
        if(m_frozenFrame.type != frameType::noframe)
            viewjson.insert("FrozenFrameType", (qint32)m_frozenFrame.type);
        if( m_frozenFrame.type != frameType::noframe && QPen() != m_frozenFrame.pen )
            viewjson.insert("FrozenFramePen", CMDJsonConvert::toJson(m_frozenFrame.pen));
    }

    //! model数据实体序列化
    m_model->serialize(viewjson);
}

void CRecipeGridView::deserialize(const QJsonObject &json)
{
    int i;

    QJsonValue jvalue;
    //! 成员变量
    jvalue = json.value("RowCount");
    if( jvalue.type() != QJsonValue::Undefined )
        m_model->setRowCount(jvalue.toInt());
    jvalue = json.value("ColumnCount");
    if( jvalue.type() != QJsonValue::Undefined )
        m_model->setColumnCount(jvalue.toInt());

    //! 打印参数
    jvalue = json.value("PrintHorizontal");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printHorizontalView = jvalue.toBool();
    jvalue = json.value("PrintVertical");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printVerticalView = jvalue.toBool();
    jvalue = json.value("PrintScale");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printScale = jvalue.toBool();
    jvalue = json.value("PrintTop");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printTop = jvalue.toInt();
    jvalue = json.value("PrintBottom");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printBottom = jvalue.toInt();
    jvalue = json.value("PrintLeft");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printLeft = jvalue.toInt();
    jvalue = json.value("PrintRight");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printRight = jvalue.toInt();
    jvalue = json.value("PrintHeader");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printHeader = jvalue.toInt();
    jvalue = json.value("PrintFooter");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printFooter = jvalue.toInt();
    jvalue = json.value("PrintLeftHeader");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printLeftHeader = jvalue.toString();
    jvalue = json.value("PrintMiddleHeader");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printMiddleHeader = jvalue.toString();
    jvalue = json.value("PrintRightHeader");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printRightHeader = jvalue.toString();
    jvalue = json.value("PrintLeftFooter");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printLeftFooter = jvalue.toString();
    jvalue = json.value("PrintMiddleFooter");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printMiddleFooter = jvalue.toString();
    jvalue = json.value("PrintRightFooter");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printRightFooter = jvalue.toString();
    jvalue = json.value("PrintDateFormat");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printDateFormat = jvalue.toString();
    jvalue = json.value("PrintTimeFormat");
    if( jvalue.type() != QJsonValue::Undefined )
        m_printPara.printTimeFormat = jvalue.toString();

    //! gridView视图相关的基本属性
    jvalue = json.value("ShowGrid");
    if( jvalue.type() != QJsonValue::Undefined )
        m_propertyPara.showGrid = jvalue.toBool();
    jvalue = json.value("HighLight");
    if( jvalue.type() != QJsonValue::Undefined )
        m_propertyPara.highLight = jvalue.toBool();
    jvalue = json.value("MultiLanguage");
    if( jvalue.type() != QJsonValue::Undefined )
        m_propertyPara.multiLanguage = jvalue.toBool();
    jvalue = json.value("SoftKeyboard");
    if( jvalue.type() != QJsonValue::Undefined )
        m_propertyPara.softKeyboard = jvalue.toBool();
    jvalue = json.value("ClickEdit");
    if( jvalue.type() != QJsonValue::Undefined )
        m_propertyPara.clickEdit = jvalue.toBool();
    jvalue = json.value("VerHeaderWidth");
    if( jvalue.type() != QJsonValue::Undefined )
        m_propertyPara.verticalHeaderWidth = jvalue.toInt();
    jvalue = json.value("HorHeaderHeight");
    if( jvalue.type() != QJsonValue::Undefined )
        m_propertyPara.horizontalHeaderHeight = jvalue.toInt();

    updateProperty();
    //! 合并单元格相关
    SpanMap spans;
    QJsonArray spanArray = json.value("SpanArray").toArray(QJsonArray());
    for( i = 0; i < spanArray.size(); i+=4 )
    {
        IndexPair index(spanArray[i].toInt(), spanArray[i+1].toInt());
        SpanPair span(spanArray[i+2].toInt(), spanArray[i+3].toInt());
        spans.insert(index, span);
    }
    setSpanForItems(spans);
    //! 设置与默认值不同的行列宽度
    qint32 index,size;
    //! 先横表头
    QJsonArray sectionHorArray = json.value("SectionHorArray").toArray(QJsonArray());
    for(i = 0; i < sectionHorArray.size(); i+=2)
    {
        index = sectionHorArray[i].toInt();
        size = sectionHorArray[i+1].toInt();
        if( index >= model()->columnCount() )
            continue;
        horizontalHeader()->resizeSection(index, size);
    }
    //! 然后是纵表头
    QJsonArray sectionVerArray = json.value("SectionVerArray").toArray(QJsonArray());
    for(i = 0; i < sectionVerArray.size(); i+=2)
    {
        index = sectionVerArray[i].toInt();
        size = sectionVerArray[i+1].toInt();
        if( index >= model()->rowCount() )
            continue;
        verticalHeader()->resizeSection(index, size);
    }

    //! 固定行列相关
    jvalue = json.value("FrozenRow");
    if( jvalue.type() != QJsonValue::Undefined )
        m_rowFrozen = jvalue.toInt();
    jvalue = json.value("FrozenColumn");
    if( jvalue.type() != QJsonValue::Undefined )
        m_columnFrozen = jvalue.toInt();
    jvalue = json.value("FrozenFrameType");
    if( jvalue.type() != QJsonValue::Undefined )
        m_frozenFrame.type = (frameType)jvalue.toInt();
    jvalue = json.value("FrozenFramePen");
    if( jvalue.type() != QJsonValue::Undefined )
        m_frozenFrame.pen = CMDJsonConvert::toQPen(jvalue);


    //! model数据实体序列化
    m_model->deserialize(json);
}

CRecipeGridView &CRecipeGridView::operator=(CRecipeGridView &other)
{
    QJsonObject json;
    other.serialize(json);
    this->deserialize(json);
    return *this;
}

void CRecipeGridView::getExpressionList(QList<int> &indexs, QList<QString> &exps)
{
    m_model->getExpressionList(indexs, exps);
}

void CRecipeGridView::setValue(const qint32 index, QVariant variant)
{
    m_model->setValue(index, variant);
}

void CRecipeGridView::changeLanguage()
{
    m_tool->changeLanguage();

    Q_ASSERT(m_model);
    m_model->changeLanguage();
}

void CRecipeGridView::collectWords()
{
    Q_ASSERT(m_model);
    m_model->collectWords();
}

void CRecipeGridView::setDefaultProperty(PropertyPara &property)
{
    property.showGrid = true;
    property.highLight = false;
    property.multiLanguage = false;
    property.softKeyboard = false;
    property.clickEdit = false;
    property.verticalHeaderWidth = g_cInitVerHeaderWidth;
    property.horizontalHeaderHeight = g_cInitHorHeaderHeight;
}

void CRecipeGridView::updateProperty()
{
    QHeaderView* horizontalView = horizontalHeader();
    QHeaderView* verticalView = verticalHeader();

    setShowGrid(m_propertyPara.showGrid);
    emit showGridChanged(m_propertyPara.showGrid);
    m_delegate->setHighLight(m_propertyPara.highLight);

    multiLanguage() ? m_enableMultiLanguage->setChecked(true) : m_disableMultiLanguage->setChecked(true);

    horizontalView->setFixedHeight(m_propertyPara.horizontalHeaderHeight);
    verticalView->setFixedWidth(m_propertyPara.verticalHeaderWidth);
    updateGeometries();
    updateFrozenViewGeometry();

}

void CRecipeGridView::mySectionHidden(Qt::Orientation orientation, int index, bool hide)
{
    QHeaderView* headerView = Qt::Horizontal == orientation ? horizontalHeader() : verticalHeader();

    hide ? headerView->hideSection(index) : headerView->showSection(index);

    emit sectionHiddenChanged(orientation, index, hide);
}

/////////////////////////////////////脚本接口///////////////////////////////////////////
void CRecipeGridView::redrawAll()
{
    update();
}

void CRecipeGridView::redrawCell(qint32 row, qint32 col)
{
    QModelIndex idx = m_model->index(row,col);
    if(idx.isValid())
        update(idx);
}

void CRecipeGridView::redrawRange(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol)
{
    CShareTool::resetIndex(startRow,startCol,endRow,endCol);

    for( qint32 irow = startRow; irow <= endRow; irow++ )
    {
        for( qint32 icol = startCol; icol <= endCol; icol++ )
        {
            QModelIndex current = m_model->index(irow, icol);
            if( current.isValid() )
                update(current);
        }
    }
}

qint32 CRecipeGridView::getNumberRows()
{
    return m_model->rowCount();
}

void CRecipeGridView::setNumberRows(qint32 count)
{
    if( count > 0 )
        m_model->setRowCount(count);
}

void CRecipeGridView::insertRows(qint32 startRow, qint32 count)
{
    qDebug("js insertRow %d %d", startRow, count);

    if( startRow >= m_model->rowCount() )
        startRow = m_model->rowCount();
    else if(startRow < 0)
        startRow = 0;
    m_model->insertRows(startRow, count);
}

void CRecipeGridView::deleteRows(qint32 startRow, qint32 count)
{
    qint32 rowcount = m_model->rowCount();
    if(startRow >= 0 && startRow < rowcount && count >= 0)
        m_model->removeRows(startRow, count);
}

qint32 CRecipeGridView::getRowHeight(qint32 row)
{
    if( row >= 0 && row < m_model->rowCount() )
        return rowHeight(row);

    return 0;
}

void CRecipeGridView::setRowHeightJs(qint32 row, qint32 height)
{
    if( row >= 0 && row < m_model->rowCount() && height > 0)
        return setRowHeight(row, height);
}

qint32 CRecipeGridView::getNumberCols()
{
    return m_model->columnCount();
}

void CRecipeGridView::setNumberCols(qint32 count)
{
    if( count > 0 )
        m_model->setColumnCount(count);
}

void CRecipeGridView::insertCols(qint32 startCol, qint32 count)
{
    if( startCol >= m_model->columnCount() )
        startCol = m_model->columnCount();
    else if(startCol < 0)
        startCol = 0;
    m_model->insertColumns(startCol, count);
}

void CRecipeGridView::deleteCols(qint32 startCol, qint32 count)
{
    qint32 colcount = m_model->columnCount();
    if(startCol >= 0 && startCol < colcount && count >= 0)
        m_model->removeColumns(startCol, count);
}

qint32 CRecipeGridView::getColWidth(qint32 col)
{
    if( col >= 0 && col < m_model->columnCount() )
        return columnWidth(col);

    return 0;
}

void CRecipeGridView::setColWidth(qint32 col, qint32 width)
{
    if( col >= 0 && col < m_model->columnCount() && width > 0)
        return setColumnWidth(col,  width);
}

void CRecipeGridView::gotoRow(qint32 row)
{
    qint32 currentCol = currentIndex().column();
    if(currentCol == -1)
        currentCol = 0;

    QModelIndex destIndex = m_model->index(row,currentCol);
    if(destIndex.isValid())
    {
        QTableView::scrollTo(destIndex, QAbstractItemView::PositionAtTop);
        setCurrentIndex(destIndex);
    }
}

void CRecipeGridView::gotoCol(qint32 col)
{
    QModelIndex destIndex = m_model->index(currentIndex().row(),col);
    if(destIndex.isValid())
    {
        QTableView::scrollTo(destIndex, QAbstractItemView::PositionAtTop);
        setCurrentIndex(destIndex);
    }
}

QString CRecipeGridView::getCellText(qint32 row, qint32 col)
{
    QModelIndex destIndex = m_model->index(row, col);
    if(destIndex.isValid())
        return m_model->data(destIndex).toString();
    return "";
}

void CRecipeGridView::setCellText(qint32 row, qint32 col, QString text)
{
    QModelIndex destIndex = m_model->index(row, col);
    CGridBaseItem* item = m_model->getItem(destIndex);
    if(destIndex.isValid())
    {
        if( !item )
            m_model->setData(destIndex, text);
        else if(item->type()== itemType::baseItem)
            m_model->setData(destIndex, text);
    }

}

void CRecipeGridView::enableRegionEdit(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, bool enableEdit)
{
    CShareTool::resetIndex(startRow,startCol,endRow,endCol);

    for( qint32 irow = startRow; irow <= endRow; irow++ )
    {
        for( qint32 icol = startCol; icol <= endCol; icol++ )
        {
            QModelIndex current = m_model->index(irow, icol);
            CGridBaseItem* item = m_model->getItem(current, true);
            item->enableEdit(enableEdit, indexWidget(current));
        }
    }
}

void CRecipeGridView::setBackgroundColor(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 red, qint32 green, qint32 blue)
{
    QColor color(red,green,blue);
    QBrush brush(color);
    if(color == Qt::white)
        brush.setStyle(Qt::NoBrush);
    else
        brush.setStyle(Qt::SolidPattern);

    CShareTool::resetIndex(startRow,startCol,endRow,endCol);

    for( qint32 irow = startRow; irow <= endRow; irow++ )
    {
        for( qint32 icol = startCol; icol <= endCol; icol++ )
        {
            QModelIndex current = m_model->index(irow, icol);
            if( current.isValid() )
                m_model->setData(current, brush, Qt::BackgroundRole);
        }
    }
}

void CRecipeGridView::copyAndReplaceRegion(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, qint32 toRow, qint32 toCol, QString strSrc, QString strDes)
{
    CShareTool::resetIndex(startRow,startCol,endRow,endCol);
    QModelIndex topLeft = m_model->index(startRow, startCol);
    QModelIndex bottomRight = m_model->index(endRow,endCol);
    const QModelIndexList& selectList = QItemSelectionRange(topLeft, bottomRight).indexes();

    didCopyRegion(selectList);

    if(toRow < 0)   toRow = 0;
    if(toCol < 0)   toCol = 0;
    didReplacePasteRegion(toRow, toCol, strSrc, strDes);

}

void CRecipeGridView::getRangeRecipeName(qint32 startRow, qint32 startCol, qint32 endRow, qint32 endCol, QStringList &nameArray)
{
    nameArray.clear();
    CShareTool::resetIndex(startRow,startCol,endRow,endCol);

    for( qint32 irow = startRow; irow <= endRow; irow++ )
    {
        for( qint32 icol = startCol; icol <= endCol; icol++ )
        {
            QModelIndex current = m_model->index(irow, icol);
            CGridBaseItem* item = m_model->getItem(current);
            if( item == NULL )
                nameArray.append("");
            else
                nameArray.append(item->recipeName());
        }
    }
}



