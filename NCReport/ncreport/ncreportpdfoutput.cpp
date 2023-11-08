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

#include "ncreportpdfoutput.h"
#include <QPainter>

/*!
  PDF output class.
  Batch mode is supported only when the reports have the same page settings otherwise the result will become incorrect.
  */

NCReportPdfOutput::NCReportPdfOutput(QPrinter::PrinterMode quality, int dpi, QPrinter::OutputFormat outputFormat, QObject *parent)
    : NCReportPrinterOutput( quality, dpi, parent ), m_outputFormat(outputFormat)
{
    setOutput( Pdf );
    setShowPrintDialog(false);
    printer()->setColorMode( QPrinter::Color );
}

void NCReportPdfOutput::setFileName( const QString& fname )
{
    NCReportOutput::setFileName( fname );
    printer()->setOutputFileName( fname );
#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
    if (m_outputFormat == QPrinter::PdfFormat)
        printer()->setOutputFormat( QPrinter::NativeFormat );
    else
        printer()->setOutputFormat( m_outputFormat );
#else
    printer()->setOutputFormat( m_outputFormat );
#endif
}

void NCReportPdfOutput::begin()
{
    NCReportPrinterOutput::begin();
}

void NCReportPdfOutput::end()
{
    NCReportPrinterOutput::end();
}

void NCReportPdfOutput::cleanup()
{
}
