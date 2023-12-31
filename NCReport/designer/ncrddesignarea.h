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
#ifndef NCRDDESIGNAREA_H
#define NCRDDESIGNAREA_H

#include <QGraphicsView>

class NCRDDesignArea;
class NCRDSectionScene;
class NCRDMainWindow;
class NCRDDocument;
class NCReportItem;
class NCRDDesignSection;

/*
struct NCRDDesignAreaData
{
    NCRDDesignAreaData();

};
*/

/*!
This class represents the design area's view inside of NCRDDesignSection report section.\
*/

/*!

\section 功能

报表设计器控件设计区域管理类

*/
class NCRDDesignArea : public QGraphicsView
{
	Q_OBJECT
public:
	NCRDDesignArea( NCRDMainWindow * mwin, NCRDDocument *doc, NCRDDesignSection *parent );
	~NCRDDesignArea();

    void setReportSection(NCRDSectionScene* se);
    NCRDSectionScene* sectionScene() const;
    NCRDDocument* reportDocument() const;
    NCRDMainWindow* mainWindow() const;
	
    int zoomLevel() const;
    qreal zoomScale() const;

public slots:
    void setZoomLevel(int zoomLevel);
    void zoomIn();
    void zoomOut();
    void zoomFull();
    void zoomOne();
    void zoomFit();
 
signals:
    void zoomLevelChanged(int zl);
    void zoomScaleChanged(double zs);
	void sectionClickFocus();
	void mouseMoved( const QPoint& );

protected:
	void contextMenuEvent( QContextMenuEvent * e );
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void resizeEvent(QResizeEvent* re);
	void wheelEvent(QWheelEvent * e);
	void drawBackground(QPainter * painter, const QRectF & rect);
	void focusInEvent( QFocusEvent * );
	// COMMANDS FOR UNDO/REDO purposes
/*	void insertItemCommand( const QPoint& pos);
	void removeItemCommand( NCReportItem*, bool removeSelected = true );
	void moveItemCommand( NCReportItem*, bool moveSelected = true );
	void resizeItemCommand( NCReportItem* );*/
private:
	//NCRDDesignAreaData* d;

	NCRDSectionScene *m_scene;
	int m_zoomLevel;
	qreal m_zoomValue;
	NCRDDesignArea* m_designArea;
	NCRDDesignSection* m_designSection;
	NCRDMainWindow *m_mainwin;
	NCRDDocument *m_doc;

	qreal calculateScaleFor(int zoomLevel) const;
	int calculateZoomLevelFor(qreal scale) const;

};

#endif
