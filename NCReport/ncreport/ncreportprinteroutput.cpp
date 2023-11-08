/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport Report Generator System
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  norbert@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncreportprinteroutput.h"

#include <QPrinter>
#ifndef NCREPORT_NO_PRINT_DIALOG
#include <QPrintDialog>
#endif
#include <QApplication>
#include <QPainter>

//#define RESOLUTION_TEST
#ifdef RESOLUTION_TEST
#include <QMessageBox>
#include <QTextStream>
#endif

#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
#define MAC_SCALE_FACTOR	1.3527
#else
#define MAC_SCALE_FACTOR	1.0000
#endif

NCReportPrinterOutput::NCReportPrinterOutput( QObject* parent)
    : NCReportOutput( parent ), m_printer(0), m_initDone(false), m_dialogDone(false)
{
    setOutput( Printer );
    setPrinter( createPrinter( QPrinter::HighResolution, -1 ) );
}

NCReportPrinterOutput::NCReportPrinterOutput( QPrinter::PrinterMode quality, int dpi, QObject *parent)
    : NCReportOutput( parent ), m_printer(0), m_initDone(false), m_dialogDone(false)
{
    setOutput( Printer );
    setPrinter( createPrinter( quality, dpi ) );
}

NCReportPrinterOutput::NCReportPrinterOutput(const QPrinterInfo &printerInfo, QPrinter::PrinterMode quality, int dpi, QObject *parent)
    : NCReportOutput( parent ), m_printer(0), m_initDone(false), m_dialogDone(false)
{
    setOutput( Printer );
    setPrinter( createPrinter( printerInfo, quality, dpi ) );
}

NCReportPrinterOutput::~ NCReportPrinterOutput()
{
    deletePainter();
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Delete printer");
#endif
    if (m_printer)
        delete m_printer;
}

QPrinter *NCReportPrinterOutput::createPrinter(QPrinter::PrinterMode quality, int dpi)
{
    QPrinter *printer = new QPrinter( quality );
    if ( dpi > 0 ) {
        scale().setDpi( dpi );
        printer->setResolution(dpi);
    }
    return printer;
}

QPrinter *NCReportPrinterOutput::createPrinter(const QPrinterInfo &printerInfo, QPrinter::PrinterMode quality, int dpi)
{
    QPrinter *printer = new QPrinter( printerInfo, quality );
    if ( dpi > 0 ) {
        scale().setDpi( dpi );
        printer->setResolution(dpi);
    }
    return printer;
}

void NCReportPrinterOutput::initPrinter()
{
    m_printer->setOrientation( option().pageOrientation() );
#if QT_VERSION >= 0x040400
    m_printer->setPaperSize( option().paperSize() );
    if ( option().paperSize() == QPrinter::Custom )
        m_printer->setPaperSize( option().pageSizeMM(), QPrinter::Millimeter );
#else
    pr->setPageSize( option().pageSize() );
#endif
    m_printer->setFullPage( true );
#if QT_VERSION >= 0x050300
    m_printer->setPageMargins( QMarginsF() );
#endif
    m_printer->setNumCopies( copies() );

    scale().setDpi( m_printer->resolution()*MAC_SCALE_FACTOR );
    rotatePrinter(m_printer);
    customizePrinter( m_printer );
}

void NCReportPrinterOutput::setCopies( int num )
{
    NCReportOutput::setCopies(num);
    m_printer->setNumCopies( num );
}

bool NCReportPrinterOutput::setup()
{
    initPrinter();

#ifndef NCREPORT_NO_PRINT_DIALOG
    if ( showPrintDialog() ) {

        if ( m_dialogDone )
            return true;

#ifndef NCREPORT_NO_CURSOR
        qApp->restoreOverrideCursor();
#endif
        QPrintDialog pdia( m_printer, parentWidget() );

#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
        qApp->processEvents(QEventLoop::AllEvents);	// avoid macOS bug
#endif

        if (pdia.exec() == QDialog::Accepted) {
            ////////////////////////////
            // RESOLUTION TEST
            ////////////////////////////
#ifdef RESOLUTION_TEST
            QString m_supportedResolutions;
            QTextStream ts(&m_supportedResolutions);

            ts << "Printer name: " << pr->printerName() << "\n";
            ts << "Resolution: " << pr->resolution() << "\n";
            ts << "Supported resolutions: ";

            QList<int> dpilist = pr->supportedResolutions();
            for (int i = 0; i < dpilist.size(); ++i)
                ts << dpilist.at(i) << ',';

            QMessageBox::information(0, "Test", m_supportedResolutions );
#endif

            NCReportOutput::setCopies( m_printer->numCopies() );
            if ( pdia.printRange() == QAbstractPrintDialog::AllPages )
                setPageRange( 1, 9999 );
            else if ( pdia.printRange() == QAbstractPrintDialog::PageRange )
                setPageRange( pdia.fromPage(), pdia.toPage() );

            scale().setDpi( m_printer->resolution()*MAC_SCALE_FACTOR );
            option().setPaperSize(m_printer->paperSize(), m_printer->orientation());

            m_dialogDone = true;
        } else
            return false;
    }
#endif

    return true;
}

void NCReportPrinterOutput::begin()
{
    if ( !batchMode() || (batchMode() && flagFirst()) ) {
        QPainter *pa  = new QPainter(printer()); // calls ->begin()
        pa->setRenderHint(QPainter::SmoothPixmapTransform,true);
        //pa->setRenderHint(QPainter::HighQualityAntialiasing,true);
        pa->setRenderHint(QPainter::Antialiasing,true);
        rotatePainter(pa);
        customizePainter(pa);
        if (MAC_SCALE_FACTOR!=1.0)
            pa->scale(1/MAC_SCALE_FACTOR,1/MAC_SCALE_FACTOR);
        setPainter( pa );
    }
    if ( batchMode() && !flagFirst() && flagPrintStarted() )
        newPage();
}

void NCReportPrinterOutput::end()
{
    if ( !batchMode() || (batchMode() && flagLast()) ) {
        painter()->end();
        deletePainter();
    }
    if ( batchMode() && flagLast() )
        setDialogDone(true);
}

void NCReportPrinterOutput::newPage()
{
    m_printer->newPage();
}

void NCReportPrinterOutput::cleanup()
{
    //deletePrinter();
}

QPrinter * NCReportPrinterOutput::printer() const
{
    return m_printer;
}

void NCReportPrinterOutput::customizePrinter(QPrinter *printer)
{
    Q_UNUSED(printer)
}

void NCReportPrinterOutput::rotatePrinter(QPrinter *printer)
{
    if (hasValidRotationAngle()) {
        switch (rotationAngle()) {
        case 90:
        case -90:
        case 270:
            printer->setOrientation( printer->orientation()==QPrinter::Portrait ? QPrinter::Landscape : QPrinter::Portrait);
            break;
        case 180:
            break;
        }
    }
}

void NCReportPrinterOutput::setPrinter(QPrinter *printer)
{
    m_printer = printer;
}

void NCReportPrinterOutput::setDialogDone(bool set)
{
    m_dialogDone = set;
}

bool NCReportPrinterOutput::dialogDone() const
{
    return m_dialogDone;
}

int	NCReportPrinterOutput::resolution() const
{
    return m_printer->resolution();
}

void NCReportPrinterOutput::setResolution( int dpi )
{
    scale().setDpi( dpi );
    m_printer->setResolution(dpi);
}

QPaintDevice* NCReportPrinterOutput::device() const
{
    return m_printer;
}

