/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2014 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2014 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#ifndef EXTENSIONSDIALOG_H
#define EXTENSIONSDIALOG_H

#include <QDialog>
#include <QList>

class IVisiScriptExtension;

namespace Ui {
class ExtensionsDialog;
}

class ExtensionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtensionsDialog( const QList<IVisiScriptExtension *> & lstExtensions, QWidget *parent = 0 );
    ~ExtensionsDialog();

public slots:
    void sltLoadExtensions();

private:
    Ui::ExtensionsDialog *ui;
};

#endif // EXTENSIONSDIALOG_H
