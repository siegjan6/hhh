/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2011 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2011 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#ifndef _OUTPUTAREAMULTILINEEDIT_H
#define _OUTPUTAREAMULTILINEEDIT_H

//#include <QPlainTextEdit>     // is much slower with append() !?
#include <QTextEdit>

class QMouseEvent;

class OutputAreaMultiLineEdit : public QTextEdit //QPlainTextEdit
{
	Q_OBJECT

public:
    OutputAreaMultiLineEdit( QWidget * parent=0, const char * name=0 );
    virtual ~OutputAreaMultiLineEdit();

	bool hasMarkedTxt();
	QString markedTxt();

    bool isLocked() const;
    void setLocked( bool value );

    //void setText( const QString & sText );
    //void append( const QString & sText );

    void PrintContent();

signals:
    void doubleClicked( int para, int pos );

protected:
    virtual void mouseDoubleClickEvent ( QMouseEvent * e );

private:
    bool    m_bLocked;
};

#endif //_OUTPUTAREAMULTILINEEDIT_H
