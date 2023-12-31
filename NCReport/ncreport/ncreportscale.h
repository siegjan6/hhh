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
#ifndef NCREPORTSCALE_H
#define NCREPORTSCALE_H

#include <QtGlobal>
#include <QPointF>
#include <QSize>
#include "ncreport_global.h"

/*!
This class is responsible for conversion from pixel into metric measurement and vice versa
 */

/*!

section 功能

报表计量类
【这个类是负责从像素转换成公制测量，反之亦然】

*/
class NCREPORTSHARED_EXPORT NCReportScale
{
public:
	NCReportScale();
	NCReportScale( qreal dpi );
	~NCReportScale();

	void setDpi( qreal dpi );
	qreal dpi() const;
	qreal pixelToMM( int pixel );
	qreal pixelToMM( qreal pixel );
	int mmToPixel( qreal mm );
	qreal mmToRealPixel( qreal mm ) const;
	QPointF pointToMM( const QPointF& point ) const;
	QPointF mmToPoint( const QPointF& pointMM ) const;
	QSizeF sizeToMM( const QSizeF& size ) const;
	QSizeF mmToSize( const QSizeF& sizeMM ) const;
	int fontPointSizeToPixel( int pointSize ) const;
	qreal fontPixelSizeToPoint( qreal pixel ) const;

	void setScreenDpi( int dpi ) { m_screenDpi = dpi; }
	inline int screenDpi() const { return m_screenDpi; }
	void setTrimFactor( qreal trim ) { m_trimFactor = trim; }
	inline qreal trimFactor() const { return m_trimFactor; }

	qreal screenToDeviceDpiFactor() const;

private:
	qreal m_dpi;
	int m_screenDpi;
	qreal m_trimFactor;
	qreal m_fontFactor;

	void setScreenResolution();
};

#endif
