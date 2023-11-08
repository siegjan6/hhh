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
#include "ncrdsectiontitle.h"

#include <QStyle>
#include <QPainter>
#include <QStyleOptionHeader>
#include <QStylePainter>

#if defined(Q_WS_X11) || defined(Q_OS_LINUX)
#define SECTION_HEADER_HEIGHT   25
#else
#define SECTION_HEADER_HEIGHT   21
#endif

NCRDSectionTitle::NCRDSectionTitle( QWidget * parent ) : QWidget( parent ),
    topMargin(2), m_icon( QIcon(":/designer/images/bullet_arrow_down.png") )
{
    setFixedHeight( SECTION_HEADER_HEIGHT );
}

NCRDSectionTitle::~ NCRDSectionTitle( )
{
}

void NCRDSectionTitle::paintEvent( QPaintEvent * )
{
    QStylePainter p(this);
    QStyleOptionHeader option;

    option.initFrom(this);
    option.icon = m_icon;
    option.iconAlignment = Qt::AlignLeft;
    option.orientation = Qt::Horizontal;
    option.position = QStyleOptionHeader::OnlyOneSection;
    option.section =0;
    option.selectedPosition = QStyleOptionHeader::NotAdjacent;
    option.sortIndicator = QStyleOptionHeader::None;
    option.text = m_text;
    option.textAlignment = Qt::AlignLeft;
    option.state = /*QStyle::State_HasFocus |*/ QStyle::State_Enabled | QStyle::State_Active;

    p.drawControl(QStyle::CE_Header, option);
    p.end();
    //---------------------------------------------
//    QPainter pa(this);
//    QPixmap pm(":/designer/images/bullet_arrow_down.png");
//    int tpos = 4;
//    if ( !pm.isNull() ) {
//        pa.drawPixmap( 4, topMargin, pm );
//        //pa.drawPixmap( 4, 0, pm );
//        tpos += pm.width()+4;
//    }
    //pa.drawText( tpos, topMargin, width()-2, height()-2, Qt::AlignLeft | Qt::AlignTop, m_text );
}

void NCRDSectionTitle::setText(const QString & t)
{
    m_text = t;
    update();
}

void NCRDSectionTitle::setIcon(const QIcon &icon)
{
    m_icon = icon;
    update();
}
