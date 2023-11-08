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
#include "ncreportsectiondirector.h"
#include "ncreportdirector.h"
#include "ncreportsection.h"
#include "ncreportdef.h"
#include "ncreportevaluator.h"

#include "ncreportitem.h"
#include "ncreportrectitem.h"
#include "ncreporttextitem.h"
#include "ncreportoutput.h"

#include <QTextDocument>
#include <QListIterator>
#include <QPainter>

NCReportSectionDirector::NCReportSectionDirector(NCReportDirector * director )
    : rd( director ),
      mSection( 0 ),
      mTotalZonesHeight(0),
      zoneMode(rd->m_reportDef->option().zonesAreBreakable() ? Breakable : Normal),
      m_pageBreakStop(false)
{
}

NCReportSectionDirector::~NCReportSectionDirector()
{
}

bool NCReportSectionDirector::renderSection(NCReportSection *section)
{
    //reset();
    mSection = section;

    if ( !section )
        return true;

    if ( !section->isShown() )	// mSection is hidden
        return true;

#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("--------- Section %s (%i) ---------", qPrintable(mSection->caption()), mSection->type());
#endif

    //rd->report->output()->setCurrentSection(section);

    //AVOID DUPLICATIONS
    if ( rd->m_reportDef->option().reportType() == NCReport::Report &&
            (mSection->type() != NCReportSection::Detail && mSection->type() != NCReportSection::PageHeader) &&
            mSection == rd->m_lastRenderedSection )
        return true;

    // SECTION PRINT WHEN
    if ( !mSection->printWhenExp().isEmpty() ) {
        if (!rd->evaluator()->evaluatePrintWhen(mSection->printWhenExp()))
            return true;
    }
    bool ok=true;

    aloneSectionProtection();

    rd->m_report->output()->sectionStarts(section, rd->evaluator());

    if (rd->m_report->output()->isDocumentTemplateBased()) {
        // section is one expression
        ok = rd->m_report->output()->renderSection(section, rd->evaluator());
        rd->m_report->output()->sectionEnds(section, rd->evaluator());
        return ok;
    }

    if (rd->m_report->output()->generatingMode()==NCReportOutput::DirectPainter) {

        if ( mSection->isAutoHeight() ) {
            // AUTOMATIC HEIGHT: ZONE HANDLING MODE
            if ( !loadZones() )
                return false;
            if ( !renderZones() )
                return false;
        } else {
            // FIXED SECTION, NO ZONES
            if ( !checkSpace( mSection->metricHeight() ) )
                return false;
            if ( !renderItems() )
                return false;
        }

    } else if (rd->m_report->output()->generatingMode()==NCReportOutput::DocumentFile) {
        renderItems();
    }

    addSectionInfo(mTotalZonesHeight);

    rd->m_lastRenderedSection = mSection;
    rd->m_report->output()->sectionEnds(section, rd->evaluator());

    return ok;
}


bool NCReportSectionDirector::renderItems()
{
    // RENDER SECTION'S ITEMS
    qreal mSectionHeight = mSection->metricHeight();
    qreal myPaintY = rd->paintPosMM().y();

    bool hasItems = false;
    paintAlternateRowBackground(rd->paintPosMM());
    for (int i = 0; i < mSection->reportItems().size(); ++i) {
        NCReportItem *item = mSection->reportItems().at(i);
        item->updateValue(rd->m_report->output(), rd->evaluator());

        if ( !rd->evaluator()->evaluatePrintWhen( item ) )  // PRINTWHEN
            continue;

        hasItems = true;

        item->setSectionMetricSize( staticSectionMetricSize() );	// tell item the section's size
        item->setZoneMetricSize( QSizeF() );
        item->adjustSize(rd->m_report->output());
        item->setDirector( rd );
        paintItem( item );
    }

    if ( hasItems || !mSection->hideIfBlank() ) {
        // Check to see if something (like the crosstab) might have already directly changed where we are
        if (myPaintY == rd->paintPosMM().y()) {
            // nope. we're still in the same spot. Go ahead and add the height of the section.
            rd->translatePaintPosY(mSectionHeight);
        }
        // add spacing.
        rd->translatePaintPosY(rd->m_reportDef->option().sectionSpacing());
    }
    return true;
}



bool NCReportSectionDirector::checkSpace( const qreal spaceMM )
{
    // check the space is needed in a page
    switch ( mSection->checkRole() ) {
        case NCReportSection::SectionOnly:
            rd->setFlag( NCReportDirector::FlagNoSpaceInPage, (spaceMM > rd->spaceToBottom() ) );
            break;
        case NCReportSection::SectionPlusDetail:
            // we want enough space of the section plus a detail! (suitable for group headers)
            rd->setFlag( NCReportDirector::FlagNoSpaceInPage, (spaceMM + rd->currentDetail()->metricHeight() > rd->spaceToBottom() ) );
            break;
        case NCReportSection::SectionPlusOrphanedTolerance:
            // we want enough space of the section plus a fixed tolerance
            rd->setFlag( NCReportDirector::FlagNoSpaceInPage, (spaceMM + mSection->orphanedTolaranceMM() > rd->spaceToBottom() ) );
            break;
    }
    if ( rd->flag(NCReportDirector::FlagNoSpaceInPage) ) {
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("--------- NO SPACE! (Section %s (%i))",qPrintable(mSection->tagname()), mSection->type());
#endif
        return false;
    }
    return true;
}


bool NCReportSectionDirector::loadZones()
{
    zones.clear();
    QListIterator<NCReportItem*> i(mSection->reportItems());
    while ( i.hasNext() ) {
        NCReportItem *item = i.next(); //mSection->reportItems().at(i);
        item->updateValue(rd->m_report->output(), rd->evaluator());
        rd->evaluator()->evaluatePrintWhen( item ); // PRINTWHEN

        if ( item->printWhenResult() ) {
            item->adjustSize(rd->m_report->output());
            //-------------------- DEBUG -----------------------------//
            //QString itemValue = item->value().toString();
            //if (item->objectType() == NCReportItem::Text)
            //	itemValue = ((NCReportTextItem*)item)->document()->toPlainText();

            //if (itemValue.length()>50)
            //	int a=0;
            //-------------------- END DEBUG -------------------------//
            qreal itemHeightMM = item->realMetricSize().height(); // item height

            if ( zones.contains(item->zoneID()) ) {
                NCReportZone &zone = zones[item->zoneID()]; //reference
                zone.visible = true;
                zone.startY = qMin( zone.startY, item->metricPos().y() );
                zone.endY = qMax( zone.endY, item->metricPos().y() + itemHeightMM + rd->m_reportDef->option().zoneSpacing() );
                zone.items << item;
            } else {
                NCReportZone zone;
                zone.id = item->zoneID();
                zone.visible = true;
                zone.startY = item->metricPos().y();
                zone.endY = item->metricPos().y() + itemHeightMM + rd->m_reportDef->option().zoneSpacing();
                zone.items << item;
                zones[item->zoneID()] = zone;
            }

        }
    }

    //--------- 2012-10-03 No Zones: Empty content but valid:
    if (zones.count()==0)
        return true;

    // CHECK IF VISIBLE ZONES HAVE ENOUGH SPACE
    mTotalZonesHeight =0;	// all zones height
    int counter=0;
    bool sectionIsPrintable = false;

    if (zoneMode == Breakable) {
        //----------------------------------------
        // New Code
        //----------------------------------------
        int counter=0;
        qreal m_totalHeightMM = 0;
        QMap<int,NCReportZone>::iterator iterator = zones.begin();
        while (iterator != zones.end()) {
            NCReportZone& zone = iterator.value();
            bool isLastZone = (counter == zones.count()-1);

            m_totalHeightMM += zone.heightMM();
            zone.totalHeightMM = m_totalHeightMM;

            if ( checkZoneSpace(zone) )
                sectionIsPrintable = true;
            else {
                if ( sectionIsPrintable && isLastZone && zoneMode == Normal )
                    sectionIsPrintable = false;
            }

            ++counter;
            ++iterator;
        }

        return sectionIsPrintable;

    } else {
        //----------------------------------------
        // Old Code (Normal)
        //----------------------------------------
        QMap<int,NCReportZone>::const_iterator iterator = zones.constBegin();
        while (iterator != zones.constEnd()) {
            const NCReportZone& zone = iterator.value();

#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Iterator:%i Zone:%i height:%f startY:%f endY:%f visible:%i",iterator.key(), zone.id, zone.heightMM(), zone.startY, zone.endY, zone.visible );
#endif
            // set items relative (in-zone) coordinates)
            for (int i = 0; i < zone.items.size(); ++i) {
                NCReportItem *item = (NCReportItem*)zone.items.at(i);
                item->setMetricOffset( QPointF(0, zone.startY) );
            }

            // CALCULATE HEIGHT
            if ( zone.visible )
                mTotalZonesHeight += zone.heightMM() + rd->m_reportDef->option().zoneSpacing() ; // add visible zone height

            ++counter;
            ++iterator;
        }

        if ( !checkSpace( mTotalZonesHeight ) )
            return false; // NO SPACE ENOUGH

        return true;

    }

    return false;
}


/*!
Checks the zone size (height) and decides if zone has enough space to print or not.
It handles the breakable items as well.
Returns decision of the whole section is ready to be printed or not.
  */
bool NCReportSectionDirector::checkZoneSpace( NCReportZone & zone )
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Zone:%i height:%f startY:%f endY:%f visible:%i", zone.id, zone.heightMM(), zone.startY, zone.endY, zone.visible );
#endif
    bool sectionIsPrintable = false;
    // set items relative (in-zone) coordinates)
    bool hasBreakableItem = false;
    for (int i = 0; i < zone.items.size(); ++i) {
        NCReportItem *item = (NCReportItem*)zone.items.at(i);
        item->setMetricOffset( QPointF(0, zone.startY) );
        item->setZoneMetricSize( zoneMetricSize(zone) );
        if (item->isAbleToBreak())
            hasBreakableItem = true;
    }

    // CALCULATE HEIGHT
    if ( zone.visible )
        increaseTotalZoneHeight(zone,1);

    // check if has enough space
    if ( checkSpace( mTotalZonesHeight) ) {
        sectionIsPrintable = true;	// section is allowed to print
        zone.hasEnoughSpace = true;
    } else {
        // all zones haven't got enough place , let's check this zone only
        //zone.hasEnoughSpace = checkSpace( zone.heightMM() );
        zone.hasEnoughSpace = checkSpace( zone.totalHeightMM );
        // ----- !!!!!!!!!!! BUG: Az elozo zonat, ami kifert bele kell szamitani a rendelkezesre allo helybe
        if ( zone.hasEnoughSpace ) {
            sectionIsPrintable = true;	// section is allowed to print
        } else {
            if ( hasBreakableItem ) {
                // try to shrink the zone to available space
                sectionIsPrintable = true;	// section is allowed to print by default
                increaseTotalZoneHeight(zone,-1);

                for (int i = 0; i < zone.items.size(); ++i) {
                    NCReportItem *item = (NCReportItem*)zone.items.at(i);

                    if ( item->isAbleToBreak() ) {
                        qreal itemHeightMM = -1;

                        if (item->printWhenResult())
                            itemHeightMM = item->breakTo( rd->spaceToBottom() - mTotalZonesHeight, rd->m_report->output() );

                        zone.hasEnoughSpace = itemHeightMM>=0;

                        if (zone.hasEnoughSpace) {
                            zone.startY = qMin( zone.startY, item->metricPos().y() );
                            zone.endY = item->metricPos().y() + itemHeightMM;
                            increaseTotalZoneHeight(zone,1);
                        } else {
                            sectionIsPrintable = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    return sectionIsPrintable;
}

/*!
Depending on factor, increases or decreases mTotalZonesHeight variable with zone height + zone spacing
*/
void NCReportSectionDirector::increaseTotalZoneHeight(const NCReportSectionDirector::NCReportZone & zone, const short factor)
{
    mTotalZonesHeight += factor*(zone.heightMM() + rd->m_reportDef->option().zoneSpacing());
}

/*!
  Renders all zones
 */
bool NCReportSectionDirector::renderZones()
{
    if (zoneMode == Breakable)
        return renderZonesBreakable();
    else
        return renderZonesNormal();
}

bool NCReportSectionDirector::renderZonesNormal()
{
    QMap<int,NCReportZone>::iterator i = zones.begin();
    int counter=0;
    while (i != zones.end()) {
        NCReportZone& zone = i.value();

        if ( zone.visible ) {
            renderZoneItemsNormal( zone );
            rd->translatePaintPosY(zone.heightMM() + rd->m_reportDef->option().zoneSpacing()); // SET GLOBAL POSITION
        }

        ++i;
        ++counter;
    }

    return true;
}

bool NCReportSectionDirector::renderZonesBreakable()
{
    QMap<int,NCReportZone>::iterator i;
    for (i = zones.begin(); i != zones.end(); ++i) {
        NCReportZone& zone = i.value();

        qDebug("Zone: %f, %f, %i, %f, %i", zone.startY, zone.endY, zone.visible, zone.heightMM(), zone.id );

        if ( zone.visible ) {
            /*
            must not run checkZoneSpace, because it is already calculated, but
            after page break the object has enough space again, so needs to recheck the hasEnoughSpace variable.
            */
            if (isPageBreakStop())
                zone.hasEnoughSpace = true;

            if (zone.hasEnoughSpace) {
                renderZoneItemsBreakable( zone );
            } else {
                rd->pageBreak(false,true);	// zone level pagebreak
                increaseTotalZoneHeight(zone,-1);	// substract zone height
                renderZoneItemsBreakable(zone);
                setPageBreakStop(true);
            }
        }
    }
    setPageBreakStop(false);
    return true;
}


/*!
  Renders the member items of a zone
  */
bool NCReportSectionDirector::renderZoneItems( NCReportZone& zone )
{
    if (zoneMode==Breakable)
        return renderZoneItemsBreakable(zone);
    else
        return renderZoneItemsNormal(zone);
}


bool NCReportSectionDirector::renderZoneItemsNormal( NCReportZone& zone )
{
    paintAlternateRowBackground(rd->paintPosMM(), zone );
    for (int i = 0; i < zone.items.size(); ++i) {
        NCReportItem *item = (NCReportItem*)zone.items.at(i);
        if (item->objectType()==NCReportItem::CrossTab) {
            rd->fatalError(QObject::tr("Cross-Tab item is unsupported in zones."));
            return false;
        }
        item->setSectionMetricSize( autoSectionMetricSize() );	// tell item the section's size
        item->setZoneMetricSize( zoneMetricSizeWithSpacing(zone) );
        item->enableOffsetPosition();
        paintItem(item);
        item->disableOffsetPosition();
    }
    return true;
}


bool NCReportSectionDirector::renderZoneItemsBreakable( NCReportZone& zone )
{
    bool partialPrint = false;
    paintAlternateRowBackground(rd->paintPosMM(), zone);
    for (int i = 0; i < zone.items.size(); ++i) {
        NCReportItem *item = (NCReportItem*)zone.items.at(i);
        //item->setSectionMetricSize( QSizeF( rd->sectionWidthMM( mSection ), mTotalZonesHeight - rd->rdef->option().zoneSpacing() ) );	// tell item the section's size
        //item->setZoneMetricSize( QSizeF( rd->sectionWidthMM( mSection ), zone.heightMM() - rd->rdef->option().zoneSpacing()*(zones.count()-1) ) );
        item->setSectionMetricSize( autoSectionMetricSize() );	// tell item the section's size
        item->setZoneMetricSize( zoneMetricSize(zone) );

        item->enableOffsetPosition();
        paintItem(item);
        item->disableOffsetPosition();
        if ( item->isBroken() )
            partialPrint = true;
    }

    rd->translatePaintPosY(zone.heightMM() + rd->m_reportDef->option().zoneSpacing()); // SET GLOBAL POSITION

    bool hasSlicedItem = false;
    NCReportSection *currentSection = mSection;
    //---------------------
    // Sliced item prints
    //---------------------
    while (partialPrint) {
        rd->m_lastRenderedSection = mSection;
        // we have to render the unprinted parts of sliced items
        rd->pageBreak(false,true);
        qreal greatestHeightMM = 0;
        for (int i = 0; i < zone.items.size(); ++i) {
            NCReportItem *item = (NCReportItem*)zone.items.at(i);
            if (item->isBroken()) {

                qreal heightMM = item->breakTo( rd->spaceToBottom(), rd->m_report->output() );
                //qreal currentYMM = rd->paintPosMM().y();

                if ( heightMM > 0 ) {

                    QPointF m_pos = item->metricPos();	// save original position
                    item->setMetricPos( QPointF(m_pos.x(), 0 ));	 //trimmed
                    paintItem(item);
                    item->setMetricPos(m_pos);

                    greatestHeightMM = qMax(heightMM,greatestHeightMM);
                    hasSlicedItem = item->isBroken();

                    //-----------------------------------
                    //PAINT AUTO HEIGHT RECT/LICE OBJECTS
                    //-----------------------------------
                    QListIterator<NCReportItem*> i(currentSection->reportItems());
                    while ( i.hasNext() ) {
                        NCReportItem *xItem = i.next();
                        if (
                                (xItem->objectType() == NCReportItem::Rectangle || xItem->objectType() == NCReportItem::Line)
                                && xItem->zoneID() == item->zoneID()
                             ) {
                            if (xItem->printWhenResult() && xItem->isAutoHeight() ) {
                                xItem->setBoundingRectOfLastPart(item->boundingRectOfLastPart());	// copy text item's last bounding rect
                                paintItem( xItem, rd->paintPosMM() );
                                xItem->setBoundingRectOfLastPart(QRectF()); //reset
                            }
                        }
                    }
                }
            }
        }
        rd->translatePaintPosY(greatestHeightMM + rd->m_reportDef->option().zoneSpacing()); // SET GLOBAL POSITION
        partialPrint = hasSlicedItem;

        if (!partialPrint) {
            // we have finished the slice. It's now completly printed.
            zone.hasEnoughSpace = true;		// to avoid to pagebreak again;
            setPageBreakStop(true);		// -||-
        }

    }

    return true;
}



void NCReportSectionDirector::setItemHidden(NCReportItem * item)
{
    hiddenItems << item;
}

bool NCReportSectionDirector::itemIsHidden(NCReportItem * item)
{
    return hiddenItems.contains(item);
}

void NCReportSectionDirector::adjustSize( NCReportItem *item )
{
    switch ( item->objectType() ) {
        case NCReportItem::Text: {
            //NCReportTextItem *textItem = (NCReportTextItem*)item;
            //textItem->adjustDocument( rd->report->output() );
            break;
        }
        default: break;
    }
}

/*!
  A flag that helps to avoid duplicated pagebreaks when printing multi zones
*/
void NCReportSectionDirector::setPageBreakStop(bool set)
{
    m_pageBreakStop = set;
}

bool NCReportSectionDirector::isPageBreakStop() const
{
    return m_pageBreakStop;
}

void NCReportSectionDirector::addSectionInfo( const qreal heightMM )
{
    if ( rd->currentPassState() == NCReportDirector::TestPass ) {
        NCReportDirector::SectionInfo si;
        si.index = rd->sectionCounter();
        si.section = mSection;
        si.linked = false;
        si.sectionHeightMM = heightMM;
        si.startedOnNewPage = false;
        si.lastSectionOfThePage = false;

        rd->sectionInfoList().append(si);
    }
    rd->sectionCounter()++;
}

void NCReportSectionDirector::aloneSectionProtection()
{
    if ( rd->currentPassState() == NCReportDirector::RealPass ) {

        int i = rd->sectionCounter();
        if (0 <= i && i < rd->sectionInfoList().size()) {
            NCReportDirector::SectionInfo si = rd->sectionInfoList().at(i);
            if (si.section == mSection && si.lastSectionOfThePage && si.section->type() == NCReportSection::GroupHeader && si.section->isNotAlone() ) {
                /*
                the group header is the last item of the page but should be not alone, do a page break.
                */
                rd->pageBreak(false,true);
            }
        }
    }
}

bool NCReportSectionDirector::simpleRenderSection(NCReportSection *section)
{
    mSection = section;

    if ( !mSection->isShown() )
        return true;

    if ( !mSection->printWhenExp().isEmpty() ) {
        if (!rd->evaluator()->evaluatePrintWhen(mSection->printWhenExp()))
            return true;
    }

    bool ok = true;

    rd->m_report->output()->sectionStarts(mSection, rd->evaluator());

    if (rd->m_report->output()->isDocumentTemplateBased()) {
        // section is one expression
        ok = rd->m_report->output()->renderSection(section, rd->evaluator());
    } else {
        paintAlternateRowBackground(rd->paintPosMM());
        int numFields=0;
        QListIterator<NCReportItem*> i(mSection->reportItems());
        while ( i.hasNext() ) {
            NCReportItem *item = i.next(); //mSection->reportItems().at(i);
            if (item->objectType() == NCReportItem::Field)
                numFields++;
            item->updateValue(rd->m_report->output(), rd->evaluator());
            if ( !rd->evaluator()->evaluatePrintWhen(item) )
                continue;

            item->setSectionMetricSize( staticSectionMetricSize() );	// tell item the section's size
            item->setZoneMetricSize( QSizeF() );
            item->adjustSize(rd->m_report->output());

            paintItem( item );
        }
    }

    rd->m_report->output()->sectionEnds(section, rd->evaluator());
    addSectionInfo(mSection->metricHeight());

    return ok;
}

void NCReportSectionDirector::reset()
{
//	mSection = 0;
//	mTotalZonesHeight = 0;
//	zoneMode = (rd->rdef->option().zonesAreBreakable() ? Breakable : Normal);
//	m_pageBreakStop = false;
//	zones.clear();
    //	hiddenItems.clear();
}

QSizeF NCReportSectionDirector::autoSectionMetricSize() const
{
    return QSizeF( rd->sectionWidthMM( mSection ), mTotalZonesHeight - rd->m_reportDef->option().zoneSpacing() );
}

QSizeF NCReportSectionDirector::staticSectionMetricSize() const
{
    return QSizeF( rd->sectionWidthMM( mSection ), mSection->metricHeight() );
}

QSizeF NCReportSectionDirector::zoneMetricSize(const NCReportZone &zone) const
{
    return QSizeF( rd->sectionWidthMM( mSection ), zone.heightMM() );
}

QSizeF NCReportSectionDirector::zoneMetricSizeWithSpacing(const NCReportSectionDirector::NCReportZone &zone) const
{
    return QSizeF( rd->sectionWidthMM( mSection ), zone.heightMM() - rd->m_reportDef->option().zoneSpacing()*(zones.count()-1) );
}

void NCReportSectionDirector::paintItem( NCReportItem *item )
{
    paintItem(item, rd->paintPosMM());
}

/*!
  Paints the current report item. Renders item only if the page is printable.
  @param item report item
  @param painterPosMM current painter position in millimeters
  */

void NCReportSectionDirector::paintItem( NCReportItem *item, const QPointF& painterPosMM )
{
    //------ PAINT ITEM ------//
    if ( rd->currentPageStatus() == NCReportDirector::Printable ) {

#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug( "Paint item: %s (%p) Painter: %p", qPrintable(item->tagname()), item, rd->m_report->output()->painter() );
#endif
        item->paint( rd->m_report->output(), painterPosMM );
        rd->m_report->output()->setFlagPrintStarted(true);
    } else {
        item->dummyPaint( rd->m_report->output(), painterPosMM );
    }
}

/*!
 * \brief NCReportSectionDirector::paintAlternateRowBackground
 * \param painterPosMM
 * PAINTS ALTERNATE BACKGROUND RECTANGLE OF A DETAIL
 */
void NCReportSectionDirector::paintAlternateRowBackground(const QPointF &painterPosMM, const NCReportZone &zone)
{
    if (mSection->type()!=NCReportSection::Detail)
        return;

    bool print=false;
    switch (mSection->alternateRowBackgroundMode()) {
    case NCReportSection::Disabled:
        return;
    case NCReportSection::Odd:
        print = (rd->currentRow()%2)==1;
        break;
    case NCReportSection::Even:
        print = (rd->currentRow()%2)==0;
        break;
    }

    qDebug("Odd/Even: %i",rd->currentRow()%2);

    if (!print)
        return;

    NCReportRectItem rectItem(rd->m_reportDef);
    rectItem.setAutoHeight( mSection->isAutoHeight() );
    //rectItem.setRect( QRectF(0,0,rd->m_reportDef->option().purePageSize().width(),mSection->metricHeight()));
    rectItem.setMetricX(0);
    rectItem.setMetricY(0);
    rectItem.setMetricWidth(rd->m_reportDef->option().purePageSize().width());
    rectItem.setMetricHeight(mSection->metricHeight());

    if (mSection->isAutoHeight()) {
        rectItem.setSectionMetricSize( autoSectionMetricSize() );
        rectItem.setZoneMetricSize(zoneMetricSize(zone));
    } else {
        rectItem.setSectionMetricSize( staticSectionMetricSize() );
    }

    rectItem.setFitRole( NCReportItem::FitHeightToZone );
    rectItem.setPen( Qt::NoPen );
    if (mSection->useDefaultAlternateRowColor())
        rectItem.setBrush( rd->m_report->alternateRowBackgroundColor() );
    else
        rectItem.setBrush( mSection->alternateRowBackgroundColor() );
    rectItem.paint(rd->m_report->output(), painterPosMM);
}


