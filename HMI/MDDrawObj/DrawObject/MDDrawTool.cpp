#include "MDDrawTool.h"

CMDDrawTool* CMDDrawTool::m_drawTool = NULL;
//////////////////////////////////////////////////////////////////////////
CMDDrawTool::~CMDDrawTool()
{
    delete m_create;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawTool::init(ICreateTool *create)
{
   if(m_drawTool != NULL)
        return;

   m_drawTool = new CMDDrawTool();
   m_drawTool->m_create = create;
}
void CMDDrawTool::release()
{
    if(m_drawTool != NULL)
        delete m_drawTool;
}
CMDDrawTool *CMDDrawTool::instance()
{
    Q_ASSERT(m_drawTool);
    return m_drawTool;
}
//////////////////////////////////////////////////////////////////////////



