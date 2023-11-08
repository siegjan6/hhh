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

#ifndef _ABOUTDLGIMPL_H
#define _ABOUTDLGIMPL_H

#include "ui_AboutDlgQt4.h"

class AboutDlgImpl : public QDialog
{
    Q_OBJECT

public:
    AboutDlgImpl( QWidget* parent = 0, Qt::WindowFlags fl = 0 );

public:
    Ui::AboutDlg ui;
};

#endif  //_ABOUTDLGIMPL_H
