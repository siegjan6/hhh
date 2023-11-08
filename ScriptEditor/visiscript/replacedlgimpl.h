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

#ifndef _REPLACEDLGIMPL_H
#define _REPLACEDLGIMPL_H

#include "ui_ReplaceDlgQt4.h"

class ReplaceDlgImpl : public QDialog
{
    Q_OBJECT

public:
    ReplaceDlgImpl( QWidget* parent = 0, Qt::WindowFlags fl = 0 );

public:
    Ui::ReplaceDlg ui;
};

#endif
