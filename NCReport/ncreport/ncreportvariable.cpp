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
#include "ncreportvariable.h"
#include "ncreportutils.h"


NCReportVariable::NCReportVariable() : NCReportData( Numeric ),
    m_functionType(Count),
    m_resetScope(Report),
    m_relatedDataSource(0)
    //m_testPassVariable(0)
{
}

NCReportVariable::~ NCReportVariable()
{
    //delete m_testPassVariable;
}

void NCReportVariable::setExpression(const QString & exp)
{
    m_varExp = exp;
}


void NCReportVariable::setFunction(const FunctionType ftype )
{
    m_functionType = ftype;
}

NCReportVariable::FunctionType NCReportVariable::function() const
{
    return m_functionType;
}

const QString& NCReportVariable::expression() const
{
    return m_varExp;
}


void NCReportVariable::update(const QVariant & val, CorrectionType ct)
{
    short dFactor = 1;
    if (ct==Undo)
        dFactor = -1;

    switch ( m_functionType ) {
        case Count: {
            m_currentValue.counter+= dFactor;
            setValue( m_currentValue.counter, Numeric );
            break;
        }
        case Sum: {
            m_currentValue.value += dFactor*val.toDouble();
            setValue( m_currentValue.value, Numeric );
            break;
        }
        case Average: {
            m_currentValue.counter+= dFactor;
            m_currentValue.value += dFactor*val.toDouble();
            setValue( m_currentValue.value/m_currentValue.counter, Numeric );
            break;
        }
        case Std: {
            // Standard deviation
            m_currentValue.counter+= dFactor;
            m_currentValue.value += dFactor*val.toDouble();
            if (dFactor<0)  // undo
                m_currentValue.valueHistory.removeLast();
            else
                m_currentValue.valueHistory.append(val.toDouble());
            setValue( NCReportUtils::standardDeviation(m_currentValue.valueHistory), Numeric );
            break;
        }
        case Min: {
            if ( val.toDouble() < m_currentValue.value )
                m_currentValue.value = val.toDouble();
            setValue( m_currentValue.value, Numeric );
            break;
        }
        case Max: {
            if ( val.toDouble() > m_currentValue.value )
                m_currentValue.value = val.toDouble();
            setValue( m_currentValue.value, Numeric );
            break;
        }
        case System: {
            break;
        }
    }
    if (ct==Off)
        m_lastUpdateValue = val;	// save last value
}

void NCReportVariable::reset()
{
    m_currentValue.value = initValue();
    m_currentValue.counter = initValue();
    m_currentValue.valueHistory.clear();
    setValue( initValue(), dataType() );
}

void NCReportVariable::correction(CorrectionType ct)
{
    update( m_lastUpdateValue, ct );
}

NCReportVariable::FunctionType NCReportVariable::nameToFunction(const QString & fname) const
{
    if ( fname.toUpper() == "COUNT" )
        return Count;
    else if ( fname.toUpper() == "SUM" )
        return Sum;
    else if ( fname.toUpper() == "AVERAGE" )
        return Average;
    else if ( fname.toUpper() == "STD" )
        return Std;
    else if ( fname.toUpper() == "MIN" )
        return Min;
    else if ( fname.toUpper() == "MAX" )
        return Max;

    return Count;
}

QString NCReportVariable::functionToName() const
{
    switch ( m_functionType ) {
        case Count: return "COUNT";
        case Sum: return "SUM";
        case Average: return "AVERAGE";
        case Std: return "STD";
        case System: return "SYSTEM";
        case Min: return "MIN";
        case Max: return "MAX";
    }

    return "";
}

NCReportVariable::ResetScope NCReportVariable::scope() const
{
    return m_resetScope;
}

QString NCReportVariable::scopeToName() const
{
    switch ( m_resetScope ) {
        case Report: return "Report";
        case Page: return "Page";
        case Group: return "Group";
    }

    return "";
}

NCReportVariable::ResetScope NCReportVariable::nameToScope(const QString & sname) const
{
    if ( sname.toUpper() == "REPORT" )
        return Report;
    else if ( sname.toUpper() == "PAGE" )
        return Page;
    else if ( sname.toUpper() == "GROUP" )
        return Group;

    return Report;
}

NCReportDataSource *NCReportVariable::relatedDataSource() const
{
    return m_relatedDataSource;
}

void NCReportVariable::setRelatedDataSource(NCReportDataSource *ds)
{
    m_relatedDataSource = ds;
}

//NCReportVariable *NCReportVariable::testPassVariable() const
//{
//    return m_testPassVariable;
//}

void NCReportVariable::setInitValue(double val)
{
    if ( m_functionType == Count ) {
        m_initValue.value = 0;
        m_initValue.counter = (int)val;
    } else {
        m_initValue.value = val;
        m_initValue.counter = 0;
    }
}

double NCReportVariable::initValue() const
{
    if ( m_functionType == Count )
        return (double)m_initValue.counter;
    else if ( m_functionType == Min )
        return 1.79769e+308;
    else
        return m_initValue.value;
}

/*!
 * \brief NCReportVariable::modifyValue
 * \param value
 * Modifies the variable to \param value. In normal situations we don't need to call this function. It is used by the \class NCReportVariableModifier
 * that forces the modification when the user want to manipulate the variable.
 */
void NCReportVariable::modifyValue( double value )
{
    if ( m_functionType == Count ) {
        m_currentValue.counter = (int)value;
        setValue( m_currentValue.counter, Numeric );
    } else {
        m_currentValue.value = value;
        setValue( m_currentValue.value, Numeric );
    }
}

void NCReportVariable::setFunction(const QString & funcname )
{
    setFunction( nameToFunction(funcname) );
}

void NCReportVariable::setScope(const QString & scopename )
{
    setScope( nameToScope( scopename ) );
}

void NCReportVariable::setScope(const ResetScope rt )
{
    m_resetScope = rt;
}

