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

#ifndef INLINE_CODE_H
#define INLINE_CODE_H

#include <QString>
#include <QByteArray>
#include <QList>
#include <QLibrary>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QTableWidget>
#include <QHeaderView>
#include <QCoreApplication>

#include "extensions.h"

typedef /*QList<IVisiScriptExtension *>*/ void * (*fcnCreateExtensions)();

inline bool extractAssetFile(const QString & sAssetFileName, const QString & sOutputFileName, bool bExecuteFlags, bool bForceInstall=false)
{
    if( bForceInstall || !QFile::exists(sOutputFileName) )
    {
        QFile aFile(sAssetFileName);
        if( aFile.open(QIODevice::ReadOnly) )
        {
            QByteArray aContent = aFile.readAll();
            aFile.close();

            QFileInfo aFileInfo(sOutputFileName);
            QString sPath = aFileInfo.absoluteDir().absolutePath();
            QDir aDir(sPath);
            aDir.mkpath(sPath);

            QFile aFileOut(sOutputFileName);
            aFileOut.open(QIODevice::WriteOnly);
            aFileOut.write(aContent);
            if( bExecuteFlags )
            {
                aFileOut.setPermissions(QFile::ExeGroup|QFile::ExeOther|QFile::ExeOwner|QFile::ExeUser|aFileOut.permissions());
            }
            aFileOut.close();

            return true;
        }
        return false;
    }
    return true;    // file already existed !
}

//#include <QDebug>

inline QList<IVisiScriptExtension *> TryLoadExtensions( const QString & sExtension )
{
    QList<IVisiScriptExtension *> lstExtensions;

    if( QFile::exists(sExtension) )
    {
        QLibrary pluginLoader(sExtension);
        pluginLoader.setLoadHints(QLibrary::ExportExternalSymbolsHint);

        if( pluginLoader.load() )
        {
            fcnCreateExtensions fcnLoadExt = (fcnCreateExtensions)pluginLoader.resolve("CreateExtensions");
            if( fcnLoadExt!=0 )
            {
                QList<IVisiScriptExtension *> * pLstExt = (QList<IVisiScriptExtension *> *)fcnLoadExt();
                for(int i =0 ;i < (*pLstExt).count();i++)
                {
                    lstExtensions.append((*pLstExt)[i]);
                }
            }
        }
        //else
        //{
        //    qDebug() << "Could not load plugin " << sExtension << " error:" << pluginLoader.errorString() << endl;
        //}
    }
//    else
//    {
//        qDebug() << "Could not find extension " << sExtension << endl;
//    }
    return lstExtensions;
}

inline void FillExtenstionTable( QTableWidget * pTable, const QList<IVisiScriptExtension *> & lstExtensions, bool bEnableEnableColumn = false )
{
    //pTable->clear();
    pTable->setRowCount(lstExtensions.count());
    int iLine = 0;
    for(int i =0 ;i < lstExtensions.count();i++)
    {
        IVisiScriptExtension * pExt = lstExtensions.at(i);
        QTableWidgetItem * pItem = new QTableWidgetItem(pExt->getName());
        pItem->setData(Qt::UserRole,QVariant(pExt->getLicenseUrl()));
        pItem->setData(Qt::UserRole+1,QVariant(pExt->getHomepageUrl()));
        pItem->setData(Qt::UserRole+2,QVariant(pExt->getHelpUrl()));
        pTable->setItem(iLine,0,pItem);
        pItem = new QTableWidgetItem("");
        pItem->setCheckState(pExt->isEnabled() ? Qt::Checked : Qt::Unchecked);
        if( !bEnableEnableColumn || !pExt->canDisable() )
        {
            pItem->setFlags(pItem->flags() & ~Qt::ItemIsEnabled);
        }
        pTable->setItem(iLine,1,pItem);
        pItem = new QTableWidgetItem(pExt->getVersion());
        pTable->setItem(iLine,2,pItem);
        pItem = new QTableWidgetItem(pExt->getDate());
        pTable->setItem(iLine,3,pItem);
        pItem = new QTableWidgetItem(pExt->getModule());
        pTable->setItem(iLine,4,pItem);
        iLine++;
    }
#if QT_VERSION >= 0x050000
     pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif
}

inline QString GetInstallationFilesPath()
{
    QString sPath = QCoreApplication::applicationDirPath();
    sPath += QDir::separator();
#if defined(Q_OS_ANDROID)
    sPath += "..";
    sPath += QDir::separator();
#endif
    sPath += "files";
    sPath = QDir::toNativeSeparators( sPath );
    QFileInfo aFileInfo(sPath);
    return aFileInfo.canonicalFilePath();
}

inline bool ContainsEncoding( const QString & s, const QString & sPattern1, const QString & sPattern2 )
{
    if( s.indexOf("coding")>=0 )
    {
        if( s.indexOf(sPattern1)>=0 )
        {
            return true;
        }
        else if( s.indexOf(sPattern2)>=0 )
        {
            return true;
        }
    }
    return false;
}

// search for something like: # -*- coding: utf-8 -*-
inline bool PeekFileForSignature( const QString & sFullFileName, const QString & sPattern1, const QString & sPattern2 )
{
    QFile aFile( sFullFileName );

    if( !aFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        return false;
    }
    else
    {
        bool bFound = false;

        QTextStream aInStream( &aFile );
        for( int i=0; i<3; i++ )
        {
            QString s = aInStream.readLine();
            if( ContainsEncoding(s,sPattern1,sPattern2) )
            {
                bFound = true;
                break;
            }
        }
        aFile.close();

        return bFound;
    }
}

inline bool PeekFileForUtf8Signature( const QString & sFullFileName )
{
    return PeekFileForSignature(sFullFileName,"utf-8","utf8");
}

inline bool PeekFileForLatin1Signature( const QString & sFullFileName )
{
    return PeekFileForSignature(sFullFileName,"latin-1","latin1");
}

inline bool ReadTextFile( const QString & sFullFileName, QString & sText, bool bUtf8 = true, QPair<bool,bool> * pPeekedEncodingIsUtf8 = 0 )
{
    QFile aFile( sFullFileName );

    if( !aFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        return false;
    }
    else
    {
        if( pPeekedEncodingIsUtf8!=0 )
        {
            bool bPeekForUtf8 = PeekFileForUtf8Signature(sFullFileName);
            bool bPeekForLatin1 = PeekFileForLatin1Signature(sFullFileName);
            (*pPeekedEncodingIsUtf8).first = bPeekForUtf8 || bPeekForLatin1;
            (*pPeekedEncodingIsUtf8).second = bPeekForUtf8;
            // set the encoding to the peeked value if any value is found !
            if( (*pPeekedEncodingIsUtf8).first )
            {
                bUtf8 = (*pPeekedEncodingIsUtf8).second;
            }
        }

        QTextStream aInStream( &aFile );
        if( bUtf8 )
        {
            aInStream.setCodec(QTextCodec::codecForName("UTF-8"));
        }
        else
        {
            aInStream.setCodec(QTextCodec::codecForName("ISO-8859-1"));
        }
        sText = aInStream.readAll();
        bool ok = aInStream.status()==QTextStream::Ok;

        aFile.close();

        return ok;
    }
}

inline QString AddStringKommasIfNeededAndConvertSeparators( const QString & sPath )
{
    QString ret = sPath;
    if( sPath.indexOf(' ')>=0 )
    {
        ret = "\"" + sPath + "\"";
    }
    return QDir::toNativeSeparators(ret);
}

inline QString GetInstallationFilesUrl()
{
    // Android returns: /data/data/de.mneuroth.visiscriptextensions/lib/files
    return "file://"+QDir::fromNativeSeparators(GetInstallationFilesPath()+QDir::separator());
}

#endif // INLINE_CODE_H
