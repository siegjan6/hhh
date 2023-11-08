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
#include "ncrdtextpropertymanager.h"
#include "ncrdmainwindow.h"
#include "ncrdsectionscene.h"
#include "ncrddef.h"
#include "ncrddocument.h"
#include "ncreportlabelitem.h"
#include "ncreporttextitem.h"
#include "ncreportutils.h"

#include <QAction>
#include <QApplication>
#include <QActionGroup>
#include <QFontComboBox>
#include <QComboBox>
#include <QFontDatabase>
#include <QFontMetricsF>
#include <QColorDialog>
#include <QMenu>
#include <QToolBar>
#include <QTextDocument>

NCRDTextPropertyManager::NCRDTextPropertyManager( NCRDMainWindow * parent )
    : NCRDPropertyManager( parent )
{
    comboFont =0;
    comboSize =0;

    actBold = new QAction(QIcon(":/designer/images/text_bold.png"), tr("&Bold"), this);
    actBold->setShortcut(Qt::CTRL + Qt::Key_B);
    QFont bold;
    bold.setBold(true);
    actBold->setFont(bold);
    actBold->setCheckable(true);
    connect(actBold, SIGNAL(triggered()), this, SLOT(textBold()));

    actItalic = new QAction(QIcon(":/designer/images/text_italic.png"), tr("&Italic"), this);
    actItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actItalic->setFont(italic);
    actItalic->setCheckable(true);
    connect(actItalic, SIGNAL(triggered()), this, SLOT(textItalic()));

    actUnderline = new QAction(QIcon(":/designer/images/text_underline.png"), tr("&Underline"), this);
    actUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    QFont underline;
    underline.setUnderline(true);
    actUnderline->setFont(underline);
    actUnderline->setCheckable(true);
    connect(actUnderline, SIGNAL(triggered()), this, SLOT(textUnderline()));

    //---------------------------
    // Horizontal alignment
    //---------------------------
    actGroupHAlign = new QActionGroup(this);
    connect(actGroupHAlign, SIGNAL(triggered(QAction *)), this, SLOT(textAlignHorizontal(QAction *)));

    actAlignLeft = new QAction(QIcon(":/designer/images/text_align_left.png"), tr("Left"), actGroupHAlign);
    actAlignLeft->setStatusTip(tr("Align text to left"));
    actAlignLeft->setCheckable(true);

    actAlignHCenter = new QAction(QIcon(":/designer/images/text_align_center.png"), tr("Center"), actGroupHAlign);
    actAlignHCenter->setStatusTip(tr("Align text to center horizontally"));
    actAlignHCenter->setCheckable(true);

    actAlignRight = new QAction(QIcon(":/designer/images/text_align_right.png"), tr("Right"), actGroupHAlign);
    actAlignRight->setStatusTip(tr("Align text to right"));
    actAlignRight->setCheckable(true);

    actAlignJustify = new QAction(QIcon(":/designer/images/text_align_justify.png"), tr("Justify"), actGroupHAlign);
    actAlignJustify->setStatusTip(tr("Align text to justify"));
    actAlignJustify->setCheckable(true);

    //---------------------------
    // Vertical alignment
    //---------------------------
    actGroupVAlign = new QActionGroup(this);
    connect(actGroupVAlign, SIGNAL(triggered(QAction *)), this, SLOT(textAlignVertical(QAction*)));

    actAlignTop = new QAction(QIcon(":/designer/images/align_above.png"), tr("Top"), actGroupVAlign);
    actAlignTop->setStatusTip(tr("Align text to top"));
    actAlignTop->setCheckable(true);

    actAlignVCenter = new QAction(QIcon(":/designer/images/align_middle.png"), tr("Middle"), actGroupVAlign);
    actAlignVCenter->setStatusTip(tr("Align text to center vertically"));
    actAlignVCenter->setCheckable(true);

    actAlignBottom = new QAction(QIcon(":/designer/images/align_bellow.png"), tr("Bottom"), actGroupVAlign);
    actAlignBottom->setStatusTip(tr("Align text to bottom"));
    actAlignBottom->setCheckable(true);

    //---------------------------
    // Color
    //---------------------------
    QPixmap pix(14, 14);
    pix.fill(Qt::black);
    actColor = new QAction(pix, tr("Text &color..."), this);
    connect(actColor, SIGNAL(triggered()), this, SLOT(textColor()));

    //setEnabled(false);
}

NCRDTextPropertyManager::~ NCRDTextPropertyManager()
{
}

void NCRDTextPropertyManager::textAlignHorizontal(QAction *a)
{
    Qt::Alignment vAlign = NCReportUtils::verticalAlignment(m_alignment);

    if (a == actAlignLeft)
        m_alignment = Qt::AlignLeft | vAlign;
    else if (a == actAlignHCenter)
        m_alignment = Qt::AlignHCenter | vAlign;
    else if (a == actAlignRight)
        m_alignment = Qt::AlignRight | vAlign;
    else if (a == actAlignJustify)
        m_alignment = Qt::AlignJustify | vAlign;

    applyChanges(Alignment);
}

void NCRDTextPropertyManager::textAlignVertical(QAction *action )
{
    Qt::Alignment hAlign = NCReportUtils::horizontalAlignment(m_alignment);

    if (action == actAlignTop)
        m_alignment = hAlign | Qt::AlignTop;
    else if (action == actAlignVCenter)
        m_alignment = hAlign | Qt::AlignVCenter;
    else if (action == actAlignBottom)
        m_alignment = hAlign | Qt::AlignBottom;

    applyChanges(Alignment);
}

void NCRDTextPropertyManager::textBold()
{
    m_font.setBold( actBold->isChecked() );
    applyChanges(FontBold);
}

void NCRDTextPropertyManager::textItalic()
{
    m_font.setItalic( actItalic->isChecked() );
    applyChanges(FontItalic);
}

void NCRDTextPropertyManager::textUnderline()
{
    m_font.setUnderline( actUnderline->isChecked() );
    applyChanges(FontUnderline);
}

void NCRDTextPropertyManager::textColor()
{
    QColor col = QColorDialog::getColor(m_color, m_mainwin);
    if (!col.isValid())
        return;

    colorChanged(col);
    applyChanges(Color);
}

void NCRDTextPropertyManager::textFamily(const QString & f)
{
    m_font.setFamily(f);
    //textSize(comboSize->currentText());
    applyChanges(FontFamily);
}

void NCRDTextPropertyManager::textSize(const QString & p)
{
    m_font.setPointSize(p.toInt());
    //NCReportScale scale;
    //m_font.setPixelSize(scale.fontPointSizeToPixel(p.toInt()));
    applyChanges(FontSize);
}

void NCRDTextPropertyManager::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actBold->setChecked(f.bold());
    actItalic->setChecked(f.italic());
    actUnderline->setChecked(f.underline());
    m_font = f;
}

void NCRDTextPropertyManager::colorChanged(const QColor &c)
{
    QPixmap pix(14, 14);
    pix.fill(c);
    actColor->setIcon(pix);
    m_color = c;
}

void NCRDTextPropertyManager::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
        actAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        actAlignHCenter->setChecked(true);
    } else if (a & Qt::AlignRight) {
        actAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        actAlignJustify->setChecked(true);
    }

    if (a & Qt::AlignTop) {
        actAlignTop->setChecked(true);
    } else if (a & Qt::AlignVCenter) {
        actAlignVCenter->setChecked(true);
    } else if (a & Qt::AlignBottom) {
        actAlignBottom->setChecked(true);
    }

    m_alignment = a;
}

void NCRDTextPropertyManager::buildMenu( QMenu *m )
{
    m_menu = m;
    if ( m_menu ) {
        m_menu->addAction(actBold);
        m_menu->addAction(actItalic);
        m_menu->addAction(actUnderline);
        m_menu->addSeparator();
        m_menu->addAction(actAlignLeft);
        m_menu->addAction(actAlignHCenter);
        m_menu->addAction(actAlignRight);
        m_menu->addAction(actAlignJustify);
        m_menu->addSeparator();
        m_menu->addAction(actAlignTop);
        m_menu->addAction(actAlignVCenter);
        m_menu->addAction(actAlignBottom);
        m_menu->addSeparator();
        m_menu->addAction(actColor);
    }
}

void NCRDTextPropertyManager::buildToolBar(QToolBar *tb)
{
    m_toolbar = tb;
    if ( m_toolbar ) {
        comboFont = new QFontComboBox(m_toolbar);
        //comboFont->setEnabled(false);
        m_toolbar->addWidget(comboFont);
        connect(comboFont, SIGNAL(activated(const QString &)), this, SLOT(textFamily(const QString &)));

        comboSize = new QComboBox(m_toolbar);
        comboSize->setObjectName("comboSize");
        m_toolbar->addWidget(comboSize);
        comboSize->setEditable(true);
        //comboSize->setEnabled(false);
        QFontDatabase db;
        foreach(int size, db.standardSizes())
            comboSize->addItem(QString::number(size));

        connect(comboSize, SIGNAL(activated(const QString &)), this, SLOT(textSize(const QString &)));
        comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));

        m_toolbar->addSeparator();
        m_toolbar->addAction(actBold);
        m_toolbar->addAction(actItalic);
        m_toolbar->addAction(actUnderline);
        m_toolbar->addSeparator();
        m_toolbar->addActions(actGroupHAlign->actions());
        m_toolbar->addActions(actGroupVAlign->actions());
        m_toolbar->addSeparator();
        m_toolbar->addAction( actColor );
    }
}

void NCRDTextPropertyManager::applyChangesToItem(NCReportItem *item, int property)
{
    if ( !item )
        return;

    switch (item->objectType()) {
        case NCReportItem::Label:
        case NCReportItem::Field:
        {
            NCReportLabelItem *label = (NCReportLabelItem*)(item);
            //label->setFont(m_font);
            if (property == FontFamily || property==All)
                label->fontRef().setFamily(m_font.family());

            if (property == FontSize || property==All)
                label->fontRef().setPointSize(m_font.pointSize());

            if (property == FontBold || property==All)
                label->fontRef().setBold(m_font.bold());

            if (property == FontItalic || property==All)
                label->fontRef().setItalic(m_font.italic());

            if (property==FontUnderline || property==All)
                label->fontRef().setUnderline(m_font.underline());

            if (property==Alignment || property==All)
                label->setAlignment(m_alignment);

            if (property==Color || property==All)
                label->setForeColor(m_color);

            label->adjustSize();
            break;
        }
        case NCReportItem::Text:
        {
            NCReportTextItem *text = (NCReportTextItem*)(item);

            text->setFont(m_font);
            text->setForeColor(m_color);
            break;
        }
        default:
            return;
    }
    item->update();

    if ( m_doc )
        m_doc->setDocumentModified();
}

void NCRDTextPropertyManager::applyChangesToManager(NCReportItem *item)
{
    if ( !item )
        return;

    switch (item->objectType()) {
        case NCReportItem::Label:
        case NCReportItem::Field:
        {
            NCReportLabelItem *label = (NCReportLabelItem*)(item);
            fontChanged(label->font());
            colorChanged(label->foreColor());
            alignmentChanged(label->alignment());
            break;
        }
        case NCReportItem::Text:
        {
            NCReportTextItem *text = (NCReportTextItem*)(item);
            fontChanged(text->font());
            colorChanged(text->foreColor());
            break;
        }
        default:
            return;
    }
    setEnabled( true );
}

void NCRDTextPropertyManager::setEnabled(bool set)
{
    //NCRDPropertyManager::setEnabled(set);

    actColor->setEnabled(set);
    actUnderline->setEnabled(set);
    actBold->setEnabled(set);
    actItalic->setEnabled(set);

    actAlignHCenter->setEnabled(set);
    actAlignLeft->setEnabled(set);
    actAlignRight->setEnabled(set);
    actAlignJustify->setEnabled(set);

    actAlignTop->setEnabled(set);
    actAlignVCenter->setEnabled(set);
    actAlignBottom->setEnabled(set);

    if ( comboFont )
        comboFont->setEnabled(set);
    if ( comboSize )
        comboSize->setEnabled(set);
}

void NCRDTextPropertyManager::reset()
{
    if ( m_doc ) {
        QFont font( m_doc->reportDef()->option().defaultFont() );
        fontChanged( font );
    }
    alignmentChanged( Qt::AlignLeft );
}

void NCRDTextPropertyManager::setAlignment(Qt::Alignment a)
{
    alignmentChanged(a);
}
