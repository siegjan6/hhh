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
#include "ncrdruler.h"
#include "ncrdscale.h"

#include <QPainter>

#define RULERWEIGHT	28

NCRDRuler::NCRDRuler( Qt::Orientation ori, QWidget * parent ) : QWidget( parent )
{
	zoomscale = 1.0f;
	orient = ori;
	scStart = 0;
	offset =0;
	const int rulerH = RULERWEIGHT;
	//unclippedPainter = new QPainter();
	//unclippedPainter->setClipping(false);
	
	NCRDScale sc;
	measureunit = sc.mmToRealPixel( 1.0 );
	//setPalette( QPalette( Qt::white ) );
	setBackgroundRole( QPalette::Window );
	//setFrameStyle( StyledPanel | Raised );
	//setFrameStyle( QFrame::Panel | QFrame::Raised);

	if ( orient == Qt::Horizontal ) {
		setMinimumHeight(rulerH);
		setMaximumHeight(rulerH);
	} else {
		setMinimumWidth(rulerH);
		setMaximumWidth(rulerH);
	}		
	resize(rulerH,rulerH);
}

NCRDRuler::~ NCRDRuler( )
{
	//delete unclippedPainter;
}

void NCRDRuler::paintEvent( QPaintEvent * )
{
	qreal incr = measureunit;
	int stickHeight =0;
	int counter=0;
	QString label;

	QPainter p( this );
	p.setPen(Qt::black);
	QFont f;
	f.setPixelSize(10);
	QFontMetrics fm( f );
	p.setFont( f );
	
	if ( offset!= 0 )
		p.translate( offset, 0 );
	
	const int m_width = width();
	const int m_height = height();
	
	int to = ( orient == Qt::Horizontal ? m_width : m_height );
	bool stick = false;
	//for ( qreal r= offset*measureunit ; r< to; r+=incr ) {
	for ( qreal r= 0.0 ; r< to; r+=incr ) {

		if ( counter%10 == 0 ) {
			stickHeight = 10;
			stick = true;
		} else if ( counter%5 == 0 ) {
			stickHeight = 8;
			stick = true;
		} else if ( counter%2 == 0 ) {
			stickHeight = 5;
			stick = true;
		} else {
			stickHeight = 5;
			stick = true;
		}

		if ( orient == Qt::Horizontal && stick )
				p.drawLine( scStart + qRound(r), m_height-stickHeight, scStart + qRound(r), m_height );
		else if ( orient == Qt::Vertical && stick )
				p.drawLine( m_width-stickHeight, scStart + qRound(r), m_width, scStart + qRound(r) );

		if ( counter%10 == 0 ) {
			// NUMBER TEXT
			label.setNum( counter/10 );
			if ( orient == Qt::Horizontal )
				p.drawText( qRound(scStart + r-fm.width(label)/2), 14, label );
			else
				p.drawText( 5, qRound(scStart + 10+r-fm.height()/2), label );
		}

		counter++;
	}
	
	if ( orient == Qt::Horizontal ) {
		p.drawLine( 0, m_height-1, m_width, m_height-1 );
		//p.drawLine( m_width-1, 0, m_width-1, m_height );
	} else {
		p.drawLine( m_width-1, 0, m_width-1, m_height );
		//p.drawLine( 0, m_height-1, m_width, m_height-1 );
	}
	
}

void NCRDRuler::setRulerMargin(int margin)
{
	scStart = margin;
}

void NCRDRuler::setScaleOffset(int point, bool doupdate )
{
	offset = point;
	if (doupdate)
		update();
}

void NCRDRuler::setZoomScale( qreal scale )
{
	NCRDScale sc;
	measureunit = sc.mmToRealPixel( 1.0 * scale );
	update();	
}

void NCRDRuler::paintMarker(const QPoint &)
{

//	unclippedPainter->begin(this);
//	if ( orient == Qt::Horizontal )
//		unclippedPainter->drawLine( pos.x(), 0, pos.x(), height()-1 );
//	else
//		unclippedPainter->drawLine( 0, pos.y(), width()-1, pos.y() );
//
//	unclippedPainter->end();
}

