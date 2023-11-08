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

#ifndef _CONFIGUREDLGIMPL_H
#define _CONFIGUREDLGIMPL_H

#include "ui_ConfigureDlgQt4.h"

#include "configure.h"

// ************************************************************************

// create only if an extension is given by the user
struct ScriptInterpreterConfiguration
{
    QString     m_sInterpreterPath;         // role 0
    QString     m_sInterpreterArguments;    // role 1
    QString     m_sFileExtension;           // role 2
};

// ************************************************************************

class UserScriptInterpreters : public QList<ScriptInterpreterConfiguration>
{
public:
    bool ExistsFor( const QString & sValue, int iRole ) const;
    int GetIndexFor(const QString & sValue, int iRole ) const;
    void RemoveFor( const QString & sValue, int iRole );
};

// ************************************************************************

class ConfigureDlgImpl : public QDialog
{
    Q_OBJECT

public:
    ConfigureDlgImpl( const UserScriptInterpreters & aUserInterpreters, bool bUseRoot, QWidget* parent = 0, Qt::WindowFlags fl = 0 );

public slots:
    void sltSelectCommand();
    void sltAssignExtension();
    void sltScriptCmdChanged(const QString & sScriptCmd);

public:
    Ui::ConfigureDlg ui;

private:
    const UserScriptInterpreters    m_aUserInterpreters;
    bool                            m_bUseRoot;
};

#endif //_CONFIGUREDLGIMPL_H
