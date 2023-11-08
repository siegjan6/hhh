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
#include "ncreportscale.h"
#include <QApplication>
#include <QDesktopWidget>

//#define	SCREEN_DPI	72
//#define	SCREEN_TRIM	1.33
//#define	SCREEN_TRIM	1.38  //2010.03.04
// ---------- 2011.01.18
//#define	SCREEN_DPI	120
//#define	SCREEN_TRIM	1.0

#define	SCREEN_DPI	96
#define	SCREEN_TRIM	1.0  //2010.12.13

#define FONT_METRIC_DIDOT		2.6600
#define FONT_METRIC_PICA		2.8457
#define FONT_METRIC_POSTSCRIPT	2.8346

NCReportScale::NCReportScale()
    : m_dpi(-1), m_screenDpi(SCREEN_DPI), m_trimFactor(SCREEN_TRIM), m_fontFactor(FONT_METRIC_POSTSCRIPT)
{
    int dpiX = qApp->desktop()->logicalDpiX();

    m_dpi = dpiX*trimFactor();
    m_screenDpi = m_dpi;

//    m_dpi = screenDpi()*trimFactor();
//    m_screenDpi = m_dpi;
    // WINDOWS 125% = 120 Dpi
    // WINDOWS 150% = 120 Dpi
}

NCReportScale::NCReportScale(qreal dpi)
{
    m_dpi = dpi;
}

NCReportScale::~NCReportScale() {}

qreal NCReportScale::pixelToMM( int pixel)
{
    return (pixel/m_dpi)*25.4;
}

qreal NCReportScale::pixelToMM(qreal pixel)
{
    return (pixel/m_dpi)*25.4;
}

int NCReportScale::mmToPixel( qreal mm )
{
    return qRound((mm*m_dpi)/25.4);
}

qreal NCReportScale::mmToRealPixel(qreal mm) const
{
    return (mm*m_dpi)/25.4;
}

void NCReportScale::setDpi(qreal dpi)
{
    m_dpi = dpi;
}

qreal NCReportScale::dpi() const
{
    return m_dpi;
}

void NCReportScale::setScreenResolution()
{
}

QPointF NCReportScale::pointToMM(const QPointF & p) const
{
    qreal x = ((NCReportScale*)this)->pixelToMM(p.x());
    qreal y = ((NCReportScale*)this)->pixelToMM(p.y());
    return QPointF( x,y );
}

QPointF NCReportScale::mmToPoint(const QPointF & pointMM) const
{
    return QPointF( ((NCReportScale*)this)->mmToPixel(pointMM.x()), ((NCReportScale*)this)->mmToPixel(pointMM.y()) );
}

QSizeF NCReportScale::sizeToMM(const QSizeF & size) const
{
    qreal w = ((NCReportScale*)this)->pixelToMM(size.width());
    qreal h = ((NCReportScale*)this)->pixelToMM(size.height());
    return QSizeF( w,h );
}

QSizeF NCReportScale::mmToSize(const QSizeF & sizeMM) const
{
    qreal w = ((NCReportScale*)this)->mmToPixel(sizeMM.width());
    qreal h = ((NCReportScale*)this)->mmToPixel(sizeMM.height());
    return QSizeF( w,h );
}


int NCReportScale::fontPointSizeToPixel( int pointSize ) const
{
    return ((NCReportScale*)this)->mmToPixel(pointSize/m_fontFactor);
}

qreal NCReportScale::fontPixelSizeToPoint( qreal pixel ) const
{
    return ((NCReportScale*)this)->pixelToMM(pixel)*m_fontFactor;
}

qreal NCReportScale::screenToDeviceDpiFactor() const
{
    return dpi() / screenDpi();
}


