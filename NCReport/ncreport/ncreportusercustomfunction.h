

#ifndef NCREPORTUSERCUSTOMFUNCTION_H
#define NCREPORTUSERCUSTOMFUNCTION_H

#include "ncreportuserfunction.h"

class NCReportItem;
class NCReportOutput;
class NCReportEvaluator;

/*!

section 功能

用户函數功能扩展类
【使程序在运行时可操作扩展函数】

*/
class NCRDUserFunction_getValueWithNullOrEmpty : public NCReportUserFunction
{
public:
    NCRDUserFunction_getValueWithNullOrEmpty() {}
    ~NCRDUserFunction_getValueWithNullOrEmpty() {}

    void updateValue(NCReportItem *item, NCReportOutput *output, NCReportEvaluator *evaluator) override;



};

// todo: 自定义扩展函数添加于此处

#endif // NCREPORTUSERCUSTOMFUNCTION_H
