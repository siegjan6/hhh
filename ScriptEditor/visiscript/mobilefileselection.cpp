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

#include "configure.h"

#include "mobilefileselection.h"

#include "ui_mobilefileselection.h"

#include <QDir>

// ************************************************************************

// shorten file name to fit in button text for mobile devices
static QString getShortFileName(const QString & sFileName, QLabel * pLabel)
{
    bool bIsShortend = false;
    QString sTemp = sFileName;
    int iLabelPixels = pLabel->width();
    int iTextWidthPixels = pLabel->fontMetrics().width(sTemp);
    // compare real width of label with needed pixels for showing text
    while( iTextWidthPixels>iLabelPixels )
    {
        sTemp = sTemp.mid(1);       // shorten string by one character
        iTextWidthPixels = pLabel->fontMetrics().width("..."+sTemp);
        bIsShortend = true;
    }
    return bIsShortend ? "..."+sTemp : sTemp;
}

// ************************************************************************

MobileFileSelection::MobileFileSelection(QWidget *parent, const QString & sPath, const QString & sHomePath, const QString & sSDCardPath, bool bOnlyDirs, bool bMultipleSelection, bool bOnlySubdirs)
  : ui(new Ui::mobilefileselection),
    m_sHomePath(QDir::toNativeSeparators(sHomePath)),
    m_sSDCardPath(QDir::toNativeSeparators(sSDCardPath)),
    m_bDirModus(bOnlyDirs),
    m_bOnlySubdirs(bOnlySubdirs),
    m_bSaveAs(false),
    m_bDeleteModus(false),
    m_bNavigateModus(true)
{
    Q_UNUSED(parent);

    ui->setupUi(this);

    QObject::connect(ui->btnMFDParentDir, SIGNAL(clicked()), this, SLOT(sltNavigateToParentDir()));
    QObject::connect(ui->lstMFDDirectory, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(sltItemSelected(QModelIndex)));
    QObject::connect(ui->lstMFDDirectory, SIGNAL(clicked(QModelIndex)), this, SLOT(sltItemActivated(QModelIndex)));
    QObject::connect(ui->btnMFDOpen, SIGNAL(clicked()), this, SLOT(sltAcepted()));
    QObject::connect(ui->btnMFDCancel, SIGNAL(clicked()), this, SLOT(sltRejected()));
    QObject::connect(ui->btnMFDSDCard, SIGNAL(clicked()), this, SLOT(sltNavigateToSDCardDir()));
    QObject::connect(ui->txtMFDInput, SIGNAL(textChanged(QString)), this, SLOT(sltInputTextChanged(QString)));
    QObject::connect(ui->btnMFDHome, SIGNAL(clicked()), this, SLOT(sltNavigateToHomeDir()));
    QObject::connect(ui->btnMFDNewDirectory, SIGNAL(clicked()), this, SLOT(sltNewDirectory()));
    QObject::connect(ui->chbNavigate, SIGNAL(toggled(bool)), this, SLOT(sltNavigateChanged(bool)));

    //QMetaObject::connectSlotsByName(parent);

    ui->btnMFDOpen->setEnabled(false);
    ui->btnMFDNewDirectory->setEnabled(false);
    ui->chbNavigate->setChecked(m_bNavigateModus);
    sltNavigateChanged(m_bNavigateModus);

    ui->chbNavigate->setVisible(bOnlyDirs);

#if defined(Q_OS_ANDROID)
    //ui->lstMFDDirectory->setStyleSheet("QListView { background-color: black }");
#endif

    QString sTempPath = QDir::toNativeSeparators(sPath);
    if( sTempPath.startsWith(m_sHomePath) )
    {
        m_sRootPath = m_sHomePath;
    }
    else if( sTempPath.startsWith(m_sSDCardPath) && m_sSDCardPath.length()>1/*ignore just / */ )
    {
        m_sRootPath = m_sCurrentPath;
    }
    else
    {
#ifdef Q_OS_WIN
        m_sRootPath = "c:\\";
#else
        m_sRootPath = "/";
#endif
    }

    //ui->lstMFDDirectory->setViewMode(QListView::IconMode);

    ui->lstMFDDirectory->setSelectionMode(bMultipleSelection?QAbstractItemView::MultiSelection:QAbstractItemView::SingleSelection);

    QDir aHomePath(sHomePath);
    aHomePath.makeAbsolute();
    m_sHomePath = QDir::toNativeSeparators(aHomePath.absolutePath());

    QDir aSDCardPath(sSDCardPath);
    aSDCardPath.makeAbsolute();
    m_sSDCardPath = QDir::toNativeSeparators(aSDCardPath.absolutePath());

    if( m_bDirModus )
    {
        m_aFileSystemModel.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot );
    }
    else
    {
        m_aFileSystemModel.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs);
    }

    ui->lstMFDDirectory->setModel(&m_aFileSystemModel);
    m_aFileSystemModel.sort(0,Qt::AscendingOrder);

    setRoot(m_sRootPath);
    updateRoot(m_sRootPath);

    QDir aPath(sPath);
    aPath.makeAbsolute();
    QString s = aPath.absolutePath();

    ui->lstMFDDirectory->show();

    navigateTo(s);

    toggleSaveAsModus(m_bSaveAs);
}

MobileFileSelection::~MobileFileSelection()
{
    delete ui;
}

bool MobileFileSelection::navigateTo( const QString & sFullPath )
{
    bool bOk = true;

    QString s;
    QDir aDir(sFullPath);
    aDir.makeAbsolute();
    QStringList aPaths = aDir.absolutePath().split(QDir::separator());
    foreach( const QString & sDir, aPaths )
    {
        if( sDir.length()>0 )
        {
            s += QDir::separator();
            s += sDir;
            updateRoot(s);
        }
    }

    return bOk;
}

void MobileFileSelection::update()
{
    updateRoot(m_sCurrentPath);
}

void MobileFileSelection::toggleSaveAsModus(bool bSaveAs)
{
    m_bSaveAs = bSaveAs;
    ui->lblMFDInput->setVisible(bSaveAs);
    ui->txtMFDInput->setVisible(bSaveAs);
    ui->btnMFDNewDirectory->setVisible(bSaveAs);
    ui->lblMFDInput->setText(tr("Filename:"));
    if( bSaveAs )
    {
        ui->btnMFDOpen->setText(tr("save"));
    }
    else
    {
        ui->btnMFDOpen->setText(tr("open"));
    }
}

void MobileFileSelection::toggleDeleteModus(bool bDeleteModus)
{
    m_bDeleteModus = bDeleteModus;
    ui->lblMFDInput->setVisible(!bDeleteModus);
    ui->txtMFDInput->setVisible(!bDeleteModus);
    ui->btnMFDNewDirectory->setVisible(!bDeleteModus);
    if( bDeleteModus )
    {
        ui->btnMFDOpen->setText(tr("delete"));
    }
}

void MobileFileSelection::prepareForFileOpen()
{
    toggleDeleteModus(false);   // order is important, because controls will be shown or hidden
    toggleSaveAsModus(false);
}

void MobileFileSelection::prepareForFileSaveAs()
{
    toggleDeleteModus(false);   // order is important, because controls will be shown or hidden
    toggleSaveAsModus(true);
}

void MobileFileSelection::prepareForFileDelete()
{
    toggleDeleteModus(true);
}

QString MobileFileSelection::getFullFileName() const
{
    if( m_bSaveAs )
    {
        return m_sCurrentPath+QDir::separator()+ui->txtMFDInput->text();
    }
    else
    {
        QModelIndexList lstSelected = ui->lstMFDDirectory->selectionModel()->selectedIndexes();
        if( lstSelected.count()>0 )
        {
            if( ui->txtMFDInput->isVisible() )
            {
                return m_aFileSystemModel.filePath(lstSelected[0])+QDir::separator()+ui->txtMFDInput->text();
            }
            else
            {
                return m_aFileSystemModel.filePath(lstSelected[0]);
            }
        }
        else
        {
            return QString("");
        }
    }
}

QString MobileFileSelection::getFileName() const
{
    QModelIndexList lstSelected = ui->lstMFDDirectory->selectionModel()->selectedIndexes();
    if( lstSelected.count()>0 )
    {
        return m_aFileSystemModel.fileName(lstSelected[0]);
    }
    else
    {
        return QString("");
    }
}

void MobileFileSelection::setRoot(const QString & sNewRootPath)
{
    QDir aPath(sNewRootPath);
    aPath.makeAbsolute();
    m_sRootPath = QDir::toNativeSeparators(aPath.absolutePath());
    // change root path of filesystem model --> needed that sort works
    if( m_sRootPath.startsWith(m_sHomePath) )
    {
        m_aFileSystemModel.setRootPath(m_sHomePath);
    }
    else if( m_sRootPath.startsWith(m_sSDCardPath) )
    {
        m_aFileSystemModel.setRootPath(m_sSDCardPath);
    }
    else
    {
        // should never occure !
        m_aFileSystemModel.setRootPath("/");
    }
}

void MobileFileSelection::updateRoot(const QString & sFullPath)
{
    QModelIndex idx = m_aFileSystemModel.index(sFullPath);
    ui->lstMFDDirectory->setRootIndex(idx);
    m_sCurrentPath = QDir::toNativeSeparators(sFullPath);
    ui->lblMFDCurrentDir->setText(getShortFileName(sFullPath,ui->lblMFDCurrentDir));
    ui->btnMFDParentDir->setEnabled(m_sRootPath!=m_sCurrentPath);
    if( m_bSaveAs )
    {
        // clear input text if directory is changed --> really wanted ?
        ui->txtMFDInput->setText("");
    }
}

void MobileFileSelection::changeDirectory(const QModelIndex & index)
{
    QString sFullPath = m_aFileSystemModel.filePath(index);
    if( m_aFileSystemModel.isDir(index) )
    {
        updateRoot(sFullPath);
    }
}

void MobileFileSelection::sltItemSelected(const QModelIndex & index)
{
    if( m_bNavigateModus || m_aFileSystemModel.isDir(index) )
    {
        changeDirectory(index);
        ui->btnMFDOpen->setEnabled(false);
    }
}

void MobileFileSelection::sltInputTextChanged(const QString & sText)
{
    bool bEnable = sText.length()>0;
    ui->btnMFDOpen->setEnabled(bEnable);
    ui->btnMFDNewDirectory->setEnabled(bEnable);
}

void MobileFileSelection::sltItemActivated(const QModelIndex & index)
{
    if( m_bNavigateModus )
    {
        changeDirectory(index);
    }
    bool bIsDir = m_aFileSystemModel.isDir(index);
    ui->btnMFDOpen->setEnabled((m_bDirModus && bIsDir && !m_bNavigateModus) || !bIsDir);
    if( m_bSaveAs && !bIsDir )
    {
        ui->txtMFDInput->setText(getFileName());
    }
}

void MobileFileSelection::sltNavigateToParentDir()
{
    QString sFullPath = m_sCurrentPath;
    if( m_sRootPath.isEmpty() || m_sRootPath.length()<sFullPath.length() )
    {
        int iPos = sFullPath.lastIndexOf("\\");
        if( iPos==-1 )
        {
            iPos = sFullPath.lastIndexOf("/");
        }
        if( iPos>=0 )
        {
            QString sNewPath = sFullPath.mid(0,iPos);
            updateRoot(sNewPath);
        }
    }
}

void MobileFileSelection::sltNavigateToHomeDir()
{
    setRoot(m_sHomePath);
    updateRoot(m_sHomePath);
    ui->btnMFDOpen->setEnabled(false);
}

void MobileFileSelection::sltNavigateToSDCardDir()
{
    setRoot(m_sSDCardPath);
    updateRoot(m_sSDCardPath);
    ui->btnMFDOpen->setEnabled(false);
}

void MobileFileSelection::sltNewDirectory()
{
    QString sNewDirectory = m_sCurrentPath+QDir::separator()+ui->txtMFDInput->text();
    QDir aDir(sNewDirectory);
    bool ok = aDir.mkpath(sNewDirectory);
    if( !ok )
    {
        emit sigErrorMessage(tr("Error: can not create directory ")+sNewDirectory);
    }
}

void MobileFileSelection::sltNavigateChanged(bool bValue)
{
    m_bNavigateModus = bValue;
    ui->btnMFDOpen->setEnabled(!bValue);
}

void MobileFileSelection::sltAcepted()
{
    emit sigAcepted();
    accept();
}

void MobileFileSelection::sltRejected()
{
    emit sigRejected();
    reject();
}
