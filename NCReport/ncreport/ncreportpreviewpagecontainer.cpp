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
#include "ncreportpreviewpagecontainer.h"
#include "ncreportpreviewpage.h"
#include "ncreportpreviewoutput.h"


NCReportPreviewPageContainer::NCReportPreviewPageContainer(QWidget * parent)
    : QWidget( parent )
{
    setBackgroundRole( QPalette::NoRole );
    m_showType = Single;
    setMargins( 20, 20, 10, 10 );
    m_currentscalefactor = 1.0;
    //m_numPagesOnScreen = 1;
    m_gap = 10;
    m_output =0;
}

NCReportPreviewPageContainer::~NCReportPreviewPageContainer()
{
}

void NCReportPreviewPageContainer::setShowType( const ShowType st)
{
    if ( st == m_showType )
        return;
    m_showType = st;
    qreal last_scale = m_currentscalefactor;
    createPages( false );
    if (last_scale == 1.0 )
        updateSizeAndPos();
    else
        scale(last_scale);
}

void NCReportPreviewPageContainer::updateSizeAndPos()
{
    if (m_pages.count()==0)
        return;
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("updateSizeAndPos()");
#endif
    switch ( m_showType ) {
    case Single: {
            resize( m_pages[0]->width() + m_leftMargin + m_rightMargin,
                    m_pages[0]->height() + m_topMargin + m_bottomMargin );
            m_pages[0]->move( m_leftMargin, m_topMargin );
            break;
        }
//	case Double: {
//			resize( m_pages[0]->width()*2 + m_leftMargin + m_rightMargin +m_gap,
//					m_pages[0]->height() + m_topMargin + m_bottomMargin );
//			m_pages[0]->move( m_leftMargin, m_topMargin );
//			if (m_pages.count() > 1)
//				m_pages[1]->move( m_leftMargin + m_pages[0]->width() + m_gap, m_topMargin );
//			break;
//        }
    case Double:
    case Continous: {
            QSize size(0,0);
            QPoint pos(m_leftMargin,m_topMargin);

            for ( int i=0; i<m_pages.size(); ++i ) {
                NCReportPreviewPage *page = m_pages.at(i);

                page->move( pos );
                if (m_showType == Continous) {
                    if ( size.width() < page->width() )
                        size.setWidth(page->width());

                    size.rheight()+= page->height() + m_gap;
                    pos.ry() += page->height() + m_gap;
                } else {
                    if ( size.height() < page->height() )
                        size.setHeight(page->height());

                    size.rwidth()+= page->width() + m_gap;
                    pos.rx() += page->width() + m_gap;
                }
            }
            size.rwidth() += m_leftMargin + m_rightMargin;
            size.rheight() += m_topMargin + m_bottomMargin;

            resize( size );
            break;
        }
    }

}

void NCReportPreviewPageContainer::updatePages()
{
}

void NCReportPreviewPageContainer::setOutput(NCReportPreviewOutput *output)
{
    if ( output == m_output )
        return;

    m_output = output;

    createPages();
    //setPageSize(output->option().pageSizeMM());
}

void NCReportPreviewPageContainer::createPages(bool doUpdateSize)
{
    qDeleteAll(m_pages.begin(), m_pages.end());
    m_pages.clear();
    m_currentscalefactor = 1.0;

    if( !m_output )
        return;

    int pagesOnScreen=0;
    switch ( m_showType ) {
        case Single:
            pagesOnScreen=1;
            break;
        case Double:
            pagesOnScreen=2;
            break;
        case Continous:
            pagesOnScreen=m_output->pages().count();
            break;
    }

    if ( pagesOnScreen > m_output->pages().count() )
        pagesOnScreen = m_output->pages().count();	// safety


    for ( int i=0; i<pagesOnScreen; ++i ) {
        NCReportPreviewPage *page = 0;

        if ( i > m_pages.count()-1 ) {
            page = new NCReportPreviewPage(this);
            m_pages.append( page );
            page->setPageSizeMM(m_output->pageSizeMM(i+1));
            page->setContent( m_output->pages().at( i ) );

            if (page->isHidden())
                page->show();
        } else {
            m_pages.at(i)->show();
        }
    }

    if ( doUpdateSize )
        updateSizeAndPos();

}

qreal NCReportPreviewPageContainer::maxPageWidth() const
{
    return 0.0;
}


void NCReportPreviewPageContainer::loadPage(int pageno)
{
    // pageno: 1 to n
    int pageCount=m_output->pages().count();

    switch( m_showType ) {
    case Single:
    case Double:
        {
            // update content of on-screen pages
            for ( int i=0; i<m_pages.count(); i++ ) {
                NCReportPreviewPage *page = m_pages.at(i);

                if ( pageno > pageCount || pageno<1 ) {
                    page->setContent( QByteArray() );
                    if ( !page->isHidden() )
                        page->hide();
                } else {
                    page->setPageSizeMM(m_output->pageSizeMM(pageno));
                    page->setContent( m_output->pages().at( pageno-1 ) );
                    //page->setPageNo(pageno);
                    if (page->isHidden())
                        page->show();
                }
                page->update();
                pageno++;
            }
            break;
        }
    case Continous:
        // all pages are already loaded, nothing to do.
        break;
    }

}

void NCReportPreviewPageContainer::scale(qreal factor)
{
    if (m_pages.count()==0)
        return;
    if (factor == m_currentscalefactor )
        return;
    for ( int i=0; i<m_pages.count(); i++ )
        m_pages.at(i)->setScaleRate( factor );

    m_currentscalefactor = m_pages.at(0)->scaleRate();
    updateSizeAndPos();
}

qreal NCReportPreviewPageContainer::currentScaleFactor() const
{
    return m_currentscalefactor;
}

void NCReportPreviewPageContainer::setMargins(int left, int right, int top, int bottom)
{
    m_leftMargin =left;
    m_rightMargin =right;
    m_topMargin =top;
    m_bottomMargin =bottom;
}

void NCReportPreviewPageContainer::setAllPageSize(const QSizeF & mmsize)
{
    for ( int i=0; i<m_pages.count(); i++ )
        m_pages.at(i)->setPageSizeMM(mmsize);
    updateSizeAndPos();
}

NCReportPreviewPageContainer::ShowType NCReportPreviewPageContainer::showType() const
{
    return m_showType;
}

NCReportPreviewPage* NCReportPreviewPageContainer::page(int i) const
{
    if (i<0 || i > m_pages.count()-1)
        return 0;
    return m_pages.at(i);
}

int NCReportPreviewPageContainer::pagePosition( int pageno ) const
{
    NCReportPreviewPage *m_page = page(pageno);
    if (!m_page)
        return 0;

    int y = m_page->y();
    int height = m_page->height();

    return y-height;
}

int NCReportPreviewPageContainer::numPagesOnScreen() const
{
    return m_pages.count();
}
