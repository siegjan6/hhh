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
#include "ncrdimagepropdialog.h"
#include "ncrddocument.h"
#include "ncreportutils.h"
#include "ncrdformgeometry.h"
#include "ncreporthttpclient.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QSvgRenderer>
#include <QPainter>

#include <QAbstractButton>
#include <QPlainTextEdit>

//! 多语言
#include "MDMultiLanguage.h"

NCRDImagePropDialog::NCRDImagePropDialog(NCReportItem * item, NCRDDocument * parent)
: QDialog( parent ), Ui::NCRDImagePropDialogUI(), m_item(item), m_doc(parent)
{
    setupUi(this);

    NCReportImageItem *imageItem = (NCReportImageItem*)item;
    isPanel->setItem(item);
    isPanel->setDocument(parent);
    isPanel->setSourceType( item->sourceType() );
    //isPanel->setSourceTypeEnabled(NCReportItem::Parameter,false);
    isPanel->setSourceTypeEnabled(NCReportItem::InternalDocument,false);
    isPanel->setSourceText( imageItem->text() );

    connect( isPanel->buttonSelect(), SIGNAL(clicked()), this, SLOT(selectFile()) );
    connect( isPanel->buttonPreview(), SIGNAL(clicked()), this, SLOT(imageChanged()) );

//    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    isPanel->sourceEdit()->setSizePolicy(sizePolicy);
//    isPanel->sourceEdit()->setMaximumHeight(1024);

    m_image = imageItem->image();

    comboFormat->setCurrentIndex( (int)imageItem->imageFormat() );
    svgXmlText = imageItem->svg();

    switch ( imageItem->aspectRatioMode() ) {
        case Qt::IgnoreAspectRatio: rbIgnoreAspect->setChecked(true); break;
        case Qt::KeepAspectRatio: rbKeepAspect->setChecked(true); break;
        case Qt::KeepAspectRatioByExpanding: rbExpandAspect->setChecked(true); break;
    }
    switch ( imageItem->transformMode() ) {
        case Qt::FastTransformation: rbFastTran->setChecked(true); break;
        case Qt::SmoothTransformation: rbSmoothTran->setChecked(true); break;
    }

    printWhenPanel->setOptions(imageItem,parent);

    comboHAlign->setCurrentIndex( NCReportUtils::horizontalAlignmentToIndex(imageItem->alignment() ) );
    comboVAlign->setCurrentIndex( NCReportUtils::verticalAlignmentToIndex(imageItem->alignment() ));

    wHtmlWidth->setText(imageItem->htmlWidth());
    wHtmlHeight->setText(imageItem->htmlHeight());
    cbPinToRight->setChecked( imageItem->pinToRight() );

#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
    lblPreview->setPixmap( QPixmap::fromImage(imageItem->image()) );
#else
    lblPreview->setPixmap( imageItem->image() );
#endif
    updatePreview();
    updateEnabled( imageItem->isScaling() );

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    http = new NCReportHttpClient(this);
    connect(http, SIGNAL(finished()), this, SLOT(httpFinished()));

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDImagePropDialog::~NCRDImagePropDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDImagePropDialog::applyChanges()
{
    NCReportImageItem *imageItem = (NCReportImageItem*)m_item;

    imageItem->setSourceType( isPanel->sourceType() );

    if ( imageItem->sourceType() == NCReportItem::DataSource || imageItem->sourceType() == NCReportItem::FileNameFromDataSource )
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
        imageItem->setImage( QImage() );
#else
        imageItem->setImage( QPixmap() );
#endif
    else
        imageItem->setImage( m_image );

    imageItem->setText( isPanel->sourceText() );
    //imageItem->setFileName( fileName->text() );
    //imageItem->setScaling( cbScaling->isChecked() );
    imageItem->setImageFormat( (NCReportImageItem::ImageFormat)comboFormat->currentIndex() );

    if ( rbIgnoreAspect->isChecked() )
        imageItem->setAspectRatioMode( Qt::IgnoreAspectRatio );
    else if ( rbKeepAspect->isChecked() )
        imageItem->setAspectRatioMode( Qt::KeepAspectRatio );
    else if ( rbExpandAspect->isChecked() )
        imageItem->setAspectRatioMode( Qt::KeepAspectRatioByExpanding );

    if ( rbFastTran->isChecked() )
        imageItem->setTransformMode( Qt::FastTransformation );
    else if ( rbSmoothTran->isChecked() )
        imageItem->setTransformMode( Qt::SmoothTransformation );

    printWhenPanel->getOptions(imageItem);

    imageItem->setAlignment( NCReportUtils::indexToHorizontalAlignment(comboHAlign->currentIndex()) | NCReportUtils::indexToVerticalAlignment(comboVAlign->currentIndex()) );
    imageItem->setHtmlWidth( wHtmlWidth->text().toLatin1() );
    imageItem->setHtmlHeight( wHtmlHeight->text().toLatin1() );
    imageItem->setPinToRight( cbPinToRight->isChecked() );

    if ( imageItem->imageFormat() == NCReportImageItem::Svg ) {
        imageItem->setSvg( svgXmlText.toUtf8() );
    }
    m_item->update();
    m_item->adjustSize();
    m_doc->setDocumentModified();
}

void NCRDImagePropDialog::discardChanges()
{
}

void NCRDImagePropDialog::selectFile()
{
    QFileInfo fi( isPanel->sourceText() );
    QString s;
    NCReportImageItem::ImageFormat m_format = (NCReportImageItem::ImageFormat)comboFormat->currentIndex();
    if ( m_format == NCReportImageItem::Binary || m_format == NCReportImageItem::Base64Encoded )
        s = QFileDialog::getOpenFileName( this, tr("Select image file"), fi.absoluteFilePath(), "Images (*.png *.jpg *.jpeg *.gif *.xpm *.bmp *.pbm *.ppm *.tiff *.svg)" );
    else if ( m_format == NCReportImageItem::Svg )
        s = QFileDialog::getOpenFileName( this, tr("Select SVG file"), fi.absoluteFilePath(), "SVG files (*.svg *.SVG)" );
    if ( s.isEmpty() )
        return;

    isPanel->setSourceText( s );
    imageChanged();
}

void NCRDImagePropDialog::imageChanged()
{
    //NCReportImageData *id = (NCReportImageData*)m_item->data();
    NCReportImageItem::ImageFormat m_format = (NCReportImageItem::ImageFormat)comboFormat->currentIndex();
    if ( m_format == NCReportImageItem::Binary || m_format == NCReportImageItem::Base64Encoded ) {

        if ( isPanel->sourceType() == NCReportItem::Url ) {
            http->downloadContent(isPanel->sourceText());	//start download
        } else {
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
            m_image = QImage( NCReportUtils::evaluatedFileName(isPanel->sourceText()) );
#else
            m_image = QPixmap( NCReportUtils::evaluatedFileName(isPanel->sourceText()) );
#endif
        }
    } else if ( m_format == NCReportImageItem::Svg ) {
        //imageItem->loadFromTextFile( fileName->text(), svgXmlText );
        NCReportUtils::fileToString( isPanel->sourceText(), svgXmlText );
    }

    updatePreview();
}

void NCRDImagePropDialog::httpFinished()
{
    if ( m_image.loadFromData( http->content() ) )
        updatePreview();
}

void NCRDImagePropDialog::updatePreview()
{
    NCReportImageItem::ImageFormat m_format = (NCReportImageItem::ImageFormat)comboFormat->currentIndex();

    //if ( m_image.isNull() )
    //	return;
    if ( m_format == NCReportImageItem::Binary || m_format == NCReportImageItem::Base64Encoded ) {

        if (!m_image.isNull()) {
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
            lblPreview->setPixmap( QPixmap::fromImage( m_image.scaled( QSize(160,160), Qt::KeepAspectRatio, Qt::SmoothTransformation )) );
#else
            lblPreview->setPixmap( QPixmap( m_image.scaled( QSize(160,160), Qt::KeepAspectRatio, Qt::SmoothTransformation )) );
#endif
        }

    } else if ( m_format == NCReportImageItem::Svg ) {
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
        m_image = QImage(160,160,QImage::Format_RGB32);
#else
        m_image = QPixmap(160,160);
        m_image.fill( this,0,0 );
#endif
        QPainter painter(&m_image);
        QSvgRenderer renderer;

        if ( renderer.load(svgXmlText.toUtf8()) ) {
            QSizeF size(renderer.defaultSize());
            size.scale( QSizeF(160,160), Qt::KeepAspectRatio );
            renderer.render( &painter, QRectF(QPointF(0,0),size) );
        }


        if ( !m_image.isNull() ) {

#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
            lblPreview->setPixmap( QPixmap::fromImage(m_image) );
#else
            lblPreview->setPixmap( m_image );
#endif
        }
    }
}

void NCRDImagePropDialog::updateEnabled(bool on)
{
    gbAspect->setEnabled(on);
    gbTrans->setEnabled(on);
}

void NCRDImagePropDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDImagePropDialog::reject()
{
    discardChanges();
    QDialog::reject();
}
