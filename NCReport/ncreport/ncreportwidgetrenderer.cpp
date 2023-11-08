#include "ncreportoutput.h"
#include "ncreportwidgetrenderer.h"
#include <QApplication>
#include <QPainter>
#include <QDesktopWidget>


NCReportWidgetRenderer::NCReportWidgetRenderer() : m_widget(0)
{
    m_scaleMode = Qt::IgnoreAspectRatio;
    m_resolution = -1;
}

NCReportWidgetRenderer::~NCReportWidgetRenderer()
{
}

void NCReportWidgetRenderer::paintItem( QPainter* painter, NCReportOutput* output, const QRectF& rect, const QString& itemdata)
{
    Q_UNUSED(itemdata)

    if (!m_widget)
        return;

    switch ( output->output() ) {
    case NCReportOutput::Printer:
    case NCReportOutput::Pdf:
    case NCReportOutput::Preview:
    case NCReportOutput::QtPreview:
    case NCReportOutput::Image:
    case NCReportOutput::Svg:
        break;
    default:
        return;
    }

    qreal f = output->resolution() / QApplication::desktop()->logicalDpiX();

    // Resize the widget to match our output window
    QSizeF size = m_widget->size();
    QSizeF origSize = size;
    size.scale(rect.size().width()/f, rect.size().height()/f, m_scaleMode);
    m_widget->resize(size.toSize());

    // Because all coordinates passed to the painter are going to be scaled up, we need to scale down the offset
    QPoint off = rect.topLeft().toPoint();
    off.setX(off.x()/f);
    off.setY(off.y()/f);

    // Scale the painter and draw our widget
    painter->save();
    painter->scale(f, f);
    m_widget->render(painter, off, QRegion(), QWidget::DrawChildren);

    // Put everything back the way it was.
    painter->restore();
    m_widget->resize(origSize.toSize());

}


void NCReportWidgetRenderer::setWidget(QWidget* p) {m_widget = p; }

void NCReportWidgetRenderer::setScaleMode(Qt::AspectRatioMode mode) { m_scaleMode = mode; }

qreal NCReportWidgetRenderer::calculateHeightMM(NCReportOutput *output) const
{
    if (!m_widget)
        return 0.0;

    qreal f = output->resolution() / QApplication::desktop()->logicalDpiX();
    qreal height = m_widget->size().height() * f;
    return output->scale().pixelToMM(height);
}

QSizeF NCReportWidgetRenderer::calculateSizeMM( NCReportOutput* output, const QSizeF& requested ) const
{
    switch ( output->output() ) {
    case NCReportOutput::Printer:
    case NCReportOutput::Pdf:
    case NCReportOutput::Preview:
            break;
    default:
            return QSizeF();
    }

    QSizeF orig;
    if (m_widget)
        orig = m_widget->size();
    else
        orig = m_baseSize;

    QSizeF size = output->scale().mmToSize(requested);
    orig.scale(size.width(), size.height(), m_scaleMode);
    size = output->scale().sizeToMM(orig);
    return size;
}

void NCReportWidgetRenderer::setDPI(unsigned int dpi) { m_resolution = dpi; }
unsigned int NCReportWidgetRenderer::getDPI() { return m_resolution; }

void NCReportWidgetRenderer::setBaseSize(const QSizeF& base) { m_baseSize = base; }
QSizeF NCReportWidgetRenderer::baseSize() const { return m_baseSize; }

