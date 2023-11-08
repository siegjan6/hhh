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
#ifndef NCRDDOCUMENT_H
#define NCRDDOCUMENT_H

#include "ncreportsection.h"
#include "ncreportsource.h"
#include "ncreportitem.h"
#include <QScrollArea>
#include <QList>
//#include <QHash>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QSplitter;
class QUndoStack;
class QMdiSubWindow;
class QActionGroup;
class QMenu;
QT_END_NAMESPACE

class NCRDMainWindow;
class NCRDRuler;
class NCRDDesignSection;
class NCRDSectionScene;
class NCRDDef;
class NCReportDataSource;
class NCReportGraphItem;
class NCRDDataSourceTree;

/*!
Represents a report document in designer
*/

/*!

\section 功能

报表文件管理类
【管理报表XML文件】。

*/
class NCRDDocument : public QScrollArea
{
    Q_OBJECT
public:
    NCRDDocument( NCRDMainWindow *parent );
    ~NCRDDocument();

    enum ZoomMode { ZoomIn, ZoomOut, ZoomOne, ZoomFull, ZoomFit };

    void newFile(const QString& filename);
    bool load(const NCReportSource & );
    bool save();
    bool saveAs( const QString& fname );
    bool StudiosaveAs( const QString& fname );
    bool save(const NCReportSource & );
    QString saveToString() const;
    QString userFriendlyCurrentFile() const;
    QString fileName() const;
    const NCReportSource& reportSource() { return m_source; }
    bool isModified();
    void testFunction();
    QUndoStack *undoStack() const;
    void setToolMode();
    void setCurrentDesignSection( NCRDDesignSection* );
    NCRDDesignSection* currentDesignSection();
    void setCurrentScene( NCRDSectionScene* );
    NCRDSectionScene* currentScene();
    NCRDMainWindow* mainWindow() const;

    void setSubWindow( QMdiSubWindow* );

    void updateGrid();
    void updatePageSettings();
    void updateSectionsByReportDef();
    void updateManagers();
    void updateLanguageMenu( QMenu* menu, QActionGroup* actionGroup, QWidget* parent );

    NCRDDef* reportDef();
    void removeSection( NCReportSection* );
    void removeGroupSections( NCReportSection* );

    void sectionPropDialog( NCReportSection* =0 );

    qreal purePageWidth() const;
    void adjustContent();
    void initPropertyEditor();
    NCReportItem* firstSelectedItem();
    void setDefaultFont( const QFont& );

    void bringToFront();
    void moveToBack();
    void alignLeft();
    void alignRight();
    void alignHCenter();
    void alignVCenter();
    void alignTop();
    void alignBottom();
    void alignSameWidth();
    void alignSameHeight();
    void copy();
    void cut();
    void paste();
    void keyboardAction( QKeyEvent* );
    void zoom( ZoomMode );
    qreal zoomScale() const;

    void loadDataSourceColumnNames( QStringList& keywords, NCReportDataSource* =0 ) const;
    QStringList dataSourceColumnNames( const QString& dataSourceID );
    QStringList allKeywords(NCReportItem::SourceType type);
    void setCurrentLanguage(const QString& langcode);
    QString currentLanguage();
    void lock_unlock( bool setlock );
    void setZoneValue( int value );
    void updateDataSourceTree(NCRDDataSourceTree *tree, bool updateDS, bool updateVar, bool updatePar, bool forceUpdate);
    QDialog *createGraphItemDialog(NCReportGraphItem *item);

    QString getViewPath();//获得数据视图觉得路径
    void setViewPath(const QString & viewPath);
public slots:
    void setDocumentModified();
    void sceneContentModified(const QList<QRectF>& );
    void setModified(bool);
    void slotSectionResized( const QSize & );
    void itemEdit( NCReportItem* );
    void actionManagerChanged(int property);

protected slots:
    void slotSectionClickFocus();
    void slotSectionFirstItemSelected(NCReportItem*);
    void slotHorizontalScroll(int);

protected:
    void closeEvent(QCloseEvent *);
    void scrollContentsBy( int dx, int dy );
    void resizeEvent(QResizeEvent *event);
    void focusInEvent(QFocusEvent *event);

private:
    bool m_isUntitled;
    bool m_modified;
    int m_counterOfDetails;
    qreal m_zoomScale;
    NCRDMainWindow *m_mainWindow;
    NCRDDef* m_rdef;
    QSplitter *m_splittter;
    NCRDDesignSection *m_currentDesignSection;
    NCRDRuler *m_rulertop;
    QWidget *m_sectionContainer;
    QVBoxLayout *m_laySC;
    NCRDSectionScene *m_currentScene;
    QUndoStack *m_undostack;
    NCReportItem *m_firstSelectedItem;
    QMdiSubWindow *m_subWindow;

    enum ItemOperation { BringFront, MoveBack, AlignLeft, AlignRight,
        AlignHCenter, AlignVCenter, AlignTop, AlignBottom, AlignSameWidth, AlignSameHeight };

    NCReportSource m_source;
    QList<NCRDDesignSection*> m_sections;
    QString m_ViewPath; //数据视图绝对路径
private:
    bool maybeSave();
    void setReportSource(const NCReportSource &rs );
    QString strippedName(const QString &fullFileName);
    void initReportDef();

    //void addSection( NCReportSection::SectionType );
    /*!Adds a new section to the document by NCReportSection parameter*/
    void addSection( NCReportSection *sect );
    NCRDDesignSection* designSection( NCReportSection* );
    NCRDDesignSection* designSectionByLayoutIndex( int idx );

    void itemAlignmentOperation( const ItemOperation );
    int updateSection( NCReportSection* );
    void updateSectionsLayoutOrder( QList<int>& layoutIndexList );
    void updateLanguage();

    //obsolete
    /*! Gets the layout index for the specified section*/
    int sectionIndex( NCReportSection* );

    //void updateGroupSections( NCReportSection *detail );
    //void updateSectionOrder();
};

#endif
