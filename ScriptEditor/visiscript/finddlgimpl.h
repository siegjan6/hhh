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

#ifndef _FINDDLGIMPL_H
#define _FINDDLGIMPL_H

#include "ui_FindDlgQt4.h"

class FindDlgImpl : public QDialog
{
    Q_OBJECT

public:
    FindDlgImpl( QWidget* parent = 0, Qt::WindowFlags fl = 0 );

public:
    Ui::FindDlg ui;
};

#endif //_FINDDLGIMPL_H
