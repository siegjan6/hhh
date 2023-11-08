#include "CreateTool.h"
#include "MDHMIForm.h"

CMDDrawObj *CCreateTool::createDrawObj(DrawType type)
{
    return CMDHMIForm::createDrawObj(type);
}
