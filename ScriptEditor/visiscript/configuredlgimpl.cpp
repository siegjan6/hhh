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

#include <QInputDialog>
#include <QStringList>

#include "configuredlgimpl.h"

#include "selectfromlistdlg.h"

//*************************************************************************

extern QString OpenFileName(QWidget * parent, const QString & sTitle, const QString & sDir, const QString & sFilter = "", bool bUseRoot = false);
extern QStringList GetDefaultFileExtensions();

//*************************************************************************

bool UserScriptInterpreters::ExistsFor(const QString & sValue, int iRole ) const
{
    return GetIndexFor( sValue, iRole )!=-1;
}

int UserScriptInterpreters::GetIndexFor( const QString & sValue, int iRole ) const
{
    for( int i=0; i<size(); i++ )
    {
        const ScriptInterpreterConfiguration & item = (*this)[i];
        switch(iRole)
        {
            case 0:
                if( item.m_sInterpreterPath==sValue )
                {
                    return i;
                }
                break;
            case 1:
                if( item.m_sInterpreterArguments==sValue )
                {
                    return i;
                }
                break;
            case 2:
                if( item.m_sFileExtension==sValue )
                {
                    return i;
                }
                break;
            default:
                // ignore !
                ;
        }
    }
    return -1;
}

void UserScriptInterpreters::RemoveFor(const QString & sValue, int iRole)
{
    int index = GetIndexFor( sValue, iRole );
    if( index!=-1 )
    {
        this->removeAt( index );
    }
}

//*************************************************************************

ConfigureDlgImpl::ConfigureDlgImpl( const UserScriptInterpreters & aUserInterpreters, bool bUseRoot, QWidget* parent, Qt::WindowFlags fl )
: QDialog( parent, fl ),
  m_aUserInterpreters( aUserInterpreters ),
  m_bUseRoot( bUseRoot )
{
    ui.setupUi(this);

#if defined(Q_OS_ANDROID)
    ui.m_pScriptArguments->setBaseSize(0,c_iComboBoxHeight);
    ui.m_pScriptArguments->setMinimumHeight(c_iComboBoxHeight);
#endif
}

void ConfigureDlgImpl::sltSelectCommand()
{
    QString sFilter = "";
#ifdef Q_OS_WIN
    sFilter = "*.exe";
#endif
    QString sFileName = OpenFileName(this,tr("Select script interpreter"),".",sFilter,m_bUseRoot);
    ui.m_pScriptCmd->setText( sFileName );
}

void ConfigureDlgImpl::sltAssignExtension()
{
    SelectFromListDlg aDlg;
    aDlg.setWindowTitle(tr("Select file extension"));

    QString sEnter(tr("<enter>"));

    aDlg.AddItem(sEnter);   // to open an input dialog
    aDlg.AddItem("???");    // means: unknown
    foreach( const QString & s, GetDefaultFileExtensions() )
    {
        aDlg.AddItem(s);
    }

    QString sInitialExtension = ui.btnAssignExtension->text().mid(2);
    if( !aDlg.IsInList(sInitialExtension) )
    {
        aDlg.AddItem(sInitialExtension);
    }
    aDlg.SelectItem(sInitialExtension);  // *.??? --> ???

#if defined(Q_OS_ANDROID)
    aDlg.showMaximized();
#else
    //aDlg.adjustSize();
#endif

    if( aDlg.exec()==1 )
    {
        QString sResult = aDlg.GetSelectedItem();
        if( sResult==sEnter )
        {
            sResult = QInputDialog::getText( this, tr("Enter"), tr("enter new file extension:") );
            // maybe in the future --> add new item to extensions list
        }
        if( sResult.length()>0 )
        {
            ui.btnAssignExtension->setText("*."+sResult);
        }
    }
}

void ConfigureDlgImpl::sltScriptCmdChanged(const QString & sScriptCmd)
{
    int iScriptInterpreterConfigurationIndex = m_aUserInterpreters.GetIndexFor(sScriptCmd,0);
    if( iScriptInterpreterConfigurationIndex!=-1 )
    {
        ui.btnAssignExtension->setText("*."+m_aUserInterpreters[iScriptInterpreterConfigurationIndex].m_sFileExtension);
    }
    else
    {
        ui.btnAssignExtension->setText("*.???");
    }
}
