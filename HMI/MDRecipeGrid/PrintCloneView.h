/*!
\author Y2hvdWps 2016.09.18
\section 主view的一个clone版本
详细说明
打印/打印预览功能会使用到，外观和主view保持一致，共享model和delegate。
因为打印时候需要用到render()函数。当有scrollBar的时候，内容无法被完全的render出来。
而且，为了避免干扰到主view。创建一个独立的view再去打印也更合理。打印完后立即删除。
*/
#ifndef PRINTCLONEVIEW_H
#define PRINTCLONEVIEW_H

#include "RecipeGridView.h"
#include <QPainter>

class CPrintCloneView : public CRecipeGridView
{
    Q_OBJECT
public:
    explicit CPrintCloneView(CRecipeGridView *parent);
    virtual ~CPrintCloneView();
    virtual void init(qint32 rowNum=-1, qint32 columnNum=-1); //! 部分初始化操作,与子类实现差异化
    virtual viewType tableViewType(){return viewType::printView;}
signals:

public slots:
    void doPrint(QPrinter *printer);    //! 打印预览的绘制操作

private:
     QRectF m_pageRect;
     QRectF m_docRect;
     QRectF m_rectHeader;
     QRectF m_rectFooter;
     qint32 m_pageCount;

private:
    QString replacePrintString(const QString &source, int count, int page);//!将打印自定义字符串转换为实际字符串
    void printOnOnePage(QPrinter* printer, QPainter& paint, qint32 currentCopy);
    void printFooterAndHeader(QPainter& paint, qint32 currentPage);
    void printAllPage(bool isPainting, QPrinter* printer, QPainter& paint, qint32 currentCopy);

};

#endif // PRINTCLONEVIEW_H
