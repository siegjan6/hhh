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
#ifndef NCRDDATASOURCEDIALOG_H
#define NCRDDATASOURCEDIALOG_H

#include <QDialog>
#include <QList>
#include <QStandardItem>
#include <QItemDelegate>

#include "ui_variabledialog.h"
#include "ui_datasourcedialog.h"

QT_BEGIN_NAMESPACE
class QStandardItemModel;
class QSqlQueryModel;
class QDataWidgetMapper;
class QComboBox;
QT_END_NAMESPACE

class NCRDDocument;
class NCReportDataSource;
class NCReportSQLDataSource;
class NCReportDataViewDataSource;

/*!
Datasource properties dialog class
*/
class NCRDDataSourceDialog : public QDialog, private Ui::NCRDDataSourceDialogUI
{
  Q_OBJECT
public:
    NCRDDataSourceDialog(NCRDDocument *doc, QWidget* parent = 0 );
    ~NCRDDataSourceDialog();

    enum DataColumns {
        ColID = 0,
        ColDSTYPE, //Data source type
        ColLOCTYPE,
        ColFILENAME,
        ColHAVEFILTER,
        ColFILTER,
        ColISEXTERNAL,
        ColHOST,
        ColDB,
        ColUSER,
        ColPASSW,
        ColPORT,
        ColDRIVER,
        ColQRY,
        ColSPECTEXT,
        ColSPECXML,
        ColLISTID,
        ColMODELID,
        ColCUSTOMID,
        ColCUSTOMDESCR,
        ColTXTCOLDELIM,
        ColTXTCDCHAR,
        ColSLCOLDELIM,
        ColSLCDCHAR,
        ColCONNID,
        ColPOINTER, //NCRDDataSourceItem( datasource, flagNew );
        ColAllowEmpty,
        ColPARENTID,
        ColOPENROLE,
        ColTXTCOLHEADER,
        ColFWDONLY,
        ColCONNECTOPTIONS,
        ColPKCOLINDEX,
        ColMODELRELATIONSPEC,
        ColTXTENCODING,
        ColUseModelAppearance,
        ColForceUpdateOnStepBackRel,
        ColDataViewName,//me:扩展
        ColEnableSampling,
        ColSamplingField,
        ColSamplingPeriodPara,
        ColSamplingNullGap,
        ColCount
    };

    void applyChanges();
    void discardChanges();

    QComboBox *comboBoxDSType();
    QComboBox *comboBoxLocType();
    QComboBox *comboTxtColumnDelimiter();
    QComboBox *comboSlColumnDelimiter();
    QComboBox *comboOpenRole();
    QComboBox * comboDataViewName();

    QStandardItemModel* getModel() const { return m_model; }
    void setCurrentDataSource( const QString& dsID );
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
    void updateEnable();
    void textEditorContentChanged();
    void testExternalConnection();
    void runSqlTest();
    void activatedInDataViewNameComboBox(int index);//me:扩展
private:
    QStandardItemModel* m_model;
    QStandardItemModel* m_columnViewModel;
    QStandardItemModel* m_dataViewFieldModel;//me:扩展 显示某数据视图的所有字段信息
    QDataWidgetMapper *m_mapper;
    NCRDDocument *m_document;
    QList<NCReportDataSource*> m_removedDatasources;

    QSqlQueryModel *m_sqlTestModel;
    NCReportSQLDataSource* m_sqlTestDataSource;

    void setupModel();
    void appendModel( NCReportDataSource*, int row, bool flagNew );
    void setSqlDataSourceData( NCReportSQLDataSource*, int row );
    void updateColumnView();
    void updateDataViewNames();//me:扩展
    void updateFieldsInDataView();//me:扩展
    NCReportDataViewDataSource * getDataViewDataSource();//me:扩展
    void updateParentDataSourceWidget();
};

/*!
QStandardItem class for storing datasources
*/
class NCRDDataSourceItem : public QStandardItem
{
public:
    NCRDDataSourceItem( NCReportDataSource *ds, bool flag )
        : datasource(ds), flagnew( flag )
    {
    }

    ~NCRDDataSourceItem()
    {
    }

    bool flagNew()
    {
        return flagnew;
    }
    NCReportDataSource* dataSource()
    {
        return datasource;
    }
private:
    NCReportDataSource *datasource;
    bool flagnew;
};

/*!
Item delegate class for handling data widget delegation in Datasource dialog
*/
class NCRDDataSourceItemDelegate : public QItemDelegate
{
public:
    NCRDDataSourceItemDelegate( NCRDDataSourceDialog *parent );
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
protected:
    //void drawDisplay( QPainter *painter, const QStyleOptionViewItem &option, const QRect & rect, const QString & text ) const;
private:
    NCRDDataSourceDialog *dialog;

    bool isComboBox( const QModelIndex & index );
};


#endif

