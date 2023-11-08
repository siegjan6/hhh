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
#ifndef NCRDDETAILDIALOG_H
#define NCRDDETAILDIALOG_H

#include <QDialog>
#include <QStandardItem>
#include <QList>
#include <QItemDelegate>

#include "ui_detaildialog.h"

QT_BEGIN_NAMESPACE
class QStandardItemModel;
class QDataWidgetMapper;
class QStringListModel;
QT_END_NAMESPACE

class NCRDDocument;
class NCReportSection;
class NCReportGroup;

/*!
Report Detail properties dialog class
*/

/*!

\section 功能

报表细节属性对话框管理类
【继承自Ui::NCRDDetailDialogUI，用来管理细节中相关属性配置】。

*/
class NCRDDetailDialog : public QDialog, private Ui::NCRDDetailDialogUI
{
  Q_OBJECT

public:
    NCRDDetailDialog( NCRDDocument *doc, NCReportSection *default_detail=0, QWidget* parent = 0 );
    ~NCRDDetailDialog();

    enum ModelColumns {
        Id=0,
        Height,
        DataSource,
        PageBreakCondition,
        StartOnNewPage,
        Pointer,
        DataSource2,
        AlternateRowMode,
        AlternateRowIsDefaultColor,
        AlternateRowColor,
        ColumnCount
    };

    QComboBox *comboDS();
    QComboBox *comboDS2();
    QComboBox *comboAltMode();

    void applyChanges();
    void discardChanges();
    void setChanged(bool);

    QDataWidgetMapper* mapper() const { return m_mapper; }

public slots:
    void accept();
    void apply(QAbstractButton*);

protected:

protected slots:
    void doGrouping();
    void add();
    void remove();
    void up();
    void down();

    void slotItemChanged( const QModelIndex & current, const QModelIndex & previous );
    void onChanged();

private:
    QStandardItemModel* m_model;
//    QStringListModel *m_modelDataSource;
//    QStringListModel *m_modelDataSource2;
    QDataWidgetMapper *m_mapper;
    NCRDDocument *m_document;
    bool m_reorderRequired;		// if detail order change
    bool m_changed;

    QList<NCReportSection*> m_removedDetails;

private:
    void setupModel();
    void appendModel( NCReportSection*, int row, bool inew );
    void setCurrentDetail( NCReportSection* detail );
    void undoChanges();
    void removeDetail(NCReportSection*);
    bool check();

    void debugModelValues(int row, const QString& label );
};


/*!
Custom model item for storing NCReportSection object pointer
*/
class NCRDStandardPointerItem : public QStandardItem
{
public:
    NCRDStandardPointerItem( NCReportSection *se, bool ins )
        : sect(se), insertedasnew(ins)
    {
    }

    ~NCRDStandardPointerItem() {}

    void setSection( NCReportSection* se )
    {
        sect = se;
    }

    void setGroup( NCReportGroup* gr )
    {
        grp = gr;
    }

    NCReportGroup *group()
    {
        return grp;
    }

    NCReportSection *section()
    {
        return sect;
    }

    bool insertedAsNew()
    {
        return insertedasnew;
    }
    void setInsertedAsNew(bool set)
    {
        insertedasnew = set;
    }

private:
    NCReportSection *sect;
    NCReportGroup *grp;
    bool insertedasnew;
};

/*!
Item delegate class for providing display and editing facilities for detail's data items from a model.
*/
class NCRDDetailItemDelegate : public QItemDelegate
{
public:
    NCRDDetailItemDelegate( NCRDDetailDialog *parent );
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
protected:
    //void drawDisplay( QPainter *painter, const QStyleOptionViewItem &option, const QRect & rect, const QString & text ) const;
private:
    NCRDDetailDialog *dialog;
};


#endif

