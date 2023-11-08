/*!
author Y2hvdWps 2016.08.29
section 复选框单元格模型类
*/
#ifndef CHECKBOXITEM_H
#define CHECKBOXITEM_H

#include "GridBaseItem.h"

class CCheckBoxItem : public CGridBaseItem
{
    friend class CItemCfgDialog;
    friend class CMulitItemCfgDialog;
public:
    CCheckBoxItem();
    virtual ~CCheckBoxItem(){}
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const CRecipeGridDelegate* delegate);
    virtual bool canShowPersistentEditor() {return true;}
    virtual void onInitialization(IDrawEnvrionment *environment);

    virtual int	type() const {return itemType::checkBox;}
    virtual CCheckBoxItem *clone() const;

    virtual void serialize(QJsonObject& json, CRecipeGridModel* mainModel);
    virtual void deserialize(const QJsonObject& json);

    virtual bool needCustomPaint(QPainter *, const QStyleOptionViewItem &){return true;}
    virtual void updateValidUIState();
    virtual bool updateShowString(QVariant value, QWidget* editor=NULL);

    virtual bool canTranslation() {return false;}
protected:
    CCheckBoxItem(const CCheckBoxItem&) = default;

protected:
    QString m_label;    //! 运行模式下,checkbox的显示文本
};

#endif // CHECKBOXITEM_H
