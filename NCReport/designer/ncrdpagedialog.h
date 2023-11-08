/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#ifndef NCRDPAGEDIALOG_H
#define NCRDPAGEDIALOG_H

#include <QDialog>
#include "ui_pagedialog.h"
#include "ncrd_global.h"

class NCRDDocument;

/*!
Page options dialog class
*/

/*!

\section 功能

页面属性对话框管理类
【管理页面的大小/语言／字体等属性】

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDPageDialog : public QDialog, private Ui::NCRDPageDialogUI
{
  Q_OBJECT

public:
  NCRDPageDialog( NCRDDocument* doc, QWidget* parent = 0 );
  ~NCRDPageDialog();
  /*$PUBLIC_FUNCTIONS$*/
    void applyChanges();
    void discardChanges();

    QTabWidget* mainTabWidget();

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
    void pageSizeChanged( const QString & );
    void addLang();
    void removeLang();
    void langUp();
    void langDown();
private:
    NCRDDocument *document;

    void setupPage();
    void addLanguages( const QString& languageCodes );
    QString languages() const;
    void moveLangItem( int moveBy );
};

#endif
