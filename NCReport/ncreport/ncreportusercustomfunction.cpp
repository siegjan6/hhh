#include "ncreportusercustomfunction.h"

#include "ncreportitem.h"
#include "ncreportoutput.h"
#include "ncreportevaluator.h"
#include "ncreportfielditem.h"


void NCRDUserFunction_getValueWithNullOrEmpty::updateValue(NCReportItem *item, NCReportOutput *output, NCReportEvaluator *evaluator)
{
    Q_UNUSED(output)
    //Q_UNUSED(evaluator)

    if(item) {
        //item->setValue("This is a user function getValueWithNullOrEmpty value!");
        if(item->objectType() == NCReportItem::ObjectType::Label || item->objectType() == NCReportItem::ObjectType::Field){
            NCReportLabelItem* it = (NCReportLabelItem*)item;
            if(!it) return;
            QVariant v = it->value();
            if(!v.isValid()){
                QString r;
                evaluator->evaluateScript("_INVALIDSTRING", r);
                it->setDisplayValue(r);
            }
            else if(v.isNull() || v.toString() == "undefined"){
                QString r;
                evaluator->evaluateScript("_NULLSTRING", r);
                it->setDisplayValue(r);
            }
            else if (v.toString().isEmpty()){
                QString r;
                evaluator->evaluateScript("_EMPTYSTRING", r);
                it->setDisplayValue(r);
            }
            //item->setValue("123");
            //item->setText("456");
        }
    }
    //item->setDisplayValue("This is a user coded value!");
//    item->setText("This is a user function getValueWithNullOrEmpty value!");
}

