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
#include "ncreportitemmodeldatasource.h"
#include "ncreportdef.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportevaluator.h"

#include <QAbstractItemModel>
#include <QStandardItem>

NCReportItemModelDataSource::NCReportItemModelDataSource(QObject * parent) : NCReportDataSource( parent )
{
    model = 0;
    setLocationType(Static);
    setDataSourceType( ItemModel );
    // add roles for data source based text format
    addAppearanceRole( Qt::FontRole );
    addAppearanceRole( Qt::TextAlignmentRole );
    addAppearanceRole( Qt::BackgroundRole );
    addAppearanceRole( Qt::ForegroundRole );
}

NCReportItemModelDataSource::~NCReportItemModelDataSource()
{
/*	if ( model )
        delete model;*/
}

void NCReportItemModelDataSource::setItemModel(QAbstractItemModel *m )
{
/*	if (model)
        delete model;*/
    model = m;
}

QAbstractItemModel* NCReportItemModelDataSource::itemModel() const
{
    return model;
}

bool NCReportItemModelDataSource::open(OPEN_MODEL model)
{
    m_OpenModel = model;
    if (!checkModel())
        return false;

    recno() = 0;
    setOpened(true);
    emit updateRequestFKey(id(), foreignKeyValue());
    emit updateRequest(id(), relationData );

    return true;
}

bool NCReportItemModelDataSource::close()
{
    if (!checkModel())
        return false;

    setOpened(false);
    flagEnd() = false;
    return true;
}

bool NCReportItemModelDataSource::first()
{
    if (!checkModel())
        return false;

    flagEnd() = false;
    recno() = 0;

    return true;
}

bool NCReportItemModelDataSource::last()
{
    if (!checkModel())
        return false;

    recno() = model->rowCount()-1;

    flagBegin() = false;

    return true;
}

bool NCReportItemModelDataSource::hasNext()
{
    return recno() < model->rowCount()-1;
}

bool NCReportItemModelDataSource::next(bool isSamplingPageBreak)
{
    Q_UNUSED(isSamplingPageBreak);

    if (!checkModel())
        return false;

    recno()++;
    if ( recno() > model->rowCount()-1 ) {
        recno() = model->rowCount()-1;
        flagEnd() = true;
        return false;
    }
    flagBegin() = false;

    return true;
}

bool NCReportItemModelDataSource::previous()
{
    if (!checkModel())
        return false;

    recno()--;
    if ( recno() < 0 ) {
        recno() = 0;
        flagBegin() = true;
        return false;
    }
    return true;
}

int NCReportItemModelDataSource::size() const
{
    return rowCount();
}

int NCReportItemModelDataSource::rowCount() const
{
    int cnt = 0;
    if (model)
        cnt = model->rowCount();
    else
        cnt = NCReportDataSource::rowCount();

    return cnt;
}

QVariant NCReportItemModelDataSource::value(const QString & columnname, bool* ok, int nQTDataRole ) const
{
    return value( columnIndexByName(columnname), ok, nQTDataRole );
}


QVariant NCReportItemModelDataSource::value(int columnno, bool *ok, int nQTDataRole ) const
{
    QModelIndex i = model->index( recno(), columnno );	// current index;
    if(ok)
        *ok = i.isValid();

    switch (nQTDataRole) {
    case def_nMarkHeaderRole: {
        // try to pass a header info!
        QVariant v = model->headerData(columnno, Qt::Horizontal, Qt::EditRole);
        if (v.isNull() || !v.isValid())
            return i.data( Qt::UserRole );
        else
            return v;
    }
    case def_nMarkHeaderDisplayRole: {
        // try to pass a header info!
        return model->headerData(columnno, Qt::Horizontal, Qt::DisplayRole);
    }
    case def_nMarkHeaderDecorationRole: {
        // try to pass a header info!
        return model->headerData(columnno, Qt::Horizontal, Qt::DecorationRole);
    }
    case def_nMarkHeaderUserRole: {
        // try to pass a header info!
        return model->headerData(columnno, Qt::Horizontal, Qt::UserRole);
    }
    case def_nMarkHeaderToolTipRole: {
        return model->headerData(columnno, Qt::Horizontal, Qt::ToolTipRole);
    }
    }

    if (nQTDataRole < 0)
        return i.data( Qt::EditRole );
    else
        return i.data(nQTDataRole);
}

bool NCReportItemModelDataSource::isValid() const
{
    if (model) {
        return model->index( recno(), 0 ).isValid();	// current index;
    }
    return false;
}

bool NCReportItemModelDataSource::read(NCReportXMLReader *r)
{
    // Assign item model by ID
    if ( r->isAttrib("modelID") )
        setModelID( r->attribToString("modelID") );
    if ( r->isAttrib("parentID") )
        setParentId( r->attribToString("parentID") );
    if ( r->isAttrib("useFormatRoles") )
        setUseAppearanceRoles( r->attribToBool("useFormatRoles") );

    if ( !r->reportDef()->itemModels().contains( modelid ) )
        return false;

    setItemModel( r->reportDef()->itemModels().value( modelid ) );

    return true;
}

bool NCReportItemModelDataSource::write(NCReportXMLWriter *w )
{
    w->writeAttribute("modelID", modelid );
    w->writeAttribute("parentID", parentId() );
    if ( useAppearanceRoles() )
        w->writeAttribute("useFormatRoles", "true");
    return true;
}

void NCReportItemModelDataSource::evaluate(NCReportEvaluator *ev)
{
    relationData = specText();
    //ev->evaluate( relationData, NCReportItem::Parameter );
    ev->evaluate( relationData, NCReportItem::DataSource );
}

bool NCReportItemModelDataSource::checkModel()
{
    if (!model) {
        if (error())
            error()->setError( tr("Item model not found. Data source ID: %1 Model ID: %2" ).arg(id()).arg(modelID()) );
        return false;
    }
    return true;
}

void NCReportItemModelDataSource::setModelID(const QString & id )
{
    modelid = id;
}

QString NCReportItemModelDataSource::modelID() const
{
    return modelid;
}

QString NCReportItemModelDataSource::columnName(int column) const
{
    if (model) {
        return model->headerData(column, Qt::Horizontal, Qt::EditRole).toString();
#ifdef QT_DEBUG
        QString c = model->headerData(column, Qt::Horizontal, Qt::EditRole).toString();
        qDebug("columnName(%i)=%s",column,qPrintable(c));
        return c;
#endif
    } else
        return QString();
}

int NCReportItemModelDataSource::columnCount() const
{
    if (model)
        return model->columnCount();
    else
        return NCReportDataSource::columnCount();
}

int NCReportItemModelDataSource::columnIndexByName(const QString &columnname) const
{
    // HRaba: -> expensive, but usefull!
    int columnno = 0;
    int nColumns = model->columnCount();
    QString strFieldName;

    QString cn(columnname);
    if ( columnname.contains(':') )
        cn = columnname.section(':',0,0);

    bool found = false;
    for (int i = 0; i < nColumns; i++) {
        strFieldName = model->headerData(i, Qt::Horizontal, Qt::EditRole).toString();
        if (strFieldName == cn) {
            columnno = i;
            found = true;
            break;
        }
    }
    if (!found) {
        if ( cn.startsWith("col") )
            columnno = cn.mid(3).toInt();
        else
            columnno = cn.toInt();
    }
    return columnno;
}

bool NCReportItemModelDataSource::seek(int index)
{
    if (index < model->rowCount()) {
        recno() = index;
        return true;
    }
    return false;
}

bool NCReportItemModelDataSource::update()
{
    bool ok = NCReportDataSource::update();
    emit updateRequest( id(), relationData);
    first();
    return ok;
}

QString NCReportItemModelDataSource::columnTitle(int column) const
{
    if (model) {
        return model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
#ifdef QT_DEBUG
        QString c = model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString();
        qDebug("columnTitle(%i)=%s",column,qPrintable(c));
        return c;
#endif
    } else
        return columnName(column);
}

QString NCReportItemModelDataSource::columnToolTip(int column) const
{
    if (model) {
        return model->headerData(column, Qt::Horizontal, Qt::ToolTipRole).toString();
    } else {
        return NCReportDataSource::columnToolTip(column);
    }
}

QString NCReportItemModelDataSource::rowTitle(int column) const
{
    if (model)
        return model->headerData(column, Qt::Vertical, Qt::DisplayRole).toString();
    else
        return QString();
}

