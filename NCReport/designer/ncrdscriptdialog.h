/****************************************************************************
*
*  Copyright (C) 2002-2015 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2015. 03. 02. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#ifndef NCRDSCRIPTDIALOG_H
#define NCRDSCRIPTDIALOG_H

#include <QDialog>
#include <QModelIndex>

class NCRDDocument;
class QStandardItemModel;
class QDataWidgetMapper;

namespace Ui {
class NCRDScriptDialog;
}

/*!

\section 功能

脚本编辑对话框管理类
【管理脚本编辑器相操作】

*/
class NCRDScriptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NCRDScriptDialog(NCRDDocument *doc, QWidget *parent = 0);
    ~NCRDScriptDialog();

    enum ModelColumns {
        Id=0,
        ScriptCode,
        ColumnCount
    };

public slots:
    void applyChanges();
    void discardChanges();

protected slots:
    void add();
    void remove();
    void onChanged();
    void slotItemChanged( const QModelIndex & current, const QModelIndex & previous );
    void openExpressionBuilder();

private:
    Ui::NCRDScriptDialog *ui;
    NCRDDocument *m_document;
    QStandardItemModel* m_model;
    QDataWidgetMapper *m_mapper;
    bool m_changed;

    QStringList m_removedScripts;

private:
    void setupModel();
    void appendModel( int row, const QString& id, const QString& script );
    void updateEnabled();

};

#endif // NCRDSCRIPTDIALOG_H
