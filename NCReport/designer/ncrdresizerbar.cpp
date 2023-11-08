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
#include "ncrdresizerbar.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStylePainter>
#include <QStyleOptionRubberBand>

NCRDResizerBar::NCRDResizerBar( QWidget * parent ) : QWidget( parent )
{
	//setFrameStyle( WinPanel | Raised);
	setMinimumHeight(7);
	setMaximumHeight(7);
	setCursor( QCursor( Qt::SizeVerCursor) );
	resize(7,7);
	//setBackgroundRole(QPalette::Button);
}

NCRDResizerBar::~ NCRDResizerBar( )
{
}

void NCRDResizerBar::mouseMoveEvent(QMouseEvent * e)
{
	e->accept();
	emit barDragged(e->y());
}

// void NCRDResizerBar::paintEvent( QPaintEvent * )
// {
//     QStylePainter p(this);
// 	QStyleOptionRubberBand option;
// 
//  option.initFrom(this);
// 	option.opaque = false;
// 	option.shape = QRubberBand::Rectangle;
// 	p.drawControl(QStyle::CE_RubberBand, option);
// }

void NCRDResizerBar::paintEvent( QPaintEvent * )
{
	QPainter p(this);

	QStyleOption opt(0);
	opt.rect = rect();
	opt.palette = palette();
	//opt.state = QStyle::State_Horizontal;
	opt.state |= QStyle::State_Enabled;
	opt.state |= QStyle::State_MouseOver;

	//parentWidget()->style()->drawControl(QStyle::CE_Splitter, &opt, &p );
	style()->drawControl(QStyle::CE_Splitter, &opt, &p );
}
