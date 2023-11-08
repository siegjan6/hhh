#include "AbstractFrozenView.h"
#include "ShareTool.h"


CAbstractFrozenView::CAbstractFrozenView(CRecipeGridView *parent) :
    CRecipeGridView(parent)
{
    m_model = static_cast<CRecipeGridModel*>(parent->model());
    setModel(m_model);
    setSelectionModel(parent->selectionModel());
    m_delegate = static_cast<CRecipeGridDelegate*>(parent->itemDelegate());
    setItemDelegate(m_delegate);

    m_parent = parent;

    setFrameStyle(QFrame::NoFrame);

    setAutoScroll(false);
    horizontalScrollBar()->setValue(parent->horizontalScrollBar()->value());
    verticalScrollBar()->setValue(parent->verticalScrollBar()->value());
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    connect(horizontalHeader(), &QHeaderView::sectionResized,
            parent, &CRecipeGridView::horizontalSectionResized);
    connect(verticalHeader(), &QHeaderView::sectionResized,
            parent, &CRecipeGridView::verticalSectionResized);

    connect(parent->horizontalHeader(),&QHeaderView::sectionResized,
            this, &CAbstractFrozenView::horizontalSectionResized);
    connect(parent->verticalHeader(),&QHeaderView::sectionResized,
            this, &CAbstractFrozenView::verticalSectionResized);

    connect(model(), &QAbstractItemModel::rowsRemoved,
            this, &CAbstractFrozenView::rowsRemoved);
    connect(model(), &QAbstractItemModel::columnsRemoved,
            this, &CAbstractFrozenView::columnsRemoved);

    connect(parent, &CRecipeGridView::showGridChanged,
            this, &CAbstractFrozenView::showGridHandler);
    connect(parent, &CRecipeGridView::sectionHiddenChanged,
            this, &CAbstractFrozenView::sectionHiddenHandler);
    connect(parent, &CRecipeGridView::updateFrozenViewGeometry,
            this, &CAbstractFrozenView::updateGeometry);
    connect(parent, &CRecipeGridView::invokeAction,
            this, &CAbstractFrozenView::triggerAction);
    connect(parent, &CRecipeGridView::setSpanChanged,
            this, &CAbstractFrozenView::setSpanHandler);

    connect(parent, &CRecipeGridView::showEditorFromFrozenView,
            this, &CAbstractFrozenView::persistentEditorShowed);

}


void CAbstractFrozenView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
}

void CAbstractFrozenView::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    QWidget* viewPort = viewport();
    QPainter painter(viewPort);

    painter.save();

    //! 绘制固定行列功能的边框
    QPen pen(m_parent->m_frozenFrame.pen);
    painter.setPen(pen);

    drawFrame(painter);

    painter.restore();
}

void CAbstractFrozenView::keyPressEvent(QKeyEvent *event)
{
    int keyValue = event->key();
    switch(keyValue)
    {
        case Qt::Key_Up:
        case Qt::Key_PageUp:
        case Qt::Key_Down:
        case Qt::Key_PageDown:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
        case Qt::Key_Home:
        case Qt::Key_End:
        //! gridview的导航键在frozenViews上面不做处理,交给mainView处理
            event->ignore();
            break;
        default:
            CRecipeGridView::keyPressEvent(event);
            break;
    }
}

void CAbstractFrozenView::contextMenuEvent(QContextMenuEvent *event)
{
    //! 右键菜单全部交给mainGridView处理
    QApplication::postEvent(m_parent->viewport(), new QContextMenuEvent(event->reason(), event->pos(), event->globalPos()));
}

bool CAbstractFrozenView::edit(const QModelIndex &index, QAbstractItemView::EditTrigger trigger, QEvent *event)
{
    return QTableView::edit(index, trigger, event);
}

void CAbstractFrozenView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    QTableView::scrollTo(index, hint);
}

void CAbstractFrozenView::expandCollapseGroup(const QModelIndex &index, bool manualExpand)
{
    m_parent->expandCollapseGroup(index,manualExpand);
}

void CAbstractFrozenView::checkSpanState()
{
    SpanMap spans;
    spanMap(spans);

    foreach( const IndexPair& index, spans.keys() )
    {
        SpanPair span = spans.value(index);
        qint32 row = index.first;
        qint32 col = index.second;
        qint32 spanRow = span.first;
        qint32 spanColumn = span.second;

        if( row >= m_parent->frozenRowCount() || col >= m_parent->frozenColumnCount() )
            continue;

        qint32 newSpanRow = frozenRowCount() - spanRow;
        qint32 newSpanCol = frozenColumnCount() - spanColumn;
        if( spanRow > newSpanRow || spanColumn > newSpanCol )
        {
            newSpanRow = newSpanRow < spanRow ? newSpanRow : spanRow;
            newSpanCol = newSpanCol < spanColumn ? newSpanCol : spanColumn;
            m_parent->mySetSpan(row, col, newSpanRow, newSpanCol);
            qDebug("frozen span......%d %d %d %d", row, col, newSpanRow, newSpanCol);
        }
    }
}


void CAbstractFrozenView::init(qint32 rowNum, qint32 columnNum)
{
    Q_UNUSED(rowNum);
    Q_UNUSED(columnNum);

    commonInit();
}

void CAbstractFrozenView::showGridHandler(bool show)
{
    setShowGrid(show);
}

void CAbstractFrozenView::drawRightLine(QPainter &painter)
{
    qint32 width = horizontalHeader()->length();
    qint32 height = verticalHeader()->length();
    qreal penWidth = painter.pen().width();
    QPointF point3(width, 0);
    QPointF point4(point3.x(), height - penWidth/2.f);
    painter.drawLine(point3, point4);
}

void CAbstractFrozenView::drawBottomLine(QPainter &painter)
{
    QRectF rect = viewport()->rect();
    qint32 width = horizontalHeader()->length();
    qreal penWidth = painter.pen().width();
    QPointF point1(0, rect.bottomLeft().y());
    QPointF point2(width - penWidth/2.f, point1.y());
    painter.drawLine(point1, point2);
}

void CAbstractFrozenView::sectionHiddenHandler(Qt::Orientation orientation, int index, bool hide)
{
    if( !canSectionHidden(orientation, index) )
        return;

    QHeaderView* headerView = Qt::Horizontal == orientation ? horizontalHeader() : verticalHeader();
    hide ? headerView->hideSection(index) : headerView->showSection(index);

    QHeaderView* parentHeader = Qt::Horizontal == orientation ? horizontalHeader() : verticalHeader();
    if( false == hide && headerView->sectionSize(index) != parentHeader->sectionSize(index)  )
    {
        headerView->resizeSection(index, parentHeader->sectionSize(index));
    }
    updateGeometry();
}

void CAbstractFrozenView::rowsRemoved(const QModelIndex &, int first, int last)
{
    qint32 i;
    qint32 rowCount = frozenRowCount();

    for( i = first; i < rowCount; i++ )
    {
        setRowHidden(i, false);
        //! 隐藏状态的row设置rowheight不生效，所以在这里要显示后马上调整大小
        if(rowHeight(i) != m_parent->rowHeight(i))
            setRowHeight(i, m_parent->rowHeight(i));
    }
    for( i = rowCount; i <= last; i++ )
        setRowHidden(i, true);

    updateGeometry();
}

void CAbstractFrozenView::columnsRemoved(const QModelIndex &, int first, int last)
{
    qint32 i;
    qint32 columnCount = frozenColumnCount();

    for( i = first; i < columnCount; i++ )
    {
        setColumnHidden(i, false);
        if(columnWidth(i) != m_parent->columnWidth(i))
            setColumnWidth(i, m_parent->columnWidth(i));
    }
    for( i = columnCount; i <= last; i++ )
        setColumnHidden(i, true);

    updateGeometry();
}

void CAbstractFrozenView::horizontalSectionResized(int logicalIndex, int, int newSize)
{
    if( newSize <= 0 || horizontalHeader()->sectionSize(logicalIndex) == newSize )
        return;

    setColumnWidth(logicalIndex, newSize);
    updateGeometry();
}

void CAbstractFrozenView::verticalSectionResized(int logicalIndex, int, int newSize)
{
    if( newSize <= 0 || verticalHeader()->sectionSize(logicalIndex) == newSize )
        return;

    setRowHeight(logicalIndex, newSize);
    updateGeometry();
}

void CAbstractFrozenView::setSpanHandler(int row, int column, int rowSpan, int columnSpan)
{
    setSpan(row, column, rowSpan, columnSpan);
}

void CAbstractFrozenView::persistentEditorShowed(const QModelIndex &index)
{
    if( !index.isValid() )
        return;

    //! Y2hvdWps null
    //! 只有运行环境下才永久显示editor
    if( !CShareTool::m_isStudioMode )
        openPersistentEditor(index);
}
