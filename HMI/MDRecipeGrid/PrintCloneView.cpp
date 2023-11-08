#include "PrintCloneView.h"
#include <QPrinter>
#include <QHeaderView>
#include <QPixmap>
#include <QRegion>
#include <QDateTime>


CPrintCloneView::CPrintCloneView(CRecipeGridView *parent) :
    CRecipeGridView(parent)
{
    m_model = static_cast<CRecipeGridModel*>(parent->model());
    setModel(m_model);
    m_delegate = static_cast<CRecipeGridDelegate*>(parent->itemDelegate());
    setItemDelegate(m_delegate);

    m_printPara = parent->m_printPara;

    clearSelection();
    setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    adjustSize();
    //! 如果没有show()，render()出来的画面会有缺失
    show();
    setHidden(true);


}

CPrintCloneView::~CPrintCloneView()
{

}

void CPrintCloneView::init(qint32 rowNum, qint32 columnNum)
{
    Q_UNUSED(rowNum);
    Q_UNUSED(columnNum);

    commonInit();
}

QString CPrintCloneView::replacePrintString(const QString &source, int count, int page)
{
    const QString sDate = "&Date";
    const QString sTime = "&Time";
    const QString sPage = "&Page";
    const QString sPageCount = "&Count";

    QDateTime current = QDateTime::currentDateTime();
    QString d = current.toString(m_printPara.printDateFormat);
    QString t = current.toString(m_printPara.printTimeFormat);
    QString p = QString::number(page);
    QString c = QString::number(count);

    QString r = "@";
    QString src = source;
    src.remove(r);

    src.replace(sDate, r, Qt::CaseSensitive);
    src.replace(r, d);
    src.replace(sTime, r, Qt::CaseSensitive);
    src.replace(r, t);
    src.replace(sPage, r, Qt::CaseSensitive);
    src.replace(r, p);
    src.replace(sPageCount, r, Qt::CaseSensitive);
    src.replace(r, c);

    return src;
}

void CPrintCloneView::printOnOnePage(QPrinter* printer, QPainter& paint, qint32 currentCopy)
{
    QHeaderView* horizontalView = horizontalHeader();
    QHeaderView* verticalView = verticalHeader();
    qint32 totalWidth = horizontalView->length();
    qint32 totalHeight = verticalView->length();
    qint32 docWidth = m_docRect.width();
    qint32 docHeight = m_docRect.height();

    qint32 imageWidth=totalWidth+frameWidth();
    if( m_printPara.printVerticalView )
        imageWidth += verticalView->width();
    qint32 imageHeight=totalHeight+frameWidth();
    if( m_printPara.printHorizontalView )
        imageHeight += horizontalView->height();

    if( currentCopy > 0 )
        printer->newPage();

    QPixmap image(imageWidth,imageHeight);
    image.fill(Qt::white);

    render(&image);

    qint32 scaleWidth = imageWidth > docWidth ? docWidth : imageWidth;
    qint32 scaleHeight = imageHeight > docHeight ? docHeight : imageHeight;
    QPixmap newImage = image.scaled(scaleWidth,scaleHeight, Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    paint.drawPixmap(m_docRect.topLeft(),newImage);

    //! 绘制页眉页脚
    printFooterAndHeader(paint, 1);
}

void CPrintCloneView::printFooterAndHeader(QPainter &paint, qint32 currentPage)
{
    QTextOption option;
    option.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    if( !m_printPara.printLeftHeader.isEmpty() )
        paint.drawText(m_rectHeader, replacePrintString(m_printPara.printLeftHeader, m_pageCount, currentPage), option);
    if( !m_printPara.printLeftFooter.isEmpty() )
        paint.drawText(m_rectFooter, replacePrintString(m_printPara.printLeftFooter, m_pageCount, currentPage), option);
    option.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if( !m_printPara.printMiddleHeader.isEmpty() )
        paint.drawText(m_rectHeader, replacePrintString(m_printPara.printMiddleHeader, m_pageCount, currentPage), option);
    if( !m_printPara.printMiddleFooter.isEmpty() )
        paint.drawText(m_rectFooter, replacePrintString(m_printPara.printMiddleFooter, m_pageCount, currentPage), option);
    option.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    if( !m_printPara.printRightHeader.isEmpty() )
        paint.drawText(m_rectHeader, replacePrintString(m_printPara.printRightHeader, m_pageCount, currentPage), option);
    if( !m_printPara.printRightFooter.isEmpty() )
        paint.drawText(m_rectFooter, replacePrintString(m_printPara.printRightFooter, m_pageCount, currentPage), option);
}

void CPrintCloneView::printAllPage(bool isPainting, QPrinter *printer, QPainter &paint, qint32 currentCopy)
{
    QHeaderView* horizontalView = horizontalHeader();
    QHeaderView* verticalView = verticalHeader();
    qint32 totalWidth = horizontalView->length();
    qint32 totalHeight = verticalView->length();
    qint32 docWidth = m_docRect.width();
    qint32 docHeight = m_docRect.height();

    qint32 startDrawY=0,startDrawX=0;
    qint32 rowCnter=0, columnCnter=0;
    qint32 columnNum = model()->columnCount();
    qint32 rowNum = model()->rowCount();

    qint32 currentPage = 0;
    qint32 offsetX=0,offsetY=0;
    //! 根据打印的配置初始化headerX,Y
    qint32 headerX =0, headerY=0;
    if( m_printPara.printVerticalView )
        headerX += verticalView->width();
    if( m_printPara.printHorizontalView )
        headerY += horizontalView->height();

    //! 按照先行后列规则绘制，遇到距离不够一整行/列。则跳到新页绘制，保证行列完整
    offsetY = headerY;//! offsetY复位
    rowCnter = 0;
    while( (rowCnter < rowNum) &&
           (offsetY < totalHeight) )
    {
        offsetX = headerX;//! offsetX复位
        startDrawY = offsetY;//! 记录当前要绘制的初始坐标值y
        while( (rowCnter < rowNum) &&
               (offsetY - startDrawY + headerY <= docHeight) )
            offsetY += rowHeight(rowCnter++);
        rowCnter -= 1;
        //! 如果最后一行无法完整绘制，则offsetY回到上一行，仅绘制到上一行。
        if( //(rowCnter < rowNum) &&
                (offsetY - startDrawY + headerY > docHeight) )
            offsetY -= rowHeight(rowCnter);

        columnCnter = 0;
        while( (columnCnter < columnNum) &&
               (offsetX < totalWidth) )
        {
            startDrawX = offsetX;//! 记录当前要绘制的初始坐标值x
            while( (columnCnter < columnNum) &&
                   (offsetX - startDrawX + headerX <= docWidth) )
            {
                offsetX += columnWidth(columnCnter++);
            }
            columnCnter -= 1;
            //! 如果最后一列无法完整绘制，则offsetX回到上一列，仅绘制到上一列。
            if( //(columnCnter-1 < columnNum) &&
                    (offsetX - startDrawX + headerX > docWidth) )
                offsetX -= columnWidth(columnCnter);

            //! 仅计算总页数，不绘制
            if( false == isPainting )
            {
                m_pageCount += 1;
                continue;
            }

            if(currentPage > 0 || currentCopy > 0)
            {
                printer->newPage();
            }

            //! 绘制当前页
            QPixmap image(docWidth, docHeight);
            image.fill(Qt::white);
            if( m_printPara.printHorizontalView )
            {
                QRegion horizontalViewRegion(startDrawX-headerX,0, offsetX - startDrawX + headerX+frameWidth(), headerY);
                render(&image, QPoint(0,0), horizontalViewRegion, QWidget::DrawChildren);
            }
            if( m_printPara.printVerticalView )
            {
                QRegion verticalViewRegion(0,startDrawY-headerY, headerX, offsetY - startDrawY + headerY+frameWidth());
                render(&image, QPoint(0,0), verticalViewRegion, QWidget::DrawChildren);
            }
            //! 绘制左上角的空白小矩形
            if(m_printPara.printHorizontalView && m_printPara.printVerticalView)
            {
                QRegion rootRectRegion(0,0, headerX, headerY);
                render(&image, QPoint(0,0), rootRectRegion, QWidget::DrawChildren);
            }
            //! 绘制内容
            QRegion mainViewRegion(startDrawX, startDrawY, offsetX - startDrawX+frameWidth(), offsetY - startDrawY+frameWidth());
            render(&image,QPoint(headerX,headerY),mainViewRegion, QWidget::DrawChildren);

            //! 绘制页眉页脚
            printFooterAndHeader(paint, currentPage+1);

            paint.drawPixmap(m_docRect.topLeft(),image);
            currentPage++;
        }
    }
}

void CPrintCloneView::doPrint(QPrinter *printer)
{
    QPainter paint(printer);
    paint.setRenderHint(QPainter::Antialiasing, true);

    QHeaderView* horizontalView = horizontalHeader();
    QHeaderView* verticalView = verticalHeader();
    if( !m_printPara.printHorizontalView )
        horizontalView->setHidden(true);
    if( !m_printPara.printVerticalView )
        verticalView->setHidden(true);

    //! 除去页眉页脚和页边距，剩下的作为文档区域
    m_pageRect = printer->pageRect(QPrinter::DevicePixel);
    m_docRect = QRectF(QPointF(0,0),m_pageRect.size());
    m_docRect.adjust(m_printPara.printLeft, m_printPara.printTop+m_printPara.printHeader,
                   -m_printPara.printRight,-m_printPara.printBottom-m_printPara.printFooter);

    m_rectHeader = QRect(m_printPara.printLeft,
                             m_printPara.printTop,
                             m_pageRect.width() - m_printPara.printLeft - m_printPara.printRight,
                             m_printPara.printHeader);
    m_rectFooter = QRect(m_printPara.printLeft,
                             m_pageRect.height() - m_printPara.printBottom - m_printPara.printFooter,
                             m_rectHeader.width(),
                             m_printPara.printFooter);

    qint32 i;
    //! 表格全部打印在一页中
    if( m_printPara.printScale ){
        m_pageCount = 1;
        for( i = 0; i < printer->copyCount(); i++ )
            printOnOnePage(printer,paint, i);
    }
    else{
        m_pageCount = 0;
        //! 先计算出总页数m_pageCount，后面绘制页眉页脚的时候可能会用到
        printAllPage(false, printer, paint, 1);

        //! 绘制表格
        for( i = 0; i < printer->copyCount(); i++ )
            printAllPage(true, printer, paint, i);

    }
}
