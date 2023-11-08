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

#include "extensionsdialog.h"
#include "ui_extensionsdialog.h"

#include "extensions.h"
#include "inline_code.h"

#include <QDesktopServices>
#include <QUrl>

ExtensionsDialog::ExtensionsDialog( const QList<IVisiScriptExtension *> & lstExtensions, QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::ExtensionsDialog)
{
    ui->setupUi(this);

    FillExtenstionTable(ui->m_lstExtensions,lstExtensions,false);

    if( ui->m_lstExtensions->rowCount()==0 )
    {
        ui->m_lstExtensions->hide();
    }
    else
    {
        ui->btnLoadExtensionsFromPlay->hide();
    }
}

ExtensionsDialog::~ExtensionsDialog()
{
    delete ui;
}

void ExtensionsDialog::sltLoadExtensions()
{
    QDesktopServices::openUrl( QUrl( "https://play.google.com/store/apps/details?id=de.mneuroth.visiscriptextensions" ) );
}
