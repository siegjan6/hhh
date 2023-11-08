/*!
author Y2hvdWps 2016.08.29
section 图像单元格模型类
*/
#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "GridBaseItem.h"


class CImageItem : public CGridBaseItem
{
    friend class CItemCfgDialog;
public:
    CImageItem();
    virtual ~CImageItem(){}
    virtual int	type() const {return itemType::image;}
    virtual CImageItem *clone() const;
    virtual void onInitialization(IDrawEnvrionment *environment);
    virtual bool needCustomPaint(QPainter *painter, const QStyleOptionViewItem &option);
    virtual void updateValidUIState(){}
    virtual bool updateShowString(QVariant value, QWidget* editor=NULL);

    virtual bool canTranslation() {return false;}

    QString imageName() const { return m_imageName; }
    void setImageName(QString name);
protected:
    CImageItem(const CImageItem&) = default;

protected:
    QString m_imageName;

    QString m_preImageName;
};

#endif // IMAGEITEM_H
