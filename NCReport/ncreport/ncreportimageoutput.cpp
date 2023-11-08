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
#include "ncreportimageoutput.h"

#include <QPainter>
//#include <QFile>
#include <QFileInfo>

NCReportImageOutput::NCReportImageOutput(QObject *parent) : NCReportOutput(parent), m_imageFormat("JPG"), m_format(QImage::Format_RGB32), m_maxHeight(20000)
{
	setOutput( Image );
}

NCReportImageOutput::~ NCReportImageOutput()
{
	//qDeleteAll(m_pages.begin(), m_pages.end());
	//m_pages.clear();
#ifdef NCREPORT_DEBUG_OUTPUT
	qDebug("NCReportImageOutput deleted.");
#endif
}

void NCReportImageOutput::setImageFormat(const char *format)
{
	m_imageFormat = format;
}

bool NCReportImageOutput::setup()
{
	return true;
}

void NCReportImageOutput::begin()
{
    QPainter *painter = new QPainter();
    painter->setRenderHint(QPainter::SmoothPixmapTransform,true);
    setPainter( painter );
	newPage();
}

void NCReportImageOutput::end()
{
	painter()->end();
#ifdef NCREPORT_DEBUG_OUTPUT
	qDebug("Image painter end %p",painter() );
#endif
	deletePainter();

	QFileInfo fi( fileName() );

	if (isSuccessive()) {
		// COPY RESULT TO A NEW IMAGE
		if (m_pages.count()>0) {
			imageSize.setHeight( scale().mmToPixel(finalPosMM().y() + option().bottomMarginMM() ) );
			m_result = m_pages.at(0).copy( QRect(QPoint(0,0),imageSize) );
			m_result.save(fileName());
		}
	} else {
		for (int i = 0; i < m_pages.size(); ++i) {
			QString pageFile = QString("%1_p%2.%3").arg(fi.path()+"/"+fi.baseName()).arg(i+1).arg(fi.suffix());
			if ( !m_pages.at(i).save(pageFile, fi.suffix().toLatin1() ) ) // writes image into ba in XXX format
				 return;
		}
	}
}

void NCReportImageOutput::newPage()
{
	if (painter() && painter()->isActive()) {
		painter()->end();
	}
	imageSize.setWidth(scale().mmToPixel(option().pageWidthMM()));

	if (isSuccessive())
		imageSize.setHeight( m_maxHeight );
	else
		imageSize.setHeight( scale().mmToPixel(option().pageHeightMM()) );

	QImage image(imageSize,m_format);
	m_pages.append(image);

	painter()->begin( &m_pages.last() );
    rotatePainter(painter());
    customizePainter(painter());

    //	if ( resolution() != 96 )
//		painter()->scale(resolution()/96,resolution()/96);

	// draw page background
	//QPen pen( Qt::black );
	painter()->setPen( Qt::NoPen );
	painter()->setBrush( QBrush(Qt::white) );
	painter()->drawRect( QRect(QPoint(0,0), imageSize) );
}

//void NCReportImageOutput::setPixelWidth(int width)
//{
//	option().setPageWidth( scale().pixelToMM(width) );
//}

/*!
  Sets maximum height of image which affects only when endless media mode is active.
  */
void NCReportImageOutput::setMaximumHeight(int height)
{
	m_maxHeight = height;
}

int NCReportImageOutput::maximumHeight() const
{
	return m_maxHeight;
}

void NCReportImageOutput::cleanup()
{
	// we don't delete pages, since it's needed
}

const QImage& NCReportImageOutput::page(int i ) const
{
	return m_pages.at(i);
}

void NCReportImageOutput::setFormat(QImage::Format format)
{
	m_format = format;
}

QImage::Format NCReportImageOutput::format() const
{
	return m_format;
}

const QImage &NCReportImageOutput::successiveResult() const
{
	return m_result;
}




