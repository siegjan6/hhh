/*!
author Y2hvdWps 2016.08.29
section 单选按钮单元格模型类
*/
#ifndef RADIOBUTTONITEM_H
#define RADIOBUTTONITEM_H

#include "GridBaseItem.h"
#include <QButtonGroup>
#include <QGroupBox>
#include "GridRadioGroup.h"

class CRadioButtonItem : public CGridBaseItem
{
    friend class CItemCfgDialog;
    friend class CGridRadioGroup;
public:
    CRadioButtonItem();
    virtual ~CRadioButtonItem(){}
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate* delegate);
    virtual int	type() const {return itemType::radioButton;}
    virtual CRadioButtonItem *clone() const;
    virtual bool needCustomPaint(QPainter *, const QStyleOptionViewItem &){return true;}
    virtual bool needCustomSetModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index);
    virtual bool canShowPersistentEditor() {return true;}
    virtual void onInitialization(IDrawEnvrionment *environment);
    virtual void updateValidUIState();
    virtual bool updateShowString(QVariant value, QWidget* editor=NULL);
    virtual void enableEdit(bool edit, QWidget* editor);
    virtual bool canTranslation() {return false;}
protected:
    CRadioButtonItem(const CRadioButtonItem&) = default;

    QButtonGroup* buttonGroup(QGroupBox* groupbox);
};

#endif // RADIOBUTTONITEM_H
