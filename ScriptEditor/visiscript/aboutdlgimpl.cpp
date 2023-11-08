/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2012 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2012 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#include "aboutdlgimpl.h"

AboutDlgImpl::AboutDlgImpl( QWidget* parent, Qt::WindowFlags fl )
: QDialog( parent, fl )
{
    ui.setupUi(this);
    QPixmap aIcon(":/icons/visiscript_512x512.png");
    ui.m_pPixmap->setPixmap(aIcon.scaled(128,128));
}
