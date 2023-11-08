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
#ifndef NCRDDESIGNSECTION_H
#define NCRDDESIGNSECTION_H

#include <QWidget>

class NCRDMainWindow;
class NCRDDocument;
class NCRDRuler;
class NCRDResizerBar;
class NCRDSectionTitle;
class NCRDDesignArea;
class NCRDSectionScene;

/*!
This widget represents a section inside of NCRDDocument.\n
Each section contains: Title (at the top), Left side ruler (at left), resizer bar (at bottom).\n
NCRDDesignSection may have header and footer section reference.
*/

/*!

 * \section 功能

这个类是报表设计器NCRDDocument类中的一部分，提供细节和分组中的相关功能，包含以下三个功能模块
１：包含标题，在这一section的左侧顶部
２：包含左侧的刻度标尺，用于记录每一细节的高度
３：包含底部大小调整功能模块
注：NCRDDesignSection也可包含组头和组脚的相关细节

*/
class NCRDDesignSection : public QWidget
{
    Q_OBJECT
public:
    NCRDDesignSection( NCRDMainWindow * mwin, NCRDDocument *doc, QWidget * parent );
    virtual ~NCRDDesignSection();

    void setSectionScene( NCRDSectionScene * );
    NCRDSectionScene* sectionScene() const;
    NCRDDocument* reportDocument() const;

    void setTitle(const QString & s, const QIcon& icon );
    void setTitle(const QString & s );
    void setWidthMM( qreal );
    void setHeightMM( qreal );

    NCRDDesignArea *designArea() const ;
    NCRDRuler* leftRuler() const { return ruler; }

    void setHeaderSection( NCRDDesignSection* );
    void setFooterSection( NCRDDesignSection* );
    NCRDDesignSection *headerSection();
    NCRDDesignSection *footerSection();
    NCRDSectionTitle *title();
    void updateSceneRect();

signals:
    void sectionResized( const QSize& );
protected slots:
    void resizeBarDragged(int d);

protected:
    NCRDSectionTitle *m_title;
    NCRDMainWindow* mainwindow;
    /*!The footer pair (pointer) of this section*/
    NCRDDesignSection *m_footerSection;
    NCRDDesignSection *m_headerSection;
    NCRDSectionScene *m_scene;
    NCRDRuler* ruler;
    NCRDDesignArea *designarea;
    NCRDResizerBar *rbar;
    NCRDDocument *m_doc;

    void init();
    void showEvent( QShowEvent * event );

    //ReportGridOptions * grid;
};


#endif
