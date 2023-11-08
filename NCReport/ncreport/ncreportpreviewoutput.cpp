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
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewpage.h"

#include <QPainter>
#include <QPicture>

NCReportPreviewOutput::NCReportPreviewOutput( QObject* parent ) : NCReportOutput( parent ),
    m_last_reportno(0), m_currentPage(0)
{
    setOutput( Preview );
}

NCReportPreviewOutput::~ NCReportPreviewOutput()
{
    deletePages();
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("NCReportPreviewOutput deleted.");
#endif
}

bool NCReportPreviewOutput::setup()
{
    return true;
}

void NCReportPreviewOutput::begin()
{
    QPainter *pa  = new QPainter();
    m_currentPage = new QPicture();

    QByteArray content;
    m_pages.append(content);
    m_pageSizesMM.append( option().pageSizeMM() );
    pa->begin( m_currentPage );

    if ( m_last_reportno < reportNum() ) {
        // save the number of the rprt. This is neccessary for batch reports for we have to know where
        // the n. report starts
        m_startPagesOfReports.append(m_pages.size());
        m_last_reportno = reportNum();
    }
    setPainter( pa );
}

void NCReportPreviewOutput::end()
{
    endPage();
    deletePainter();
}

void NCReportPreviewOutput::newPage()
{
    endPage();
    beginPage();
}

void NCReportPreviewOutput::deletePages()
{
    //qDeleteAll(m_pages.begin(), m_pages.end());
    m_pages.clear();
    m_pageSizesMM.clear();
    m_startPagesOfReports.clear();
}

void NCReportPreviewOutput::endPage()
{
    if (painter())
        painter()->end();

    if (m_currentPage) {
        NCReportPreviewPage::savePicture(*m_currentPage, m_pages.last());
        delete m_currentPage;
        m_currentPage = 0;
    }
}

void NCReportPreviewOutput::beginPage()
{
    m_currentPage = new QPicture();

    QByteArray content;
    m_pages.append(content);
    m_pageSizesMM.append( option().pageSizeMM() );
    painter()->begin( m_currentPage );
    rotatePainter(painter());
}

void NCReportPreviewOutput::cleanup()
{
    // we won't delete pictures since it is needed by Preview
}

QByteArray NCReportPreviewOutput::page(int pageNo ) const
{
    if ( pageNo>m_pages.count()-1 || pageNo<0 )
        return 0;
    return m_pages.at(pageNo);
}

const QList<QByteArray> & NCReportPreviewOutput::pages()
{
    return m_pages;
}


const QList<int>& NCReportPreviewOutput::startPagesOfReports()
{
    return m_startPagesOfReports;
}

QSizeF NCReportPreviewOutput::pageSizeMM(int pageNo ) const
{
    if ( pageNo > m_pageSizesMM.count() || pageNo<1 )
        return QSizeF();
    else
        return m_pageSizesMM.at(pageNo-1);
}


