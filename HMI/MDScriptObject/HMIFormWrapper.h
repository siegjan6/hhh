/*!
\author aGRw 2015.12.07
\section 图形窗体，脚本封装类
*/

#ifndef MDHMIFORMWRAPPER_H
#define MDHMIFORMWRAPPER_H

#if defined(MDHMIFORM_LIBRARY)
#  define MDHMIFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDHMIFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "MDObjectWrapper.h"

class CMDHMIForm;

class MDHMIFORMSHARED_EXPORT CHMIFormWrapper : public CMDObjectWrapper
{
    Q_OBJECT

    Q_PROPERTY(int left READ left WRITE setLeft)
    Q_PROPERTY(int top READ top WRITE setTop)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
    Q_PROPERTY(bool visible READ visible WRITE setVisible)
public:
    CHMIFormWrapper(void* object, const QString& name);
    virtual ~CHMIFormWrapper(){}
//////////////////////////////////////////////////////////////////////////
//property
public Q_SLOTS:
    void setLeft(int value);
    int left();
    void setTop(int value);
    int top();
    void setWidth(int value);
    int width();
    void setHeight(int value);
    int height();
    void setVisible(bool value);
    bool visible();
    void setLayerVisible(int layer, bool value);
    bool layerVisible(int layer);
    void setLayerLocked(int layer, bool value);
    bool layerLocked(int layer);
public Q_SLOTS:
    void close();
    void hide();
    void move(int x, int y);
    void print(bool showPrintDialog);
    void printPreview();
    void printRegion(int x, int y, int w, int h, bool showPrintDialog, bool isFullPage);
    void printObject(const QString& name, bool showPrintDialog, bool isFullPage);
    //动态创建对象
    bool createObject(const QString& origName, const QString& newName);
public:
    CMDHMIForm* object();
};


#endif // MDHMIFORMWRAPPER_H

