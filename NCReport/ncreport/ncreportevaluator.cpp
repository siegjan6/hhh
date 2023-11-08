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
#include "ncreportevaluator.h"
#include "ncreportdef.h"
#include "ncreportdatasource.h"
#include "ncreportvariable.h"
#include "ncreportgroup.h"
#include "ncreportdirector.h"
#include "ncreportimageitem.h"
#include "ncreportgraphitem.h"
#include "ncreportbarcodeitem.h"
#include "ncreporttextitem.h"
#include "info.h"
#include "ncreportutils.h"
#include "ncreportdataformat.h"
#include "ncreportpageoption.h"

#ifdef QT_SCRIPT_LIB
#include <QScriptValue>
#include <QScriptEngine>
#endif
#include <QDate>
#include <QSysInfo>
#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include <QCoreApplication>
#include <QFont>

#define NCREPORT_PARAMETER_TOKEN	"$P{"
#define NCREPORT_DATASOURCE_TOKEN	"$D{"
#define NCREPORT_VARIABLE_TOKEN		"$V{"
#define NCREPORT_FIELD_TOKEN		"$F{"
#define NCREPORT_USERSCRIPT_TOKEN	"$S{"
#define NCREPORT_USERPRE_TOKEN	    "$PRE<#"
#define NCREPORT_USER_END_TOKEN	    "#>"
#define NCREPORT_END_TOKEN			"}"
#define NCREPORT_INVALID_DATA		"#DATA#"
#define NCREPORT_INVALID_PARAMATER	"#PARAM#"
#define NCREPORT_INVALID_VARIABLE	"#VAR#"
#define NCREPORT_SCAN_TOKEN			"$SCAN{"
#define NCREPORT_ENDSCAN_TOKEN		"$ENDSCAN{"

#define NCREPORT_GROUPFUNC_YEAR_TOKEN  "$YEAR{"
#define NCREPORT_GROUPFUNC_MONTH_TOKEN "$MONTH{"
#define NCREPORT_GROUPFUNC_WEEK_TOKEN  "$WEEK{"
#define NCREPORT_GROUPFUNC_DAY_TOKEN   "$DAY{"

//#define NCREPORT_FORMAT_SEPARATOR_TOKEN ";"
//#define IF_OPEN_TOKEN		"$IF<%"
//#define IF_CLOSE_TOKEN		"%>"
//#define ENDIF_TOKEN			"$ENDIF"
#define SCRIPT_OPEN_TOKEN		"<%"
#define SCRIPT_CLOSE_TOKEN		"%>"

#define SYSVAR_PAGENO		"pageno"
#define SYSVAR_GLOBALPAGENO "gpageno"
#define SYSVAR_PAGECOUNT	"pagecount"
#define SYSVAR_REPORTNO		"reportno"
#define SYSVAR_REPORTCOUNT	"reportcount"
#define SYSVAR_FORCECOPIES	"forcecopies"
#define SYSVAR_CURRENTCOPY	"currentcopy"
#define SYSVAR_CURRENTROW	"currentrow"
#define SYSVAR_DATE			"date"
#define SYSVAR_TIME			"time"
#define SYSVAR_DATETIME		"datetime"

#define SYSVAR_COREAPPNAME		"coreappname"
#define SYSVAR_COREAPPVERSION	"coreappversion"
#define SYSVAR_COREORGNAME		"coreorgname"
#define SYSVAR_COREORGDOMAIN	"coreorgdomain"

#define SYSVAR_APPNAME		"appname"
#define SYSVAR_APPLONGNAME	"applongname"
#define SYSVAR_APPINFO		"appinfo"
#define SYSVAR_APPVERSION	"appversion"
#define SYSVAR_APPCOPYRIGHT	"appcopyright"
#define SYSVAR_QTVERSION	"qtversion"
#define SYSVAR_OS			"os"
#define SYSVAR_LANG			"lang"

#define MULTI_LINE_TEXT_BEGIN   "MLTXT("
#define MULTI_LINE_TEXT_END   ")"

#define PRE_EVAL "^"

NCReportEvaluator::NCReportEvaluator() :
    m_engine(0),
    m_reportDef(0),
    m_director(0),
    m_section(0),
    m_currentDataSource(0),
    m_currentDataColumnIndex(-1),
    m_currentDataRole(-1)
{
}

NCReportEvaluator::NCReportEvaluator(NCReportDef *reportDef ) :
    m_engine(0),
    m_reportDef(reportDef),
    m_director(0),
    m_section(0),
    m_currentDataSource(0),
    m_currentDataColumnIndex(-1),
    m_currentDataRole(-1)
{
}

NCReportEvaluator::~NCReportEvaluator()
{
#ifdef QT_SCRIPT_LIB
    if (m_engine)
        delete m_engine;
#endif
}

void NCReportEvaluator::setReportDef(NCReportDef *reportDef)
{
    m_reportDef = reportDef;
}

NCReportDef *NCReportEvaluator::reportDef()
{
    return m_reportDef;
}

bool NCReportEvaluator::evaluate(QString & exp)
{
    bool ok=true;
    ok = evaluateEmbeddedScripts(exp);
    if (ok)
        ok = evaluateScan(exp);

    if (ok)
        ok=evaluate( exp, NCReportItem::DataSource );
    if (ok)
        ok=evaluate( exp, NCReportItem::Parameter );
    if (ok)
        ok=evaluate( exp, NCReportItem::Variable );
    if (ok)
        ok=evaluate( exp, NCReportItem::FieldDisplay );
    return ok;
}


bool NCReportEvaluator::evaluate(QString & expr, NCReportItem::SourceType stype)
{
    QString token;
    switch (stype) {
        case NCReportItem::DataSource: token = NCREPORT_DATASOURCE_TOKEN; break;
        case NCReportItem::Parameter: token = NCREPORT_PARAMETER_TOKEN; break;
        case NCReportItem::Variable: token = NCREPORT_VARIABLE_TOKEN; break;
        case NCReportItem::FieldDisplay: token = NCREPORT_FIELD_TOKEN; break;
        default: return false;
    }

    int noo = expr.count( token );
    if (noo == 0)	// token not found
        return true;

    bool ok=false;
    for ( int i=1; i <= noo; i++ ) {
        int beginPos = 0;
        int length = 0;
        bool multiLineCare = false;
        QString name = parseTokenExpression( expr, token, NCREPORT_END_TOKEN, beginPos, length );

        if ( name.startsWith(MULTI_LINE_TEXT_BEGIN)) {
            int beginPos = 0;
            int length = 0;
            name = parseTokenExpression( name, MULTI_LINE_TEXT_BEGIN, MULTI_LINE_TEXT_END, beginPos, length );
            multiLineCare = true;
        }

        QVariant val;
        QString stringValue;
        ok=false;
        switch (stype) {
        case NCReportItem::DataSource:
            ok =  getDataSourceValue( name, val);
            break;
        case NCReportItem::Parameter:
            ok =  getParameterValue( name, val );
            break;
        case NCReportItem::Variable:
            ok =  getVariableValue( name, val );
            break;
        case NCReportItem::FieldDisplay:
        {
            ok = true;
            stringValue = getFieldDisplayValue( name );
            break;
        }
        default:
            break;
        }

        if ( multiLineCare ) {
            if (stringValue.isNull())
                stringValue = val.toString();

            QString crlf( QString("\\") + QString("n") );

            stringValue.replace("\r\n", crlf );
            stringValue.replace("\n\r", crlf );
            stringValue.replace("\n", crlf );
        }

#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("Evaluate: %s = '%s'",qPrintable(name),qPrintable(stringValue.isNull() ? val.toString() : stringValue));
#endif

        if (ok)
            expr.replace( beginPos, length, stringValue.isNull() ? val.toString() : stringValue );
        else
            expr.replace( beginPos, length, "N/A" );

        QString exprTmp = expr;
        if(exprTmp.count(PRE_EVAL)>0) {
            quoteMarkSafety( exprTmp );
            quoteMarkReplace(exprTmp,false);
            int b = exprTmp.indexOf(PRE_EVAL);
            int e = exprTmp.indexOf(PRE_EVAL,b+1);
            exprTmp = exprTmp.mid(b+1, e-b);
            QVariant result;
            ok = evaluateScriptPrivate(exprTmp,result);
            expr = expr.replace(PRE_EVAL,"");
            continue;
        }
    }
    return ok;
}

bool NCReportEvaluator::evaluateEmbeddedScripts(QString &exp)
{
    bool ok = true;
    int beginpos = 0;
    while (beginpos >= 0) {
        beginpos = exp.indexOf( SCRIPT_OPEN_TOKEN, beginpos );
        int endpos = exp.indexOf( SCRIPT_CLOSE_TOKEN, beginpos+1 );
        if ( ( beginpos < 0 ) || ( endpos <0 ) )
            break;
        int nOpenTokenLength = QByteArray(SCRIPT_OPEN_TOKEN).size();

//    bool ok = true;
//    if ( exp.contains(SCRIPT_OPEN_TOKEN) && exp.contains(SCRIPT_CLOSE_TOKEN) ) {
//        for (int num=0; num<100; ++num ) {
//            int beginpos = exp.indexOf( SCRIPT_OPEN_TOKEN );
//            if (beginpos<0)
//                break;
//            int endpos = exp.indexOf( SCRIPT_CLOSE_TOKEN, beginpos+1 );
//            int nOpenTokenLength = QByteArray(SCRIPT_OPEN_TOKEN).size();

        int nCloseTokenLength = QByteArray(SCRIPT_CLOSE_TOKEN).size();
        //提取出脚本
        QString script = exp.mid( beginpos + nOpenTokenLength, endpos - beginpos - nOpenTokenLength );

        if ( !script.isEmpty() ) {
            quoteMarkReplace(script,true);
            evaluate( script, NCReportItem::DataSource );
            evaluate( script, NCReportItem::Parameter );
            evaluate( script, NCReportItem::Variable );
            quoteMarkSafety( script );
            quoteMarkReplace(script,false);
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Script: %s",qPrintable(script));
#endif
            QVariant result;
            //执行脚本
            ok = evaluateScript(script, result);
            script = result.toString();
            if (!ok)
                reportDef()->error()->setError(script);
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Script result: %s",qPrintable(script));
#endif
        }
        exp.replace(beginpos,endpos-beginpos+nCloseTokenLength+1,script);
    }
    return ok;
}

bool NCReportEvaluator::evaluateScan(QString &exp)
{
    bool ok = true;
    if ( exp.contains(NCREPORT_SCAN_TOKEN) && exp.contains(NCREPORT_ENDSCAN_TOKEN) ) {

        int from=0;	// search from
        for (int num=0; num<100; ++num ) {
            //SCAN iteration
            int beginpos = exp.indexOf( NCREPORT_SCAN_TOKEN, from );
            if (beginpos<0)
                break;
            int endpos = exp.indexOf( NCREPORT_END_TOKEN, beginpos+1 );
            int nTokenLength = QString(NCREPORT_SCAN_TOKEN).length();
            QString datasourceID = exp.mid( beginpos + nTokenLength, endpos - beginpos - nTokenLength );
            QString function;
            int scan_section_endpos = exp.indexOf( NCREPORT_ENDSCAN_TOKEN, from );
            QString band = exp.mid( beginpos + nTokenLength + datasourceID.length()+1,
                                    scan_section_endpos - beginpos - nTokenLength - datasourceID.length()- 1 );
            if (datasourceID.contains(',')) {
                function = datasourceID.right(1);
                datasourceID.chop(2);
            }

            if ( datasourceID.isEmpty() )
                datasourceID = m_reportDef->defaultDataSourceName();
            NCReportDataSource *ds = m_reportDef->dataSource(datasourceID);
            if ( ds ) {
                exp.remove(beginpos, scan_section_endpos-beginpos + QString(NCREPORT_ENDSCAN_TOKEN).length()+1 ); 		// remove band section
                //ds->first();
                if (function=="U") {
                    // update function
                    ds->evaluate( this );
                    ds->update();
                }
                ds->seek( -1 );
                QString subText;
                while ( ds->next() ) {
                //for ( int i=0; i<ds->rowCount(); ++i ) {
                    QString band_temp(band);
                    ok=evaluate( band_temp ); // recursive evaluation
                    subText += band_temp;
                }
                exp.insert( beginpos, subText );
            }
            from+= scan_section_endpos;
        }
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug( "Text: %s", qPrintable( exp ) );
#endif
    }
    return ok;
}

int NCReportEvaluator::getItemRoleInfo(QString& name)
{
    // HRaba
    if (name.isEmpty())
        return -1;
    int nDataRole = -1;

    QString strToken(NCREPORT_DATASOURCE_TOKEN);
    int beginpos = name.indexOf(strToken, 0 );
    int endpos = name.indexOf( NCREPORT_END_TOKEN, beginpos+1 );
    //int length = strToken.length();
    if ((beginpos >= 0) && (endpos >= 0))
    {
        int nTokenLength = strToken.length();
        name = name.mid(beginpos + nTokenLength, endpos - beginpos - nTokenLength);
    }

    if (name.endsWith(def_strMarkEditRole, Qt::CaseInsensitive))
    {
        nDataRole = Qt::EditRole;
        name = name.left(name.indexOf(def_strMarkEditRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }
    if (name.endsWith(def_strMarkUserRole, Qt::CaseInsensitive))
    {
        nDataRole = Qt::UserRole;
        name = name.left(name.indexOf(def_strMarkUserRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }
    if ( name.contains('{') && name.contains('}') && name.contains(def_strMarkUserRole,Qt::CaseInsensitive) )
    {
        int pos1 = name.indexOf("{")+1;
        int pos2 = name.indexOf("}");
        nDataRole = name.mid( pos1, pos2-pos1 ).toInt();
        name = name.left(name.indexOf(":"));
        return nDataRole;
    }
    if (name.endsWith(def_strMarkDisplayRole, Qt::CaseInsensitive))
    {
        nDataRole = Qt::DisplayRole;
        name = name.left(name.indexOf(def_strMarkDisplayRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }
    if (name.endsWith(def_strMarkDecorationRole, Qt::CaseInsensitive))
    {
        nDataRole = Qt::DecorationRole;
        name = name.left(name.indexOf(def_strMarkDecorationRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }
    if (name.endsWith(def_strMarkHeaderRole, Qt::CaseInsensitive))
    {
        nDataRole = def_nMarkHeaderRole;
        name = name.left(name.indexOf(def_strMarkHeaderRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }
    if (name.endsWith(def_strMarkHeaderDisplayRole, Qt::CaseInsensitive))
    {
        nDataRole = def_nMarkHeaderDisplayRole;
        name.chop(sizeof(def_strMarkHeaderDisplayRole)-1);
        return nDataRole;
    }
    if (name.endsWith(def_strMarkHeaderDecorationRole, Qt::CaseInsensitive))
    {
        nDataRole = def_nMarkHeaderDecorationRole;
        name = name.left(name.indexOf(def_strMarkHeaderDecorationRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }
    if (name.endsWith(def_strMarkHeaderUserRole, Qt::CaseInsensitive))
    {
        nDataRole = def_nMarkHeaderUserRole;
        name = name.left(name.indexOf(def_strMarkHeaderUserRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }
    if (name.endsWith(def_strMarkHeaderToolTipRole, Qt::CaseInsensitive))
    {
        nDataRole = def_nMarkHeaderToolTipRole;
        name = name.left(name.indexOf(def_strMarkHeaderToolTipRole, -1, Qt::CaseInsensitive));
        return nDataRole;
    }

    return nDataRole;
}

/*!
 * \brief NCReportEvaluator::updateFieldAppearance
 * \param field
 * \return true if the set was valid
 * Updates the possibly appearance settings from the data source to a field item if available.
 * It uses Qt::ItemDataRole that is useful for item model based sources when text style properties are set for the model.
 */
bool NCReportEvaluator::updateFieldAppearance(NCReportFieldItem *field)
{
    if (!field->dataSource() || !field->dataSource()->useAppearanceRoles())
        return false;

    bool ok = true;
    for (int i=0; i<field->dataSource()->appearanceRoles().count(); i++) {

        if (i==0)
            field->restoreTextStyle();

        QVariant v = field->dataSource()->value(field->dataColumn(), &ok, field->dataSource()->appearanceRoles().at(i) );
        if (!v.isValid())
            continue;

        switch (field->dataSource()->appearanceRoles().at(i)) {
        case Qt::FontRole:
            field->setFont( v.value<QFont>() );
            break;
        case Qt::TextAlignmentRole:
            field->setAlignment( (Qt::AlignmentFlag)v.value<int>() );
            break;
        case Qt::BackgroundRole:
            field->setBackColor( v.value<QColor>() );
            break;
        case Qt::ForegroundRole:
            field->setForeColor( v.value<QColor>() );
            break;
        }
    }

    return ok;
}

bool NCReportEvaluator::getDataSourceValue( const QString& columnExpr, QVariant& val )
{
    if (columnExpr.isEmpty())
        return false;

    // CONSTANT VALUES
    if (columnExpr == "%CONST") {
        val = "0";

        return true;
    }
    QString column(columnExpr);

    // Substitute parameter
    if (column.startsWith(NCREPORT_PARAMETER_TOKEN))
        evaluate( column, NCReportItem::Parameter );

    m_currentDataSource = 0;
    m_currentDataColumnIndex = -1;
    QString dataSourceID;
    QString strFieldName;
    QString functionName;
    int funcType = NoFunc;

    if (!parseDataSourceExpression(column, dataSourceID, strFieldName, functionName, funcType))
        return false;

    m_currentDataRole = getItemRoleInfo(strFieldName); // HRaba
    int rowPosition = -2; //the current data row (record) is used

    if ( strFieldName.contains(',') ) {
        // contains row positioning parameter
        rowPosition = strFieldName.section(',',1,1).toInt();
        strFieldName = strFieldName.section(',',0,0);
    }
    if (dataSourceID.isEmpty()) {
        if (m_director)
            m_currentDataSource = m_director->currentDataSource();
    } else {
        m_currentDataSource = m_reportDef->dataSource(dataSourceID);
    }

    if (!m_currentDataSource) {
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("Datasource %s not found (Exp: %s)", qPrintable(dataSourceID),qPrintable(column) );
#endif
        return false;
    }

    if ( m_currentDataSource->isOpened() ) {
        if ( rowPosition > -2 )
            m_currentDataSource->seek( rowPosition );
        bool ok = true;

        if (funcType == DataSourceFunction) {
            evaluateFunctionValue(functionName, funcType, val, m_currentDataSource);
            /* disable fast update from updateFieldFromDataSource() to force the update of functions */
            m_currentDataSource = 0;
            return true;
        }
        m_currentDataColumnIndex = m_currentDataSource->columnIndexByName(strFieldName);
        if (m_currentDataColumnIndex>=0)
            val = m_currentDataSource->value(m_currentDataColumnIndex, &ok, m_currentDataRole);
        else if (m_currentDataColumnIndex==-2)
            val = m_currentDataSource->value(strFieldName, &ok, m_currentDataRole);
        else
            return false;

        if (funcType==ValueFunction) {
            evaluateFunctionValue(functionName, funcType, val, m_currentDataSource);
            /* disable fast update from updateFieldFromDataSource() to force the update of functions */
            m_currentDataSource = 0;
        }

        if (!ok)
            return false;
    }

    return true;
}

bool NCReportEvaluator::getParameterValue( const QString& name, QVariant& val )
{
    val = m_reportDef->parameters().value(name);
    return true;
}

bool NCReportEvaluator::getVariableValue( const QString& name, QVariant& val )
{
    NCReportVariable *v = m_reportDef->variables().value(name);
    if (v) {
        val = v->value();
        return true;
    } else {
        if ( getSysVarValue( name, val ) )
            return true;
    }

    return false;
}



bool NCReportEvaluator::evaluateScript( const QString & exp, QString & result )
{
    QVariant value;
    bool ok = evaluateScriptPrivate(exp,value);
    result = value.toString();
    return ok;
}

bool NCReportEvaluator::evaluateScript(const QString & exp, bool& result)
{
    QVariant value;
    bool ok = evaluateScriptPrivate(exp,value);
    if (ok)
        result = value.toBool();
    else
        result = false;
    return ok;
}


bool NCReportEvaluator::evaluateScript(const QString & exp, double & result)
{
    QVariant value;
    bool ok = evaluateScriptPrivate(exp,value);
    result = value.toDouble();
    return ok;
}


bool NCReportEvaluator::evaluateScript(const QString & exp, QVariant & result)
{
    return evaluateScriptPrivate(exp,result);
}

bool NCReportEvaluator::evaluateScriptPrivate(const QString &exp, QVariant &result)
{
    QString script(exp);
    if (script.contains(NCREPORT_USERPRE_TOKEN)) {
        // FOUND PRE IN-REPORT DEFINED SCRIPT
        int beginPos = 0;
        int length = 0;
        //script = script.trimmed();
        QString pre_script = parseTokenExpression( script, NCREPORT_USERPRE_TOKEN, NCREPORT_USER_END_TOKEN, beginPos, length );
        //QString pre_script = m_reportDef->script(scriptId);
        if (!pre_script.isEmpty()) {
            evaluate( pre_script, NCReportItem::DataSource );
            evaluate( pre_script, NCReportItem::Parameter );
            evaluate( pre_script, NCReportItem::Variable );
            evaluate( pre_script, NCReportItem::FieldDisplay );
#ifdef QT_SCRIPT_LIB
            if (!m_engine)
                m_engine = new QScriptEngine;

            QScriptValue sv = m_engine->evaluate( pre_script );
            if ( sv.isUndefined()  || sv.isError() )
                ;
            else {
                script = script.replace(NCREPORT_USERPRE_TOKEN+pre_script+NCREPORT_USER_END_TOKEN, "");
                evaluateScriptPrivate(script, result);
            }
#endif
        }
    }

    if (exp.contains(NCREPORT_USERSCRIPT_TOKEN)) {
        // FOUND IN-REPORT DEFINED SCRIPT
        int beginPos = 0;
        int length = 0;
        //script = script.trimmed();
        QString scriptId = parseTokenExpression( exp, NCREPORT_USERSCRIPT_TOKEN, NCREPORT_END_TOKEN, beginPos, length );
        script = m_reportDef->script(scriptId);
        if (!script.isEmpty()) {
            evaluate( script, NCReportItem::DataSource );
            evaluate( script, NCReportItem::Parameter );
            evaluate( script, NCReportItem::Variable );
            evaluate( script, NCReportItem::FieldDisplay );
        }
    }

#ifdef QT_SCRIPT_LIB
    if (!m_engine)
        m_engine = new QScriptEngine;

    QScriptValue sv = m_engine->evaluate( script );
    if ( sv.isUndefined() )
        return false;

    if ( sv.isError() ) {
        result = sv.toString();
        return false;
    } else {
        result = sv.toVariant();
    }
    return true;
#else
    Q_UNUSED(exp)
    Q_UNUSED(result)
    return false;
#endif
}

//从表达式中提取token.token被特殊的分割符包围
QString NCReportEvaluator::parseTokenExpression( const QString & expression, const QString &tokenBegin, const QString &tokenEnd, int& tokenBeginPos, int& tokenLength) const
{
    tokenBeginPos = expression.indexOf( tokenBegin, 0 );
    int tokenEndPos = expression.indexOf( tokenEnd, tokenBeginPos+1 );
    tokenLength = tokenEndPos-tokenBeginPos +1;

    int nTokenLength = tokenBegin.length();
    return expression.mid( tokenBeginPos + nTokenLength, tokenEndPos - tokenBeginPos - nTokenLength);
}

bool NCReportEvaluator::evaluateField(NCReportFieldItem * field)
{
    bool ok = false;
    NCReportFieldData* fd = (NCReportFieldData*)field->d;
    QString fieldExp = field->text();
    if ( fieldExp.isEmpty() )
        return false;

    switch ( field->sourceType() ) {
        case NCReportItem::DataSource:
            ok = updateFieldFromDataSource(fieldExp, fd->valueRef(), field);
            break;
        case NCReportItem::Template:
        case NCReportItem::ScriptExpression:
        {
            quoteMarkReplace(fieldExp,true);
            ok = evaluate( fieldExp );	// evaluates data, variables, parameters
            quoteMarkSafety( fieldExp );
            quoteMarkReplace(fieldExp,false);

            if ( field->sourceType() == NCReportItem::ScriptExpression )
                ok = evaluateScript( fieldExp, fd->valueRef() );	// stores evaluated script to displayValue
            else
                fd->setValue( fieldExp );

            break;
        }
        case NCReportItem::Parameter:
        {
            if ( field->isUpToDate() )
                ok = true;
            else {
                ok = getParameterValue( fieldExp, fd->valueRef() );
                bool isTemplate = false;
                if ( fd->valueRef().type() == QVariant::String ) {
                    QString s = fd->valueRef().toString();
                    if (s.contains(NCREPORT_DATASOURCE_TOKEN)) {
                        evaluate(s,NCReportItem::DataSource);
                        isTemplate = true;
                    }
                    if (s.contains(NCREPORT_PARAMETER_TOKEN)) {
                         evaluate(s,NCReportItem::Parameter);
                         isTemplate = true;
                    }
                    if (s.contains(NCREPORT_VARIABLE_TOKEN)) {
                         evaluate(s,NCReportItem::Variable);
                         isTemplate = true;
                    }
                    if (isTemplate)
                        fd->setValue(QVariant(s));
                }

                if (!isTemplate )
                    field->setUpToDate(true); //parameter does not change while running report
            }

            break;
        }
        case NCReportItem::Variable:
            ok = getVariableValue( fieldExp, fd->valueRef() );
            break;
        case NCReportItem::SystemVariable:
            ok = getSysVarValue( fieldExp, fd->valueRef() );
            break;
        default: break;
    }

    if (!ok) { // The data is invalid or not found
        //fd->displayValue = INVALID_DATA;
        //return false;
    }

#if 0
    QString argTemplate(fd->arg);
    if (!argTemplate.isEmpty()) {
        evaluate( argTemplate, NCReportItem::DataSource );
        evaluate( argTemplate, NCReportItem::Parameter );
        evaluate( argTemplate, NCReportItem::Variable );
    }
#endif

    NCReportDataFormat::formatField(field);

//    formatData( fd->dataType(), fd->valueRef(), fd->displayValue, fd->formatnum, fd->localized, fd->fieldwidth, fd->format,
//                fd->precision, fd->fillchar, fd->numBlankIfZero, fd->dateFormat, argTemplate, fd->numChars, fd->textStyle.alignment );

    field->setAdjusted(false);
    return ok;
}

/*!
  Fast mode of updating the field from data source
  */
bool NCReportEvaluator::updateFieldFromDataSource(const QString& columnExpr, QVariant& val, NCReportFieldItem *field)
{
    bool ok = true;
    if (field->dataSource()) {
        val = field->dataSource()->value(field->dataColumn(), &ok, field->dataRole() );


    } else {
        ok = getDataSourceValue( columnExpr, val );
        // to increase the performance, we save shortcuts to data source:
        if (ok) {
            field->setDataSource(m_currentDataSource);
            field->setDataColumn(m_currentDataColumnIndex);
            field->setDataRole(m_currentDataRole);
        }

    }
    if (ok) {
        updateFieldAppearance( field );
    }


    return ok;
}

void NCReportEvaluator::evaluateFunctionValue(const QString &functionName, int fType, QVariant& value, NCReportDataSource *ds )
{
    if (fType==NoFunc)
        return;

    if (fType==ValueFunction) {

        QString arguments = functionName.mid(functionName.indexOf('(')+1);
        arguments.chop(1);
        arguments = arguments.trimmed();
        int pos = arguments.section(',',0,0).toInt();
        int n = arguments.section(',',1,1).toInt();
        if (functionName.startsWith("MID",Qt::CaseInsensitive))
            value = value.toString().mid(pos,n);
        else if (functionName.startsWith("LEFT",Qt::CaseInsensitive))
            value = value.toString().left(pos);
        else if (functionName.startsWith("RIGHT",Qt::CaseInsensitive))
            value = value.toString().right(pos);
        else if (functionName.startsWith("USERFUNC",Qt::CaseInsensitive))
            value = ds->getUserFunctionValue(value, arguments);

    } else if (fType==DataSourceFunction) {
        if (!ds)
            return;

        if (functionName=="rowCount()")
            value = ds->rowCount();
        else if (functionName=="isAvailable()")
            value = ds->isAvailable();
        else if (functionName=="isValid()")
            value = ds->isValid();
        else if (functionName=="isEmpty()")
            value = (ds->rowCount()==0);
        else if (functionName=="isNotEmpty()")
            value = (ds->rowCount()>0);
        else if (functionName=="update()") {
            value = "";
            ds->evaluate( this );
            if ( ds->update() )
                ds->first();
        }
        else
        {
            // 数据源扩展函数
            QString arguments = functionName.mid(functionName.indexOf('(')+1);
            arguments.chop(1);
            arguments = arguments.trimmed();
            QStringList args = arguments.split(',');
            QString colName = "";
            if(args.length() > 0)
                colName = args[0];
            QString filter = "";
            if(args.length() > 1)
                filter = args[1];
            if (functionName.startsWith("getValue",Qt::CaseInsensitive))
            {
                QString filterRes;
//                if(filter.startsWith(DELAY_EVAL) && filter.endsWith(DELAY_EVAL))
                bool ok = evaluateScript( filter, filterRes );
                QString columnRes;
                ok = evaluateScript( colName, columnRes );
                if(ok)
                    colName = columnRes;
                value = ds->value(colName, filterRes);
            }
        }
    }
}


NCReportEvaluator::FunctionType NCReportEvaluator::functionType(const QString &functionName) const
{
    int i=0;
    while ( availableValueFunctions()[i] ) {
        if (functionName.startsWith(availableValueFunctions()[i],Qt::CaseInsensitive)) {
            return ValueFunction;
        }
        i++;
    }
    i=0;
    while ( availableDataSourceFunctions()[i] ) {
        if (functionName.startsWith(availableDataSourceFunctions()[i],Qt::CaseInsensitive)) {
            return DataSourceFunction;
        }
        i++;
    }
    return NoFunc;
}

bool NCReportEvaluator::isFunction(const QString& name) const
{
    return name.contains('(');
}

bool NCReportEvaluator::parseDataSourceExpression(const QString &expr, QString &dataSourceID, QString &columnName, QString &functionName, int& fType ) const
{
    int dotCount = expr.count('.');
    if (dotCount==0) {
        // column without delimiter, this means the default datasource
        if ( isFunction(expr) ) {
            // function
            fType = functionType(expr);
            if (fType==DataSourceFunction) {
                functionName = expr;
            } else {
#ifdef NCREPORT_DEBUG_OUTPUT
                qDebug("Invalid function: %s", qPrintable(expr));
#endif
                return false;
            }
        } else {
            // column
            columnName = expr;
        }
    } else {
        QStringList parts = expr.split('.');
        switch (parts.count()) {
        case 2:
        {
            //例子:DataSource1.rowCount()
            if (isFunction(parts.at(1))) {
                fType = functionType(parts.at(1));
                if (fType==ValueFunction) {
                    // column.function
                    columnName = parts.at(0);
                    functionName = parts.at(1);
                } else if (fType==DataSourceFunction) {
                    // ds.function
                    dataSourceID = parts.at(0);
                    functionName = parts.at(1);
                } else {
                    // invalid function
#ifdef NCREPORT_DEBUG_OUTPUT
                     qDebug("Invalid function: %s", qPrintable(parts.at(1)));
#endif
                     return false;
                }
            } else {
                // ds.column
                dataSourceID = parts.at(0);
                columnName = parts.at(1);
            }
            break;
        }
        case 3:
        {
            //第一种情况:数据源名.字段名.函数()
            //例子:DataSource1.a1.MID(n,m)
            //第二种情况:扩展的数据源的字段名为数据源名.表名.字段名
            //两种情况,都有两个".".都会分割成三个token.所以 还是区分开这两种情况.
            //第一种情况,最后一个token是函数,有"(",")".故以此辨认出第一种情况.
            dataSourceID = parts.at(0);
            if(isFunction(parts.at(2))) //第一种情况
            {
                columnName = parts.at(1);
                functionName = parts.at(2);
            }
            else //第二种情况
            {
                columnName = parts.at(1);
                columnName.append(".").append(parts.at(2));
            }
            //<原代码>
            //dataSourceID = parts.at(0);
            //columnName = parts.at(1);
            //functionName = parts.at(2);
            //fType = ValueFunction;
            //</原代码>
            break;
        }
        default:
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("Invalid expression: %s", qPrintable(columnName));
#endif
            return false;
        } // END CASE
    }

    return true;
}

const QStringList NCReportEvaluator::availableSystemVariables()
{
    QStringList list;
    list << SYSVAR_PAGENO;
    list << SYSVAR_GLOBALPAGENO;
    list << SYSVAR_PAGECOUNT;
    list << SYSVAR_REPORTNO;
    list << SYSVAR_REPORTCOUNT;
    list << SYSVAR_FORCECOPIES;
    list << SYSVAR_CURRENTCOPY;
    list << SYSVAR_CURRENTROW;
    list << SYSVAR_DATE;
    list << SYSVAR_TIME;
    list << SYSVAR_DATETIME;

    list << SYSVAR_COREAPPNAME;
    list << SYSVAR_COREAPPVERSION;
    list << SYSVAR_COREORGNAME;
    list << SYSVAR_COREORGDOMAIN;

    list << SYSVAR_APPNAME;
    list << SYSVAR_APPLONGNAME;
    list << SYSVAR_APPINFO;
    list << SYSVAR_APPVERSION;
    list << SYSVAR_APPCOPYRIGHT;
    list << SYSVAR_QTVERSION;
    list << SYSVAR_OS;
    list << SYSVAR_LANG;

    list.sort();
    return list;
}

const QStringList NCReportEvaluator::availableVariables()
{
    QStringList list;
    QHash<QString,NCReportVariable*>::const_iterator i = m_reportDef->variables().constBegin();
    while ( i!= m_reportDef->variables().constEnd() ) {
        list << i.value()->id();
        ++i;
    }
    return list;
}

const char** NCReportEvaluator::availableDataSourceFunctions()
{
    static const char* p_array[] = { "rowCount", "isAvailable", "isValid", "isEmpty", "isNotEmpty", "update", "getValue", 0 };
    return p_array;
}

const char **NCReportEvaluator::availableValueFunctions()
{
    static const char* p_array[] = { "MID", "LEFT", "RIGHT", "USERFUNC", 0 };
    return p_array;
}

const int *NCReportEvaluator::availableValueFunctionArguments()
{
    static const int p_array[] = { 2, 1, 1, 1, 0 };
    return p_array;
}


bool NCReportEvaluator::evaluateVariable(NCReportVariable * var)
{
    QVariant val;
    bool ok = true;
    QString varExp(var->expression());
    bool isCurrentDS = true;
    NCReportDataSource *currentDS = m_director->currentDataSource();

    if ( varExp.contains(SCRIPT_OPEN_TOKEN) ) {
        ok = evaluate(varExp);
        val = varExp.toDouble();
    } else {
        isCurrentDS = (currentDS == m_currentDataSource);	// check if variable gets data from current data source or not

        ok = getDataSourceValue( varExp, val );
        if (ok)
            var->setRelatedDataSource(m_currentDataSource);
    }
    if (ok) {
        if (m_reportDef->option().updateOnlyCurrentDataSourceVariables()) {
            //-----------------------------------------------------------------
            // update only current DS. variable, but it causes problem
            // when using data source relations where the variables have data
            // source identifier.
            //-----------------------------------------------------------------
            if ( isCurrentDS )
                var->update( val );
        } else {
            //-----------------------------------------------------------------
            // updates variable always
            //-----------------------------------------------------------------
            var->update( val );
        }
    }

    return ok;
}

bool NCReportEvaluator::evaluateGroup(NCReportGroup * g)
{
    QVariant val;
    bool ok = true;
    int startPos = 0;
    int length = 0;
    if (g->expression().contains(SCRIPT_OPEN_TOKEN)) {
        QString strValue(g->expression());
        ok = evaluate(strValue);
        g->setValue(strValue);
        return ok;
    } else if (g->expression().startsWith(NCREPORT_GROUPFUNC_YEAR_TOKEN)) {
        QString column = parseTokenExpression( g->expression(), NCREPORT_GROUPFUNC_YEAR_TOKEN, NCREPORT_END_TOKEN, startPos, length );
        ok = getDataSourceValue( column, val );
        if (ok)
            val = val.toDate().year();
    } else if (g->expression().startsWith(NCREPORT_GROUPFUNC_MONTH_TOKEN)) {
        QString column = parseTokenExpression( g->expression(), NCREPORT_GROUPFUNC_MONTH_TOKEN, NCREPORT_END_TOKEN, startPos, length );
        ok = getDataSourceValue( column, val );
        if (ok)
            val = val.toDate().month();
    } else if (g->expression().startsWith(NCREPORT_GROUPFUNC_WEEK_TOKEN)) {
        QString column = parseTokenExpression( g->expression(), NCREPORT_GROUPFUNC_WEEK_TOKEN, NCREPORT_END_TOKEN, startPos, length );
        ok = getDataSourceValue( column, val );
        if (ok)
            val = val.toDate().weekNumber();
    } else if (g->expression().startsWith(NCREPORT_GROUPFUNC_DAY_TOKEN)) {
        QString column = parseTokenExpression( g->expression(), NCREPORT_GROUPFUNC_DAY_TOKEN, NCREPORT_END_TOKEN, startPos, length );
        ok = getDataSourceValue( column, val );
        if (ok)
            val = val.toDate().dayOfWeek();
    } else {
        ok = getDataSourceValue( g->expression(), val );
    }
    if (ok)
        g->setValue( val.toString() );
    else
        g->setValue( "NULL" );
    ok = true;  // we don't want error

    return ok;
}

bool NCReportEvaluator::getSysVarValue(const QString & name, QVariant & val)
{
    if ( name==SYSVAR_PAGENO || name =="pagenum" )
        val = m_director ? qMax(m_director->pageNum(),1) : 0;
    else if ( name==SYSVAR_GLOBALPAGENO )
        val = m_director ? m_director->globalPageNum() : 0;
    else if ( name==SYSVAR_PAGECOUNT )
        val = m_director ? m_director->pageCount() : 0;
    else if ( name==SYSVAR_REPORTNO || name=="reportnum")
        val = m_director ? m_director->reportNumber() : 0;
    else if ( name==SYSVAR_REPORTCOUNT )
        val = m_director ? m_director->reportCount() : 0;
    else if ( name==SYSVAR_FORCECOPIES )
        val = m_director ? m_director->numForceCopies() : 0;
    else if ( name==SYSVAR_CURRENTCOPY )
        val = m_director ? m_director->currentForceCopy() :0;
    else if ( name==SYSVAR_CURRENTROW )
        val = m_director ? m_director->currentRow() : 0;
    else if ( name==SYSVAR_DATE )
        val = QDate::currentDate();
    else if ( name==SYSVAR_TIME )
        val = QTime::currentTime();
    else if ( name==SYSVAR_DATETIME )
        val = QDateTime::currentDateTime();
    else if ( name==SYSVAR_COREAPPNAME )
        val = QCoreApplication::applicationName();
    else if ( name==SYSVAR_COREAPPVERSION )
        val = QCoreApplication::applicationVersion();
    else if ( name==SYSVAR_COREORGNAME )
        val = QCoreApplication::organizationName();
    else if ( name==SYSVAR_COREORGDOMAIN )
        val = QCoreApplication::organizationDomain();
    else if ( name==SYSVAR_COREAPPVERSION )
        val = QCoreApplication::applicationVersion();

    else if ( name==SYSVAR_APPNAME )
        val = NCREPORTAPP_NAME;
    else if ( name==SYSVAR_APPLONGNAME )
        val = NCREPORTAPP_LONGNAME;
    else if ( name==SYSVAR_APPINFO )
        val = QString() + NCREPORTAPP_NAME+" "+NCREPORTAPP_VERSION+" "+NCREPORTAPP_RELEASENAME+" "+NCREPORTAPP_COPYRIGHT;
    else if ( name==SYSVAR_APPVERSION )
        val = NCREPORTAPP_VERSION;//App()->appVersion();
    else if ( name==SYSVAR_APPCOPYRIGHT )
        val = NCREPORTAPP_COPYRIGHT;//;App()->appCopyright();
    else if ( name==SYSVAR_QTVERSION )
        val = qVersion();
    else if ( name==SYSVAR_OS ) {
        #if defined(Q_OS_WIN) || defined(Q_WS_WIN)
            val = QSysInfo::WindowsVersion;
        #endif
        #if defined(Q_OS_LINUX) || defined(Q_WS_X11)
            val = "Linux";
        #endif
        #if defined(Q_OS_MACX) || defined(Q_WS_MAC)
            val = "MacOSX";
        #endif
    }
    else if ( (name==SYSVAR_LANG || name=="language") && m_reportDef )
        val = m_reportDef->option().currentLanguage();
    else
        val = NCREPORT_INVALID_VARIABLE;

    return true;
}

QString NCReportEvaluator::getFieldDisplayValue(const QString &id)
{
    NCReportFieldItem *field = 0;
    bool found=false;
    QString value;

    if (m_section) {
        for (int i=0; i<m_section->reportItems().count(); ++i ) {
            NCReportItem *item= m_section->reportItems().at(i);
            if (item->objectType() == NCReportItem::Field && item->id() == id) {
                found=true;
                field=((NCReportFieldItem*)item);
                break;
            }
        }
    } else {
        for (int i=0; i<m_reportDef->fieldItems().count(); ++i ) {
            field = m_reportDef->fieldItems().at(i);
            if (field->id() == id) {
                found=true;
                break;
            }
        }
    }
    if (found) {
        if (!field->isUpToDate())
            field->updateValue(0,this);
        value = field->displayValue();
    }

    return value;
}

void NCReportEvaluator::setDirector(NCReportDirector *director)
{
    m_director = director;
}

NCReportDirector *NCReportEvaluator::director()
{
    return m_director;
}

void NCReportEvaluator::setSection(NCReportSection *section)
{
    m_section = section;
}

NCReportSection *NCReportEvaluator::section() const
{
    return m_section;
}

// todo: NCReportItem isEmpty() 则忽略不打印，针对此问题可以提供一个全局配置
bool NCReportEvaluator::evaluatePrintWhen(NCReportItem * item)
{
    if ( item->d->printWhen.isEmpty() ) {
//        item->d->printWhenResult = !item->isEmpty();
        item->d->printWhenResult = true;
    } else {
        item->d->printWhenResult = evaluatePrintWhen(item->d->printWhen);
        if ( item->d->printWhenResult )
//            item->d->printWhenResult = !item->isEmpty();
            item->d->printWhenResult = true;
    }

    if (item->d->printWhenResult)
        item->printWhenTrueAction(this);

    return item->d->printWhenResult;
}


bool NCReportEvaluator::evaluatePrintWhen(const QString & expr)
{
    if (expr.isEmpty())
        return true;

    if (m_director) {
        bool evenRow = m_director->currentRow()%2;
        if ( expr=="%2" || expr =="@EVEN" )
            return evenRow;
        else if ( expr=="!%2" || expr=="@ODD" )
            return !evenRow;
        else if ( expr=="@ISFINISHED" )
            return m_director->flag(NCReportDirector::FlagFinished);
        else if ( expr=="@ISNOTFINISHED" )
            return !m_director->flag(NCReportDirector::FlagFinished);
        else if ( expr=="@SKIP_NEXT" ) {
            // NEXT RECORD TRICK (FOR SKIP WITHIN A SECTION)
            if (m_director->nextRecord()) {
                return false;
            } else {
                m_director->finish();
                return false;
            }
        }
        else if ( expr=="@DS2" ) {
            // Secondary data source is valid
            if (m_director->currentDetail() && !m_director->currentDetail()->secondaryDatasourceID().isEmpty()) {
                NCReportDataSource *ds = m_reportDef->dataSource(m_director->currentDetail()->secondaryDatasourceID());
                if (ds)
                    return ds->isAvailable();
                else
                    return false;
            } else {
                return false;
            }
        }
    }

    bool ok = true;
    QString ex = expr;
    ok = evaluate( ex );	// evaluates data, variables, parameters

    if (!ok)
        return false;

    if (ex=="1" || ex=="true")
        return true;
    else if (ex=="0" || ex=="false")
        return false;

    bool result = true;
    ok = evaluateScript( ex, result );	// stores evaluated script to displayValue
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug( "Evaluate printWhen: %s Result: %i", qPrintable(ex),result );
#endif
    if (ok)
        return result;
    else
        qDebug( "Evaluate printWhen error: %s", qPrintable(ex));
    return ok;
}

bool NCReportEvaluator::evaluateDynamicText( NCReportItem *item, QString* target )
{
    // If target is not empty, then html text is stored in *target instead of using ->setHtml(...)

    NCReportTextItem *textItem = (NCReportTextItem*)item;
    NCReportTextData *td = (NCReportTextData*)item->d;
    QString errorMsg;

    switch ( item->sourceType() ) {
        case NCReportItem::Static:
            break;
        case NCReportItem::File:
        {
            if (td->text.contains('$')) {
                QString fileName = td->text;
                evaluate( fileName, NCReportItem::DataSource );
                evaluate( fileName, NCReportItem::Parameter );
                evaluate( fileName, NCReportItem::Variable );
                NCReportUtils::evaluateFileName(fileName);
                NCReportUtils::fileToString( fileName, td->documentTemplate );
            }
            break;
        }
        case NCReportItem::DataSource:
        {
            getDataSourceValue( td->text, td->valueRef() );
            td->documentTemplate = td->valueRef().toString();
            td->valueRef().clear();
            break;
        }
        case NCReportItem::FileNameFromDataSource:
        {
            getDataSourceValue( td->text, td->valueRef() );
            if ( !NCReportUtils::fileToString( td->valueRef().toString(), td->documentTemplate ) )
                errorMsg = QObject::tr("File not found: <i>%1</i>").arg( td->valueRef().toString() );
            break;
        }
        case NCReportItem::Parameter:
        {
            if (getParameterValue(td->text, td->valueRef()) ) {
                td->documentTemplate = td->valueRef().toString();
                td->valueRef().clear();
            } else {
                errorMsg = QObject::tr("Parameter not found: %1").arg( td->text );
            }
            break;
        }
        default:
            break;

    }

    if (errorMsg.isNull()) {
        QString text(td->documentTemplate);

        if ( item->isTemplateMode() )
            evaluate( text );

        if (target)
            *target = text;
        else
            textItem->setHtml( text );

    } else {
        textItem->setHtml(errorMsg);
    }

    return true;
}


bool NCReportEvaluator::evaluateDynamicImage(NCReportItem *item)
{
    bool ok=true;
    NCReportImageData* id = (NCReportImageData*)item->d;
    QVariant oldvalue = id->valueRef();
    //getDataSourceValue( id->text, id->valueRef() );
    // script or template mode
    QString tmpText = id->text;
    quoteMarkReplace(tmpText,true);
    evaluate( tmpText );	// evaluates data, variables, parameters
    quoteMarkSafety( tmpText );
    quoteMarkReplace(tmpText,false);

    if ( item->sourceType() == NCReportItem::ScriptExpression )
        evaluateScript( tmpText, id->valueRef() );
    else
        id->valueRef() = tmpText;

    if ( id->valueRef() != oldvalue )
        ok = ((NCReportImageItem*)item)->load( this );
    return ok;
}

bool NCReportEvaluator::evaluateDynamicGraph(NCReportItem *item )
{
    NCReportGraphData* gd = (NCReportGraphData*)item->d;
    getDataSourceValue( gd->text, gd->valueRef() );
    if (gd->sourceType == NCReportItem::DataSource)
        gd->graphDefinition = gd->valueRef().toString();

    return true;
}

bool NCReportEvaluator::evaluateDynamicBarcode( NCReportItem *item )
{
    NCReportBarcodeData* bd = (NCReportBarcodeData*)item->d;
    QVariant oldvalue = bd->valueRef();
    switch ( bd->sourceType ) {
    case NCReportItem::DataSource:
        getDataSourceValue( bd->text, bd->valueRef() );
        break;
    case NCReportItem::Parameter:
        getParameterValue( bd->text, bd->valueRef() );
        break;
    case NCReportItem::Template:
    case NCReportItem::ScriptExpression:
    {
        QString barcode = bd->text;
        quoteMarkReplace(barcode,true);
        evaluate( barcode );	// evaluates data, variables, parameters
        quoteMarkSafety( barcode );
        quoteMarkReplace(barcode,false);

        if ( item->sourceType() == NCReportItem::ScriptExpression )
            evaluateScript( barcode, bd->valueRef() );
        else
            bd->setValue( barcode );

        break;
    }
    }
    if ( bd->valueRef() != oldvalue )
        ((NCReportBarcodeItem*)item)->update_barcode();
    return true;
}

void NCReportEvaluator::quoteMarkSafety( QString& exp )
{
    exp.replace("\"","\\\"" );
    exp.replace("'","\'" );
}

void NCReportEvaluator::quoteMarkReplace( QString& exp, bool encode)
{
    if (encode) {
        // encode
        exp.replace("'","{SINGLE_QUOTE_MARK}");
        exp.replace("\"","{DOUBLE_QUOTE_MARK}");
    } else {
        // decode
        exp.replace("{SINGLE_QUOTE_MARK}","'");
        exp.replace("{DOUBLE_QUOTE_MARK}","\"");
    }
}

void NCReportEvaluator::tokenizeKeyWordList(QStringList &keywordList, NCReportItem::SourceType type)
{
    return;
    for (int i=0;i<keywordList.count();++i) {
        QString s = keywordList.at(i);
        tokenizeKeyWord(s, type);
        keywordList[i] = s;
    }
}

void NCReportEvaluator::tokenizeKeyWord(QString &keyword, NCReportItem::SourceType type)
{
    QString begin_token;
    switch (type) {
    case NCReportItem::DataSource: begin_token=NCREPORT_DATASOURCE_TOKEN; break;
    case NCReportItem::Parameter: begin_token=NCREPORT_PARAMETER_TOKEN; break;
    case NCReportItem::Variable:
    case NCReportItem::SystemVariable: begin_token=NCREPORT_VARIABLE_TOKEN; break;
    case NCReportItem::FieldDisplay: begin_token=NCREPORT_FIELD_TOKEN; break;
    default:
        return;
    }
    if (!keyword.startsWith('$'))
        keyword = begin_token + keyword + NCREPORT_END_TOKEN;
}

QStringList NCReportEvaluator::arrayToStringList(const char *array[])
{
    QStringList list;
    int i=0;
    while ( array[i] ) {
        list << array[i];
        i++;
    }
    return list;
}



