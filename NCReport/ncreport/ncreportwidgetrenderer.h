#ifndef REPORTWIDGETRENDERER_H
#define REPORTWIDGETRENDERER_H

#include "ncreportabstractitemrendering.h"
#include "ncreport_global.h"
#include <QWidget>

/*!

section 功能

ncreport绘制自定义内容的抽象类
【继承自类NCReportGraphRenderer】
＃＃＃暂时屏蔽图形／自定义控件的相关功能＃＃＃

*/
class NCREPORTSHARED_EXPORT NCReportWidgetRenderer : public NCReportGraphRenderer
{
public:
    NCReportWidgetRenderer();
    ~NCReportWidgetRenderer();

    virtual void paintItem( QPainter* painter, NCReportOutput* output, const QRectF& rect, const QString& itemdata );
    void setWidget(QWidget* p);
    void setScaleMode(Qt::AspectRatioMode mode = Qt::KeepAspectRatioByExpanding);
    virtual qreal calculateHeightMM( NCReportOutput* output ) const;
    virtual QSizeF calculateSizeMM( NCReportOutput* output, const QSizeF& requested ) const;

    void setDPI(unsigned int dpi = -1);
    unsigned int getDPI();

    void setBaseSize(const QSizeF& base = QSizeF());
    QSizeF baseSize() const;

    QWidget* widget() { return m_widget; }
    Qt::AspectRatioMode scaleMode() const { return m_scaleMode; }

private:
    QWidget* m_widget;
    Qt::AspectRatioMode m_scaleMode;
    unsigned int m_resolution;
    QSizeF m_baseSize;
};

#endif // REPORTWIDGETRENDERER_H
