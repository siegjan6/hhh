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
#include "ncrddesignsection.h"
#include "ncrdsectiontitle.h"
#include "ncrddesignarea.h"
#include "ncrddocument.h"
#include "ncrdresizerbar.h"
#include "ncrdruler.h"
#include "ncrdsectionscene.h"
#include "ncrdmainwindow.h"
#include "ncrdscale.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

NCRDDesignSection::NCRDDesignSection( NCRDMainWindow * mwin, NCRDDocument *doc, QWidget * parent )
    : QWidget( parent )
{
    mainwindow = mwin;
    m_doc = doc;
    init();
}

NCRDDesignSection::~ NCRDDesignSection( )
{
}

void NCRDDesignSection::init( )
{
    m_footerSection = 0;
    m_headerSection = 0;
    m_scene =0;
    //setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    m_title = new NCRDSectionTitle( this );
    layout->addWidget(m_title);

    QHBoxLayout *layMS = new QHBoxLayout;
    layMS->setMargin(0);
    layMS->setSpacing(0);

    ruler = new NCRDRuler( Qt::Vertical, this );
    designarea = new NCRDDesignArea( mainwindow, m_doc, this );

    layMS->addWidget( ruler );
    layMS->addWidget( designarea );

    layout->addLayout(layMS);

    // resizer bar
    rbar = new NCRDResizerBar( this );
    connect( rbar, SIGNAL(barDragged(int)), this, SLOT(resizeBarDragged(int) ) );
    layout->addWidget(rbar);

    setLayout(layout);

    connect( designarea, SIGNAL(mouseMoved(QPoint)), ruler, SLOT(paintMarker(QPoint)) );
}

void NCRDDesignSection::setTitle(const QString & s, const QIcon &icon )
{
    m_title->setText( s );
    m_title->setIcon( icon );
}

void NCRDDesignSection::setTitle( const QString & s )
{
    m_title->setText( s );
}

// void NCRDDesignSection::resizeBarDragged( int d )
// {
// 	int h = designarea->height() + d;
// 	if( h < 1)
// 		h = 1;
//
// 	designarea->resize(designarea->width(), h);
//     adjustSize();
//     QWidget *parent = parentWidget();
//     if (parent ) {
// 		parent->adjustSize();
// 		//parent->parentWidget()->adjustSize();
// /*		if(w->isA("ReportSectionDetail"))
// 		w->parentWidget()->adjustSize();*/
//     }
//
// 	//setFixedHeight( h);
//
// }

void NCRDDesignSection::resizeBarDragged( int d )
{
    int h = height() + d;
    const int trim = 0;

    // SNAP To GRID
    //if ( m_scene && m_scene-> mainwindow->isSnapGrid() &&  )
    //	h = (int)( m_scene->snapToY( (qreal)h ) );

    if( h < m_title->height()+rbar->height()+trim )
        h = m_title->height()+rbar->height()+trim;

    setFixedHeight( h );
    adjustSize();
    parentWidget()->adjustSize();

    if ( m_scene ) {
        NCRDScale sc;
        qreal mH = sc.pixelToMM( h - m_title->height() - rbar->height() ) / m_doc->zoomScale();
        m_scene->setMetricHeight( mH );
        //qDebug("Height resized mm=%f",mH );
    }
    //updateSceneRect();
    m_doc->setDocumentModified();
    emit sectionResized( QSize(designarea->width(),h) );
}

NCRDDesignArea * NCRDDesignSection::designArea( ) const
{
    return designarea;
}

void NCRDDesignSection::setWidthMM( qreal mW )
{
    NCRDScale sc;
    int w = sc.mmToPixel(mW);
    setFixedWidth( ruler->width() + qRound(w*m_doc->zoomScale())  );
    adjustSize();

    //updateSceneRect();
    //m_doc->documentWasModified();
    emit sectionResized( QSize(w,designarea->height()) );
}

void NCRDDesignSection::setHeightMM( qreal mH )
{
    NCRDScale sc;
    int h = sc.mmToPixel( mH );
    setFixedHeight( m_title->height()+rbar->height() + qRound(h*m_doc->zoomScale())  );
    adjustSize();
    if ( m_scene ) {
        //qDebug("setHeightMM=%f",mH );
        m_scene->setMetricHeight( mH );
    }
    //updateSceneRect();
    //m_doc->documentWasModified();
    emit sectionResized( QSize(h,designarea->height()) );
}

void NCRDDesignSection::setFooterSection( NCRDDesignSection *s )
{
    m_footerSection = s;
}
NCRDDesignSection * NCRDDesignSection::footerSection( )
{
    return m_footerSection;
}

NCRDDesignSection * NCRDDesignSection::headerSection()
{
    return m_headerSection;
}

void NCRDDesignSection::setHeaderSection( NCRDDesignSection *s )
{
    m_headerSection = s;
}

NCRDSectionTitle * NCRDDesignSection::title()
{
    return m_title;
}

void NCRDDesignSection::setSectionScene(NCRDSectionScene *rs )
{
    m_scene = rs;
    //designArea()->setScene( rs );
    designArea()->setReportSection( rs );
}

NCRDSectionScene * NCRDDesignSection::sectionScene() const
{
    return m_scene;
}

void NCRDDesignSection::updateSceneRect()
{
    //qDebug("ensure visible");
    designarea->ensureVisible( 0.0,	0.0 , 0.0, 0.0, 0, 0 );

    //m_scene->setSceneRect( 0.0, 0.0, designarea->width(), designarea->height() );
}

void NCRDDesignSection::showEvent(QShowEvent *)
{
    updateSceneRect();
}
