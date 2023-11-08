/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Ltd. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*  Created: 2014.05.05. (nocisoft)
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  office@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncreportvariablemodifier.h"
#include "ncreportevaluator.h"
#include "ncreportdef.h"

#include <QTextStream>

NCReportVariableModifier::NCReportVariableModifier()
{
}

bool NCReportVariableModifier::modifyValue( const QString &expression, NCReportEvaluator *evaluator)
{
    QString exp(expression);
    QTextStream in(&exp);
    int lineno=0;
    bool ok=true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        lineno++;
        if (line.isEmpty())
            continue;

        if (line.startsWith("$V{"))
            ok = evaluate(line, evaluator);
        else
            continue;

        if (!ok)
            break;
    }

    return ok;
}

bool NCReportVariableModifier::evaluate(const QString &line, NCReportEvaluator *evaluator)
{
    ModifierFunction function = Invalid;

    const char* operators[] = { "+=", "-=", "/=", "*=", "=", 0 };
    int i=0;
    while (operators[i]) {
        if (line.contains(operators[i])) {
            function = (ModifierFunction)i;
            break;
        }
        i++;
    }

    if (function==Invalid)
        return false;

//    if ( line.contains(operators[PlusEqual]) )
//        function = PlusEqual;
//    else if ( line.contains(operators[MinusEqual]) )
//        function = MinusEqual;
//    else if ( line.contains(operators[PlusEqual]) )
//        function = PlusEqual;
//    else if ( line.contains(operators[PlusEqual]) )
//        function = PlusEqual;
//    else if ( line.contains(operators[PlusEqual]) )
//        function = PlusEqual;
//    else if ( line.contains(operators[PlusEqual]) )
//        function = PlusEqual;
//    else if ( line.contains(operators[PlusEqual]) )
//        function = PlusEqual;

    QString leftOperand = line.section(operators[function],0,0).trimmed();
    if (leftOperand.isEmpty()) {
        return false;
    }
    QString varID(leftOperand.mid(3));
    varID.chop(1);
    NCReportVariable *v = evaluator->reportDef()->variables().value(varID);
    if (!v) {
        return false;
    }

    QString rightOperand = line.section(operators[function],1,1).trimmed();
    if (rightOperand.isEmpty()) {
        return false;
    }
    evaluator->evaluate( rightOperand, NCReportItem::DataSource );
    evaluator->evaluate( rightOperand, NCReportItem::Parameter );
    evaluator->evaluate( rightOperand, NCReportItem::Variable );

    QVariant result;
    if (evaluator->evaluateScript(rightOperand, result)) {
        switch(function) {
        case Equal: v->modifyValue(result.toDouble()); break;
        case PlusEqual: v->modifyValue( v->value().toDouble() + result.toDouble() ); break;
        case MinusEqual: v->modifyValue( v->value().toDouble() - result.toDouble() ); break;
        case DivideEqual: v->modifyValue( v->value().toDouble() / result.toDouble() ); break;
        case MultiplyEqual: v->modifyValue( v->value().toDouble() * result.toDouble() ); break;
        case Invalid: return false;
        }
    }

    return true;
}
