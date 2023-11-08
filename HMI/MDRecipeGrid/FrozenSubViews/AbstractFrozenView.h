/*!
author Y2hvdWps 2016.08.30
section 固定表格视图虚基类
详细说明:
参考了Qt自带的事例:Frozen Column Example
用于“行列管理”中的“固定行列”功能,此view和主view共享同一个model.
并显示于主view的上面
*/
#ifndef ABSTRACTFROZENVIEW_H
#define ABSTRACTFROZENVIEW_H


#include "RecipeGridView.h"
#include <QHeaderView>
#include <QScrollBar>
#include <QPalette>
#include <QApplication>
#include <QPainter>
#include <QPen>


class CAbstractFrozenView : public CRecipeGridView
{
    Q_OBJECT
public:
    explicit CAbstractFrozenView(CRecipeGridView *parent = 0);
    virtual ~CAbstractFrozenView(){}


    virtual viewType tableViewType() = 0;
    virtual void init(qint32 rowNum=-1, qint32 columnNum=-1);

protected:
    /*!
    事件处理
    */
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void contextMenuEvent(QContextMenuEvent * event);

    virtual qint32 frozenRowCount() const {return 0;}
    virtual qint32 frozenColumnCount() const {return 0;}
protected:
    virtual bool edit(const QModelIndex & index, EditTrigger trigger, QEvent * event);
    virtual void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible);
    virtual void expandCollapseGroup(const QModelIndex& index, bool manualExpand=false);   //!展开/收缩组
    virtual void drawFrame(QPainter& painter) = 0;
    virtual bool canSectionHidden(Qt::Orientation orientation, int index)=0;
    void checkSpanState();
    void drawRightLine(QPainter& painter);
    void drawBottomLine(QPainter& painter);
protected:
    CRecipeGridView *m_parent;

signals:

protected slots:
    void sectionHiddenHandler(Qt::Orientation orientation, int index, bool hide);
    void showGridHandler(bool show);
    virtual void rowsRemoved(const QModelIndex &, int first, int last);
    virtual void columnsRemoved(const QModelIndex &, int first, int last);
    virtual void updateGeometry() = 0;
    void horizontalSectionResized(int logicalIndex, int /*oldSize*/, int newSize);
    void verticalSectionResized(int logicalIndex, int /*oldSize*/, int newSize);
    void setSpanHandler(int row, int column, int rowSpan, int columnSpan);
    void persistentEditorShowed(const QModelIndex & index);
};

#endif // ABSTRACTFROZENVIEW_H
