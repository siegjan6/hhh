/****************************************************************************
*
*  Copyright (C) 2002-2009 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncreportqtpreviewoutput.h"
#include "info.h"

#if QT_VERSION >= 0x040400
#include <QPrintPreviewDialog>
#endif
#include <QPainter>
#include <QSettings>

NCReportQtPreviewOutput::NCReportQtPreviewOutput(QPrinter::PrinterMode quality, int dpi, QObject* parent) : NCReportPrinterOutput( quality, dpi, parent )
    ,m_report(0), m_copies(1)
{
#if QT_VERSION >= 0x040400
    setOutput( QtPreview );
    m_preview = new QPrintPreviewDialog(printer());
    QSettings settings;
    m_preview->restoreGeometry( settings.value("geometry/qtpreviewwindow").toByteArray() );

    connect(m_preview, SIGNAL(paintRequested(QPrinter *)), this, SLOT(printPreview(QPrinter *)));
#endif
}

NCReportQtPreviewOutput::~ NCReportQtPreviewOutput()
{
#if QT_VERSION >= 0x040400
    QSettings settings;
    settings.setValue("geometry/qtpreviewwindow", m_preview->saveGeometry() );
    delete m_preview;
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("NCReportQtPreviewOutput deleted");
#endif
#endif
}

bool NCReportQtPreviewOutput::setup()
{
#if QT_VERSION < 0x040400
    return true;
#endif
    initPrinter();
#if QT_VERSION >= 0x040700
    printer()->setCopyCount(m_copies);
#else
    printer()->setNumCopies(m_copies);
#endif
    return true;
}


#if QT_VERSION >= 0x040400
QPrintPreviewDialog* NCReportQtPreviewOutput::dialog()
{
    return m_preview;
}
#endif

void NCReportQtPreviewOutput::printPreview(QPrinter *p)
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("printPreview(%p) ?= %p",p, printer() );
#else
    Q_UNUSED(p);
#endif
    // HBE: Bugfix for printing pageranges only (PGZ BUG 30)
    int from = printer()->fromPage();
    int to = printer()->toPage();
    if (from <= 0 && to <= 0)
    {
        from = 1;
        to = 9999;
    }
#if QT_VERSION >= 0x040700
    m_copies = printer()->copyCount();
#else
    m_copies = printer()->numCopies();
#endif
    m_report->output()->setPageRange(from, to);
    // Bugfix end
    m_report->runReport();
}

void NCReportQtPreviewOutput::setReport( NCReport* report )
{
    m_report = report;
}

void NCReportQtPreviewOutput::begin()
{
    NCReportPrinterOutput::begin();
}

void NCReportQtPreviewOutput::end()
{
    NCReportPrinterOutput::end();
}

