#include "MDPropertyDrawFlod.h"
#include "MDDrawFoldLine.h"
#include"MDNodeStream.h"
CMDPropertyDrawFlod::CMDPropertyDrawFlod(CMDDrawObj *parent):
    CMDPropertyDrawNode(parent)
{
}

CMDPropertyDrawFlod::~CMDPropertyDrawFlod()
{
}

QString CMDPropertyDrawFlod::DILconfig()
{
    return "";
}

void CMDPropertyDrawFlod::setDILconfig(const QString &stream)
{
        Q_UNUSED(stream);
        if(m_streamDlg == nullptr)
        {
            m_streamDlg = new StreamDlg();
            m_streamDlg->setObject(object()->m_pLineStream);
            connect(m_streamDlg,&StreamDlg::changeValue,
                    [=](QString name){emit DILstreamChanged(name);});
        }
        m_streamDlg->exec();
}

CMDDrawFoldLine *CMDPropertyDrawFlod::object()
{
    return dynamic_cast<CMDDrawFoldLine*>(m_object);
}
