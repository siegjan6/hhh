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
#ifndef NCRDIMAGEPROPDIALOG_H
#define NCRDIMAGEPROPDIALOG_H

#include <QDialog>
#include "ui_imagepropdialog.h"
#include "ncreportimageitem.h"

#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
#include <QImage>
#else
#include <QPixmap>
#endif

class NCRDDocument;
class NCReportItem;
class NCReportHttpClient;

QT_BEGIN_NAMESPACE
class QButtonGroup;
QT_END_NAMESPACE

/*!
Image item properties dialog class
*/

/*!

\section 功能

图片控件属性配置对话框管理类
＃＃＃在代码中暂时屏蔽了此项功能＃＃＃

*/
class NCRDImagePropDialog : public QDialog, private Ui::NCRDImagePropDialogUI
{
  Q_OBJECT

public:
  NCRDImagePropDialog( NCReportItem *item, NCRDDocument* parent );
  ~NCRDImagePropDialog();
  /*$PUBLIC_FUNCTIONS$*/
    void applyChanges();
    void discardChanges();

public slots:
  /*$PUBLIC_SLOTS$*/
    void accept();
    void reject();

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
    void updateEnabled( bool );
    void updatePreview();
    void imageChanged();
    void selectFile();
    void httpFinished();

private:
    NCReportItem *m_item;
    NCRDDocument *m_doc;
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
    QImage m_image;
#else
    QPixmap m_image;
#endif
    QString svgXmlText;
    NCReportHttpClient* http;
};

#endif

