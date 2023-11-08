/**************************************************************************
 *
 *	project				 : VisiScript
 *
 *	copyright            : (C) 2013 by Michael Neuroth
 *
 ***************************************************************************/
/*********************************************************************************
 *																				 *
 * This file is part of the VisiScript package (a frontend for script languages) *
 *																				 *
 * Copyright (C) 2013 by Michael Neuroth.										 *
 *                                                                               *
 * This program is free software; you can redistribute it and/or modify			 *
 * it under the terms of the GNU General Public License as published by    		 *
 * the Free Software Foundation; either version 2 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 ********************************************************************************/

#ifndef MOBILEFILESELECTION_H
#define MOBILEFILESELECTION_H

#include "configure.h"

#include <QObject>
#include <QDialog>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QVariant>

class QListView;

class Ui_mobilefileselection;

class MobileFileSelection : public QDialog
{
    Q_OBJECT

public:
    explicit MobileFileSelection(QWidget *parent, const QString & sPath = "",
                                 const QString & sHomePath = "", const QString & sSDCardPath = "",
                                 bool bOnlyDirs = false, bool bMultipleSelection = false,
                                 bool bOnlySubdirs = false);
    ~MobileFileSelection();

    QString getFullFileName() const;

    void update();

    void prepareForFileOpen();
    void prepareForFileSaveAs();
    void prepareForFileDelete();

signals:
    void sigAcepted();
    void sigRejected();
    void sigErrorMessage(const QString & sMsg);

public slots:
    void sltItemActivated(const QModelIndex & index);
    void sltItemSelected(const QModelIndex & index);
    void sltInputTextChanged(const QString & sText);
    void sltNavigateToParentDir();
    void sltNavigateToHomeDir();
    void sltNavigateToSDCardDir();
    void sltNewDirectory();
    void sltNavigateChanged(bool bValue);
    void sltAcepted();
    void sltRejected();

private:
    QString getFileName() const;
    void updateRoot(const QString & sFullPath);
    void setRoot(const QString & sNewRootPath);
    void changeDirectory(const QModelIndex & index);
    void toggleSaveAsModus(bool bSaveAs);
    void toggleDeleteModus(bool bDeleteModus);
    bool navigateTo(const QString & sFullPath );

    Ui_mobilefileselection *    ui;                 // not an owner !!!

    QFileSystemModel    m_aFileSystemModel;
    QString             m_sCurrentPath;
    QString             m_sRootPath;
    QString             m_sHomePath;
    QString             m_sSDCardPath;
    bool                m_bDirModus;
    bool                m_bOnlySubdirs;
    bool                m_bSaveAs;
    bool                m_bDeleteModus;
    bool                m_bNavigateModus;
};

#endif // MOBILEFILESELECTION_H
