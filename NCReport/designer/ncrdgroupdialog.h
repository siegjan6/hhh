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
#ifndef NCRDGROUPDIALOG_H
#define NCRDGROUPDIALOG_H

#include <QDialog>
#include "ui_groupdialog.h"
QT_BEGIN_NAMESPACE
class QStandardItemModel;
class QDataWidgetMapper;
QT_END_NAMESPACE
class NCReportSection;
class NCReportGroup;
class NCRDDocument;

/*!
Group/Band properties dialog class
*/

/*!

\section 功能

报表分组属性对话框类
【在细节中添加分组选项按钮时弹出次分组配置界面对话框，用与配置某一细节的相关分组属性】。

*/
class NCRDGroupDialog : public QDialog, private Ui::NCRDGroupDialogUI
{
  Q_OBJECT

public:
  NCRDGroupDialog( NCRDDocument* doc, NCReportSection *detail, QWidget* parent = 0 );
  ~NCRDGroupDialog();
  /*$PUBLIC_FUNCTIONS$*/
    void applyChanges();
    void discardChanges();
    void setCurrentGroup(NCReportGroup*);
    QDataWidgetMapper* mapper() const { return m_mapper; }

public slots:
  /*$PUBLIC_SLOTS$*/

protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
    void add();
    void remove();
    void up();
    void down();
    void slotItemChanged( const QModelIndex & current, const QModelIndex & previous );
    void slotVarListChanged(QListWidgetItem*);
    void openExpressionBuilder();

private:
    QStandardItemModel* m_model;
    //QStandardItemModel* modelVars;
    QDataWidgetMapper *m_mapper;
    QList<NCReportGroup*> m_removedGroups;
    NCRDDocument *m_document;
    NCReportSection *m_detail;
    bool m_internalChange;
    bool m_sortGroups;

private:

    void setupModel();
    void setupListVars();
    void appendModel( NCReportGroup*, int row, bool =false );
    void updateVarListFromModel();
    void updateModelFromVarList();
    void updateEnable();

};

#endif
