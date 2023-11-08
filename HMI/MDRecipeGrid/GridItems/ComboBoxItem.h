/*!
author Y2hvdWps 2016.08.29
section 列表框单元格模型类
*/
#ifndef COMBOBOXITEM_H
#define COMBOBOXITEM_H

#include "GridBaseItem.h"


class CComboBoxItem : public CGridBaseItem
{
    friend class CItemCfgDialog;
public:
    CComboBoxItem();
    virtual ~CComboBoxItem(){}

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate* delegate);
    virtual bool canShowPersistentEditor() {return true;}
    virtual void onInitialization(IDrawEnvrionment *environment);
    virtual int	type() const {return itemType::comboBox;}
    virtual CComboBoxItem *clone() const;
    virtual bool needCustomPaint(QPainter *, const QStyleOptionViewItem &){return true;}
    virtual bool needCustomSetModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index);
    virtual void updateValidUIState();
    virtual bool updateShowString(QVariant value, QWidget* editor=NULL);

    virtual bool canTranslation() {return false;}
protected:
    CComboBoxItem(const CComboBoxItem&) = default;

};

#endif // COMBOBOXITEM_H
