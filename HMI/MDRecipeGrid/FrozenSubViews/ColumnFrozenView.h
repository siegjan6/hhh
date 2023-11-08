/*!
author Y2hvdWps 2016.08.30
section 固定列视图
*/
#ifndef COLUMNFROZENVIEW_H
#define COLUMNFROZENVIEW_H

#include "AbstractFrozenView.h"


class CColumnFrozenView : public CAbstractFrozenView
{
    Q_OBJECT
public:
    explicit CColumnFrozenView(qint32 columnFrozen, CRecipeGridView *parent = 0);
    virtual ~CColumnFrozenView(){}

    virtual void drawFrame(QPainter& painter);

    virtual viewType tableViewType(){return viewType::columnFrozenView;}
    virtual void updateGeometry();

protected:
    virtual qint32 frozenColumnCount() const {return m_frozenColumn;}
    virtual bool canSectionHidden(Qt::Orientation orientation, int index);
protected:
    qint32 m_frozenColumn;
signals:
protected slots:
    virtual void rowsRemoved(const QModelIndex&, int, int){}

};

#endif // COLUMNFROZENVIEW_H
