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
#ifndef NCRDVARIABLEDIALOG_H
#define NCRDVARIABLEDIALOG_H

#include <QDialog>
#include <QList>
#include <QStandardItem>
#include <QItemDelegate>
#include "ui_variabledialog.h"

class NCRDDocument;
class NCReportVariable;

QT_BEGIN_NAMESPACE
class QStandardItemModel;
class QDataWidgetMapper;
class QComboBox;
QT_END_NAMESPACE

/*!
Dialog class for setting/editing variable's properties
*/

/*!

section 功能

【变量编辑和设置管理类】

*/
class NCRDVariableDialog : public QDialog, private Ui::NCRDVariableDialogUI
{
  Q_OBJECT

public:
  NCRDVariableDialog( NCRDDocument *doc, QWidget* parent = 0 );
  ~NCRDVariableDialog();
  /*$PUBLIC_FUNCTIONS$*/
    void applyChanges();
    void discardChanges();

    void setCurrentVariable( const QString& varID );
    QDataWidgetMapper* mapper() const { return m_mapper; }


public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
    void add();
    void remove();
    void slotItemChanged( const QModelIndex & current, const QModelIndex & previous );
private:
    QStandardItemModel* m_model;
    QDataWidgetMapper *m_mapper;
    NCRDDocument *document;
    QList<NCReportVariable*> removeVars;

    void setupModel();
    void appendModel( NCReportVariable*, int row, bool flagNew );
    void updateEnable();

};


class NCRDVariableItem : public QStandardItem
{
public:
    NCRDVariableItem( NCReportVariable *var, bool flag )
        : m_var(var), flagnew( flag )
    {
    }

    ~NCRDVariableItem()
    {
    }

    bool flagNew()
    {
        return flagnew;
    }
    NCReportVariable *variable()
    {
        return m_var;
    }
private:
    NCReportVariable *m_var;
    bool flagnew;
};


class NCRDVariableItemDelegate : public QItemDelegate
{
public:
    NCRDVariableItemDelegate( NCRDVariableDialog *parent, bool intable );
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setComboBoxes( QComboBox* combo1, QComboBox* combo2 );
protected:
    //void drawDisplay( QPainter *painter, const QStyleOptionViewItem &option, const QRect & rect, const QString & text ) const;
private:
    NCRDVariableDialog *dialog;
    QComboBox *comboboxCol2, *comboboxCol3;
    bool useInTableView;
};


#endif

