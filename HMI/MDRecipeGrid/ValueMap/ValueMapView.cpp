#include "ValueMapView.h"
#include <QPushButton>
#include "ValueMapDelegate.h"
#include "ValueMapModel.h"
#include "ShareTool.h"
#include <QEvent>
#include <QMouseEvent>
#include <QHeaderView>


CValueMapView::CValueMapView(QWidget *parent) :
    QTableView(parent)
{
    m_tool = CMDMultiLanguage::createWidgetTool();
    m_tool->init(this);
}

void CValueMapView::init(CValueMapManager::Values &data)
{
    //! UI相关
    CValueMapDelegate* tableDelegate = new CValueMapDelegate( this, tr("请输入内容"), tr("输入不能为空"));
    CValueMapManager::Values& values = data;
    CValueMapModel* mianModel = new CValueMapModel(values.size(), 3, this);
    setModel(mianModel);
    setItemDelegate(tableDelegate);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDragDropOverwriteMode(false);
    verticalHeader()->setHidden(true);
    QHeaderView* header = horizontalHeader();
    qint32 length = width() - 20;
    header->resizeSection(CValueMapDelegate::itemtype::value, length*5/12);
    header->resizeSection(CValueMapDelegate::itemtype::desc, length*5/12);
    header->resizeSection(CValueMapDelegate::itemtype::image, length*2/12);
    //!table数据源赋值
    for( qint32 row = 0; row < values.size(); ++row )
    {
        QModelIndex index1 = mianModel->index(row, CValueMapDelegate::itemtype::value);
        mianModel->setData(index1, values.at(row).first);
        QModelIndex index2 = mianModel->index(row, CValueMapDelegate::itemtype::desc);
        mianModel->setData(index2, values.at(row).second);
        setImageButtonItem(row);
    }

    m_tool->changeLanguage();
}

void CValueMapView::deleteSelectedRows()
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    QMap<qint32, qint32> deleteRows;
    foreach(const QModelIndex& index, selectList)
        deleteRows.insert(index.row(), index.column());

    QMapIterator<qint32,qint32> reiter(deleteRows);
    reiter.toBack();
    while(reiter.hasPrevious())
    {
        reiter.previous();
        qint32 aa = reiter.key();
        qDebug("row=%d", aa);
        model()->removeRow(reiter.key());
    }

}

void CValueMapView::selectedRows(QList<qint32> &rows)
{
    const QModelIndexList& selectList = selectedIndexes();
    if( selectList.isEmpty() )
        return;

    QMap<qint32, qint32> deleteRows;
    foreach(const QModelIndex& index, selectList)
        deleteRows.insert(index.row(), index.column());

    QMapIterator<qint32,qint32> reiter(deleteRows);
    reiter.toBack();
    while(reiter.hasPrevious())
    {
        reiter.previous();
        rows.append(reiter.key());
    }
}

void CValueMapView::setImageButtonItem(qint32 row)
{
    QPushButton* imageBtn = new QPushButton(this);
    imageBtn->setText("......");
    imageBtn->installEventFilter(this);
    connect(imageBtn, &QPushButton::clicked, this, &CValueMapView::imageSelectButtonClickd);
    QModelIndex index = model()->index(row, CValueMapDelegate::itemtype::image);
    setIndexWidget(index, imageBtn);
}

void CValueMapView::setEnvironment(IDrawEnvrionment *environment)
{
    m_environment = environment;
}

bool CValueMapView::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPoint pt = mouseEvent->globalPos();
        QPoint parentPt = viewport()->mapFromGlobal(pt);
        QModelIndex index = indexAt(parentPt);
        if(index.isValid())
            m_imageButtonClickedRow = index.row();
        else
            m_imageButtonClickedRow = -1;
    }
    return QTableView::eventFilter(obj, event);
}

void CValueMapView::imageSelectButtonClickd()
{
    QString fileName = CShareTool::setlectImageUI(m_environment, this);
    if( fileName.isEmpty() )
        return;
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if( !button )
        return;
    if( m_imageButtonClickedRow != -1 )
    {
        QModelIndex index = model()->index(m_imageButtonClickedRow, CValueMapDelegate::itemtype::desc);
        model()->setData(index, fileName);
    }

}
