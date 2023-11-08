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
#include "ncrdshapepropertymanager.h"
#include "ncrdmainwindow.h"
#include "ncrdsectionscene.h"
#include "ncreportshapeitem.h"
#include "ncrdscale.h"
#include "ncrdfillpatterncombo.h"
#include "ncrdlinestylecombo.h"

#include <QAction>
#include <QApplication>
#include <QActionGroup>
#include <QFontComboBox>
#include <QComboBox>
#include <QFontDatabase>
#include <QFontMetricsF>
#include <QColorDialog>
#include <QMenu>
#include <QToolBar>
#include <QDoubleSpinBox>

NCRDShapePropertyManager::NCRDShapePropertyManager( NCRDMainWindow * parent )
	: NCRDPropertyManager( parent )
{
	comboLineStyle=0;
	//comboLineWidth=0;
	spinLineWidth=0;
	comboBrush=0;
	
    QPixmap pix(14, 14);
    pix.fill(Qt::black);
    actLineColor = new QAction(pix, tr("Line &color..."), this);
    connect(actLineColor, SIGNAL(triggered()), this, SLOT(lineColor()));
	
    actFillColor = new QAction(pix, tr("Fill &color..."), this);
    connect(actFillColor, SIGNAL(triggered()), this, SLOT(fillColor()));
	
}

NCRDShapePropertyManager::~ NCRDShapePropertyManager()
{
}

void NCRDShapePropertyManager::lineColor()
{
    QColor col = QColorDialog::getColor(m_lineColor, m_mainwin);
    if (!col.isValid())
        return;
	
    lineColorChanged(col);
    applyChanges(LineColor);
}

void NCRDShapePropertyManager::fillColor()
{
    QColor col = QColorDialog::getColor(m_fillColor, m_mainwin);
    if (!col.isValid())
        return;
	
    fillColorChanged(col);
    applyChanges(FillColor);
}

void NCRDShapePropertyManager::lineWidth(double w)
{
	NCRDScale sc;
	m_pen.setWidthF( sc.mmToPixel(w) );
    applyChanges(LineWidth);
}

void NCRDShapePropertyManager::lineStyle(int idx)
{
	Qt::PenStyle ps=Qt::SolidLine;
	switch( idx ) {
		case 0: ps=Qt::NoPen; break;
		case 1: ps=Qt::SolidLine; break;
		case 2: ps=Qt::DashLine; break;
		case 3: ps=Qt::DotLine; break;
		case 4: ps=Qt::DashDotLine; break;
		case 5: ps=Qt::DashDotDotLine; break;
		case 6: ps=Qt::CustomDashLine; break;
	}
	m_pen.setStyle( ps );
    applyChanges(LineStyle);
}

void NCRDShapePropertyManager::fillPattern(int idx)
{
	Qt::BrushStyle bs=Qt::NoBrush;
	switch( idx ) {
		case 0: bs=Qt::NoBrush; break;
		case 1: bs=Qt::SolidPattern; break;
		case 2: bs=Qt::Dense1Pattern; break;
		case 3: bs=Qt::Dense2Pattern; break;
		case 4: bs=Qt::Dense3Pattern; break;
		case 5: bs=Qt::Dense4Pattern; break;
		case 6: bs=Qt::Dense5Pattern; break;
		case 7: bs=Qt::Dense6Pattern; break;
		case 8: bs=Qt::Dense7Pattern; break;
		case 9: bs=Qt::HorPattern; break;
		case 10: bs=Qt::VerPattern; break;
		case 11: bs=Qt::CrossPattern; break;
		case 12: bs=Qt::BDiagPattern; break;
		case 13: bs=Qt::FDiagPattern; break;
		case 14: bs=Qt::DiagCrossPattern; break;
	}
	m_brush.setStyle( bs );
    applyChanges(FillPattern);
}



void NCRDShapePropertyManager::lineColorChanged(const QColor &c)
{
    QPixmap pix(14, 14);
    pix.fill(c);
    actLineColor->setIcon(pix);
	m_lineColor = c;
	m_pen.setColor(c);
}

void NCRDShapePropertyManager::lineWidthChanged(double w)
{
	//m_pen.setWidthF( w );
	spinLineWidth->setValue( w );
}

void NCRDShapePropertyManager::fillColorChanged(const QColor &c)
{
    QPixmap pix(14, 14);
    pix.fill(c);
    actFillColor->setIcon(pix);
	m_fillColor = c;
	m_brush.setColor(c);
}

void NCRDShapePropertyManager::penChanged(const QPen & p)
{
	m_pen = p;
	int idx=0;
	switch( p.style() ) {
		case Qt::NoPen: idx=0; break;
		case Qt::SolidLine: idx=1; break;
		case Qt::DashLine: idx=2; break;
		case Qt::DotLine: idx=3; break;
		case Qt::DashDotLine: idx=4; break;
		case Qt::DashDotDotLine: idx=5; break;
		case Qt::CustomDashLine: idx=6; break;
		default: idx=0;
	}
	comboLineStyle->setCurrentIndex(idx);
}

void NCRDShapePropertyManager::brushChanged(const QBrush & b)
{
	m_brush = b;
	int idx=0;
	switch( b.style() ) {
		case Qt::NoBrush: idx=0; break;
		case Qt::SolidPattern: idx=1; break;
		case Qt::Dense1Pattern: idx=2; break;
		case Qt::Dense2Pattern: idx=3; break;
		case Qt::Dense3Pattern: idx=4; break;
		case Qt::Dense4Pattern: idx=5; break;
		case Qt::Dense5Pattern: idx=6; break;
		case Qt::Dense6Pattern: idx=7; break;
		case Qt::Dense7Pattern: idx=8; break;
		case Qt::HorPattern: idx=9; break;
		case Qt::VerPattern: idx=10; break;
		case Qt::CrossPattern: idx=11; break;
		case Qt::BDiagPattern: idx=12; break;
		case Qt::FDiagPattern: idx=13; break;
		case Qt::DiagCrossPattern: idx=14; break;
		default: idx=0;
	}
	comboBrush->setCurrentIndex(idx);
}

void NCRDShapePropertyManager::buildMenu( QMenu * )
{
}

void NCRDShapePropertyManager::buildToolBar(QToolBar *tb)
{
	m_toolbar = tb;
	if ( m_toolbar ) {	
		
/*		comboLineWidth = new QComboBox(m_toolbar);
		comboLineWidth->setEditable(true);
		connect(comboLineWidth, SIGNAL(activated(const QString &)), this, SLOT(lineWidth(const QString &)));
		QStringList values;
		for ( double i=0.1; i<=1.0; i+=0.1 )
			values << QString::number(i);
		comboLineWidth->addItems(values);*/
		spinLineWidth = new QDoubleSpinBox( m_toolbar );
		spinLineWidth->setMinimum( 0.0 );
		spinLineWidth->setMaximum( 10.0 );
		spinLineWidth->setDecimals(2);
		spinLineWidth->setSingleStep( 0.05 );
		spinLineWidth->setSuffix(" mm");
		connect(spinLineWidth, SIGNAL(valueChanged(double)), this, SLOT(lineWidth(double)));
		
		comboLineStyle = new NCRDLineStyleCombo(m_toolbar);
		connect(comboLineStyle, SIGNAL(activated(int)), this, SLOT(lineStyle(int)));
		comboLineStyle->setCurrentIndex(1);
	
		comboBrush = new NCRDFillPatternCombo(m_toolbar);
		connect(comboBrush, SIGNAL(activated(int)), this, SLOT(fillPattern(int)));
		comboBrush->setCurrentIndex(0);
		
		m_toolbar->addWidget(comboLineStyle);
		m_toolbar->addWidget(spinLineWidth);
		m_toolbar->addAction( actLineColor );
		m_toolbar->addSeparator();
		m_toolbar->addWidget(comboBrush);
		m_toolbar->addAction( actFillColor );
	}	
	
	setEnabled(false);
}

void NCRDShapePropertyManager::applyChangesToItem(NCReportItem *item, int property)
{
	if ( !item )
		return;
	NCReportShapeItem *shape = (NCReportShapeItem*)(item);

    if (property==All) {
        shape->setBrush(m_brush);
        shape->setLineWidth( spinLineWidth->value());
    } else {

        switch (property) {
        case Color:
        case LineColor:
        case LineStyle:
            shape->setPen(m_pen);
            break;
        case FillColor:
        case FillPattern:
            shape->setBrush(m_brush);
            break;
        case LineWidth:
            shape->setLineWidth( spinLineWidth->value());
            break;
        default:
            break;
        }
    }

	item->update();
	if ( m_doc )
		m_doc->setDocumentModified();
}

void NCRDShapePropertyManager::applyChangesToManager(NCReportItem *item)
{
	if ( !item )
		return;
	NCReportShapeItem *shape = (NCReportShapeItem*)(item);
	
	penChanged(shape->pen());
	brushChanged(shape->brush());
	lineColorChanged(shape->pen().color());
	lineWidthChanged(shape->lineWidth());
	fillColorChanged(shape->brush().color());
	setEnabled( true );
}


void NCRDShapePropertyManager::reset()
{
	penChanged( QPen() );
	brushChanged( QBrush() );
	fillColorChanged( Qt::white );
	lineColorChanged( Qt::black );
	lineWidthChanged( 0.1 );
}

QPen NCRDShapePropertyManager::pen() const
{
	return m_pen;
}

QBrush NCRDShapePropertyManager::brush() const
{
	return m_brush;
}

QColor NCRDShapePropertyManager::currentLineColor() const
{
	return m_lineColor;
}

QColor NCRDShapePropertyManager::currentFillColor() const
{
	return m_fillColor;
}

void NCRDShapePropertyManager::setEnabled(bool set)
{
	comboLineStyle->setEnabled(set);
	spinLineWidth->setEnabled(set);
	comboBrush->setEnabled(set);
	actLineColor->setEnabled(set);
	actFillColor->setEnabled(set);
}


