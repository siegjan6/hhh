/*!
author Y2hvdWps 2016.08.30
section 固定行列视图
详细说明:
当固定的行和列都存在时，此固定视图必须被使用，显示在主视图的左上角。
*/
#ifndef FIXEDFROZENVIEW_H
#define FIXEDFROZENVIEW_H

#include "AbstractFrozenView.h"


class CFixedFrozenView : public CAbstractFrozenView
{
    Q_OBJECT
public:
    explicit CFixedFrozenView(qint32 rowFrozen, qint32 columnFrozen, CRecipeGridView *parent = 0);
    virtual ~CFixedFrozenView(){}

    virtual void drawFrame(QPainter& painter);

    virtual viewType tableViewType(){return viewType::fixedFrozenView;}
    virtual void updateGeometry();

protected:
    virtual qint32 frozenRowCount() const {return m_frozenRow;}
    virtual qint32 frozenColumnCount() const {return m_frozenColumn;}
    virtual bool canSectionHidden(Qt::Orientation orientation, int index);
protected:
    qint32 m_frozenRow;
    qint32 m_frozenColumn;

signals:
public slots:

};

#endif // FIXEDFROZENVIEW_H
