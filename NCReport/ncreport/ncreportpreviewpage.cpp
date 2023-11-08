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
#include "ncreportpreviewpage.h"
#include "ncreportscale.h"

#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QPicture>
#include <QBuffer>

#if QT_VERSION >= 0x040600
#include <QProcessEnvironment>
#include <QGraphicsDropShadowEffect>
#endif

NCReportPreviewPage::NCReportPreviewPage( QWidget * parent ) : QWidget( parent ),
    m_content(QByteArray()), m_scale(1.0), m_sizeMM(QSizeF(0,0)), m_safeMode(false), m_dropShadow(true)
{
    setBackgroundRole( QPalette::NoRole );
//#if QT_VERSION >= 0x040600
//    m_avoid_rdp_crash = QProcessEnvironment::systemEnvironment().toStringList().contains("AVOID_RDP_CRASH=1");
//#endif

    if (m_dropShadow)
        setDropShadow(true);
}

NCReportPreviewPage::~ NCReportPreviewPage()
{
}

void NCReportPreviewPage::setContent(const QByteArray &content)
{
    m_content = content;
}

QByteArray NCReportPreviewPage::content() const
{
    return m_content;
}

void NCReportPreviewPage::paintEvent(QPaintEvent *e )
{
    Q_UNUSED(e)

    QPainter painterPage;
    QPixmap *pix=0;
    QSize pageSize = size();
    QPicture picture;
    picture.setBoundingRect( QRect(QPoint(0,0),pageSize));

    loadPicture(picture,m_content);

    if ( m_safeMode ) {
        //BY ANDY
        pix = new QPixmap(pageSize);
        painterPage.begin(pix);
    } else {
        painterPage.begin( this );  // paint in widget
    }

    painterPage.setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
    drawBackground( &painterPage );

    painterPage.scale( m_scale, m_scale );
    painterPage.drawPicture( 0, 0, picture );
    painterPage.end();
    if ( m_safeMode ) {
        QPainter painter;
        painter.begin(this);
        painter.drawPixmap(0,0,*pix);
        painter.end();
        delete pix;
    }
}

void NCReportPreviewPage::drawBackground( QPainter * painter )
{
    // Shadow and border
    const bool paintShadow=false;
    const bool paintOutLine=true;

    QRect paperRect(0,0,width(),height());
    painter->fillRect( paperRect, QBrush( Qt::white ) );

    // ----------- PAINT SHADOW 1
    if (paintShadow) {
        const int shmargin = 4;
        QRect rightShadow( width()-shmargin, shmargin, shmargin, height() );
        QRect bottomShadow( shmargin, height()-shmargin, width(), shmargin );
        painter->fillRect(rightShadow, Qt::darkGray);
        painter->fillRect(bottomShadow, Qt::darkGray);
    }
    // DRAW OUTLINE
    if (paintOutLine) {
        painter->setPen( Qt::gray );
        painter->drawRect( paperRect.adjusted(0,0,-1,-1) );
        painter->setPen( Qt::black );
    }
}

void NCReportPreviewPage::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit pageDoubleClicked(event->pos());
}

void NCReportPreviewPage::loadPicture(QPicture &picture, QByteArray& ba)
{
    QBuffer buffer(&ba,0);
    buffer.open(QIODevice::ReadOnly);
    picture.load(&buffer);
}

void NCReportPreviewPage::savePicture(QPicture &picture, QByteArray &ba)
{
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    picture.save(&buffer);
}

void NCReportPreviewPage::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case Qt::Key_R:
        break;
    default:
        QWidget::keyPressEvent(event);
    }

}

void NCReportPreviewPage::setScaleRate(qreal rate)
{
    NCReportScale sc;

    if (rate<0.1) rate=0.1;
    if (rate>7.0) rate=7.0;
    int w = sc.mmToPixel( m_sizeMM.width() * rate )+5;
    int h = sc.mmToPixel( m_sizeMM.height() * rate )+5;
    //resize( w, h );
    setFixedSize( w, h );

    m_scale = rate;
}

void NCReportPreviewPage::setPageSizeMM(const QSizeF & sizeMM)
{
    m_sizeMM = sizeMM;
    setScaleRate(m_scale);
}

qreal NCReportPreviewPage::scaleRate() const
{
    return m_scale;
}

void NCReportPreviewPage::setDropShadow(bool set)
{
#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
    Q_UNUSED(set)
    return;
#endif
    if (set) {
        if ( !graphicsEffect() ) {
            // DROP SHADOW
            QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
            shadow->setBlurRadius(7.0);
            shadow->setColor(Qt::darkGray);
            shadow->setOffset(4,4);
            setGraphicsEffect( shadow );
        }
    } else {
        if (graphicsEffect())
            setGraphicsEffect(0);
    }

    m_dropShadow = set;
}

bool NCReportPreviewPage::dropShadow() const
{
    return m_dropShadow;
}

QSizeF NCReportPreviewPage::pageSizeMM() const
{
    return m_sizeMM;
}

