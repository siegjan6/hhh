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
#include "nccolorcombo.h"

#include <QColorDialog>
#include <QPainter>
#include <QPixmap>

#define COLORARRAY_SIZE 17

static QColor *standardPalette = 0;

static void createStandardPalette()
{
    if ( standardPalette )
    return;

    standardPalette = new QColor [COLORARRAY_SIZE];

    int i = 0;

    standardPalette[i++] = Qt::red;
    standardPalette[i++] = Qt::green;
    standardPalette[i++] = Qt::blue;
    standardPalette[i++] = Qt::cyan;
    standardPalette[i++] = Qt::magenta;
    standardPalette[i++] = Qt::yellow;
    standardPalette[i++] = Qt::darkRed;
    standardPalette[i++] = Qt::darkGreen;
    standardPalette[i++] = Qt::darkBlue;
    standardPalette[i++] = Qt::darkCyan;
    standardPalette[i++] = Qt::darkMagenta;
    standardPalette[i++] = Qt::darkYellow;
    standardPalette[i++] = Qt::white;
    standardPalette[i++] = Qt::lightGray;
    standardPalette[i++] = Qt::gray;
    standardPalette[i++] = Qt::darkGray;
    standardPalette[i++] = Qt::black;
}


NCColorCombo::NCColorCombo( QWidget *parent ) : QComboBox( parent )
{
    _showEmptyList=false;

    customColor.setRgb( 255, 255, 255 );
    internalcolor.setRgb( 255, 255, 255 );

    createStandardPalette();

    setIconSize( QSize(25,15) );
    addColors();
    //populateList();

    connect( this, SIGNAL( activated(int) ), SLOT( slotActivated(int) ) );
    connect( this, SIGNAL( highlighted(int) ), SLOT( slotHighlighted(int) ) );
}


NCColorCombo::~NCColorCombo()
{
    //delete d;
}

void NCColorCombo::setColor( const QColor &col )
{
    internalcolor = col;
    _showEmptyList=false;
    addColors();
}


QColor NCColorCombo::color() const {
    return internalcolor;
}

void NCColorCombo::resizeEvent( QResizeEvent *re )
{
    QComboBox::resizeEvent( re );
    addColors();
}

void NCColorCombo::showEmptyList()
{
    _showEmptyList=true;
    addColors();
}

void NCColorCombo::slotActivated( int index )
{
    if ( index == 0 ) {
        QColor c = QColorDialog::getColor( customColor, this );
        if ( c.isValid() ) {
            customColor = c;
            QPainter painter;
            QPen pen;
            QRect rect( 0, 0, width(), QFontMetrics(painter.font()).height()+4);
            QPixmap pixmap( rect.width(), rect.height() );

            if ( qGray( customColor.rgb() ) < 128 )
                pen.setColor( Qt::white );
            else
                pen.setColor( Qt::black );

            painter.begin( &pixmap );
            QBrush brush( customColor );
            painter.fillRect( rect, brush );
            painter.setPen( pen );
            painter.drawText( 2, QFontMetrics(painter.font()).ascent()+2, tr("Custom...") );
            painter.end();

            setItemIcon( 0, QIcon(pixmap) );
            pixmap.detach();
        }

        internalcolor = customColor;
    }
    else
        internalcolor = standardPalette[ index - 1 ];

    emit activated( internalcolor );
}

void NCColorCombo::slotHighlighted( int index )
{
    if ( index == 0 )
        internalcolor = customColor;
    else
        internalcolor = standardPalette[ index - 1 ];

    emit highlighted( internalcolor );
}

void NCColorCombo::addColors()
{
    QPainter painter;
    QPen pen;
    //QRect rect( 0, 0, width(), QFontMetrics(painter.font()).height()+4 );
    QRect rect( 0, 0, width(), 15 );
    QPixmap pixmap( rect.width(), rect.height() );
    int i;

    clear();
    if (_showEmptyList) return;

    createStandardPalette();

    for ( i = 0; i < COLORARRAY_SIZE; i++ )
        if ( standardPalette[i] == internalcolor ) break;

    if ( i == COLORARRAY_SIZE )
        customColor = internalcolor;

    if ( qGray( customColor.rgb() ) < 128 )
        pen.setColor( Qt::white );
    else
        pen.setColor( Qt::black );

    painter.begin( &pixmap );
    QBrush brush( customColor );
    painter.fillRect( rect, brush );
    painter.setPen( pen );
    painter.drawText( 2, QFontMetrics(painter.font()).ascent()+2, tr("Custom...") );
    painter.end();

    int idx =0;
    insertItem( idx, QIcon(pixmap), ""/*tr("Custom...")*/ );
    pixmap.detach();

    for ( i = 0; i < COLORARRAY_SIZE; i++ )
    {
        idx++;
        painter.begin( &pixmap );
        QBrush brush( standardPalette[i] );
        painter.fillRect( rect, brush );
        painter.end();

        insertItem( idx, QIcon(pixmap), "" );
        pixmap.detach();

        if ( standardPalette[i] == internalcolor )
            setCurrentIndex( i + 1 );
    }
}

void NCColorCombo::setColor( const QString & colorname )
{
    setColor( QColor(colorname) );
}

QString NCColorCombo::colorName() const
{
    return internalcolor.name();
}

void NCColorCombo::populateList()
{
    QStringList colorNames = QColor::colorNames();

    insertItem(0, tr("Custom..."));
    for (int i = 1; i < colorNames.size(); ++i) {
        QColor color(colorNames[i]);

        insertItem(i, colorNames[i]);
        setItemData(i, color, Qt::DecorationRole);
    }
}
