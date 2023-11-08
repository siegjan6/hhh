/*!
author Y2hvdWps 2016.08.30
section 固定行视图
*/
#ifndef ROWFROZENVIEW_H
#define ROWFROZENVIEW_H

#include "AbstractFrozenView.h"


class CRowFrozenView : public CAbstractFrozenView
{
    Q_OBJECT
public:
    explicit CRowFrozenView(qint32 rowFrozen, CRecipeGridView *parent = 0);
    virtual ~CRowFrozenView(){}

    virtual void drawFrame(QPainter& painter);

    virtual viewType tableViewType(){return viewType::rowFrozenView;}
    virtual void updateGeometry();

protected:
    virtual qint32 frozenRowCount() const {return m_frozenRow;}
    virtual bool canSectionHidden(Qt::Orientation orientation, int index);
protected:
    qint32 m_frozenRow;

signals:
protected slots:
    virtual void columnsRemoved(const QModelIndex &, int, int){}
};

#endif // ROWFROZENVIEW_H
