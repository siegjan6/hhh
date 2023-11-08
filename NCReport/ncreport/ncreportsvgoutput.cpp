/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportsvgoutput.h"

#include <QSvgGenerator>
#include <QBuffer>
#include <QPainter>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#define SVGOUTPUT_DPI	96

NCReportSvgOutput::NCReportSvgOutput( QObject* parent ) : NCReportOutput( parent ), m_generator(0), m_buffer(0), m_pageNo(0)
{
    setOutput( Svg );
}

NCReportSvgOutput::~ NCReportSvgOutput()
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("NCReportSvgOutput deleted.");
#endif
}

bool NCReportSvgOutput::setup()
{
    scale().setDpi( SVGOUTPUT_DPI );
    return true;
}

void NCReportSvgOutput::begin()
{
    setPainter( new QPainter() );
    newPage();
}

void NCReportSvgOutput::end()
{
    painter()->end();
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("SVG painter end %p",painter() );
#endif
    deletePainter();
    deletePageGenerator();
}

void NCReportSvgOutput::newPage()
{
    m_pageNo++;

    if (painter() && painter()->isActive()) {
        painter()->end();
        deletePageGenerator();
    }
    createPageGenerator();
    painter()->begin( m_generator );
    rotatePainter(painter());
    customizePainter(painter());

    // draw page background

    QPen pen( Qt::black );
    painter()->setPen( pen );
    painter()->setBrush( QBrush(Qt::white) );
    painter()->drawRect( 0, 0, scale().mmToPixel(option().pageWidthMM()), scale().mmToPixel(option().pageHeightMM()) );
}

void NCReportSvgOutput::createPageGenerator()
{
    QSize size( scale().mmToPixel(option().pageWidthMM()), scale().mmToPixel(option().pageHeightMM()) );
    m_generator = new QSvgGenerator();
    m_generator->setResolution(SVGOUTPUT_DPI);
    //scale().setDpi( m_generator->resolution() );

    m_generator->setSize( size );
#if (QT_VERSION >= 0x040400)
    m_generator->setViewBox( QRect(QPoint(0,0),size) );
    m_generator->setTitle( option().reportName() );
    m_generator->setDescription( option().reportName() + " "+QString("page %1").arg(m_pages.size()) );

    QFileInfo fi( fileName() );
    QString pageFile = QString("%1_p%2.svg").arg(fi.path()+"/"+fi.baseName()).arg( m_pageNo );
    m_generator->setFileName( pageFile );
#endif
}


void NCReportSvgOutput::deletePageGenerator()
{
    delete m_generator;
    m_generator = 0;
}


void NCReportSvgOutput::cleanup()
{
    // we don't delete pages, since it's needed
}


