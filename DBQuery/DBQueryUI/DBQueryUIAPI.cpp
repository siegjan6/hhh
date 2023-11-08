#include "MDDBQueryConfigure.h"
#include <QString>
#include "ViewListDialog.h"
#include "ViewFieldsDialog.h"
#include <QDebug>
#include "../include/MDDBQueryUI/MDDBQueryUI.h"
#include "RuntimeFilterDialog.h"
#include "IMDDBQueryEngine.h"

namespace DBQUI
{
/*
bool GetFieldsFromView(const QString & viewName, QList<DBQ::ViewField> & fieldListOut)
{
    QString fullPath = gConfigFilesDir;
    fullPath.append("/view_").append(viewName).append(".xml");

    DBQ::View * pView = 0;
    QString errStr;
    if(!DBQ::LoadView(fullPath, DBQ::FILE_TYPE_JSON, &pView, errStr))
        return false; //TODO:log

    if(!pView)
        return false; //TODO:log

    foreach(DBQ::ViewField * pField, pView->fields)
        fieldListOut.push_back(*pField);

    DBQ::ReleaseView(pView);

}
*/

DBQUERYUI_SHARED_EXPORT QStringList GetViewNameSelected(const QString & viewPath, VIEW_SELECTION_MODE mode/*,QString selectedViewName*/)
{
    ViewListDialog dlg;
    dlg.LoadViews(viewPath);
    //    dlg.selectedCurrentViewName(selectedViewName);
    QStringList viewNames;
    if(dlg.exec() == QDialog::Accepted)
    {
        if(VIEW_SINGLE_SELECTION == mode)
        {
            viewNames = dlg.GetViewNameSelected();
            if(viewNames.size() > 0)
            {
                QString firstView = viewNames.at(0);
                viewNames.clear();
                viewNames.push_back(firstView);
            }
            qDebug()<<viewNames;

        }
        else if(VIEW_MULTIPLE_SELECTION == mode)
        {
            viewNames = dlg.GetViewNameSelected();
        }
    }
    return viewNames;
}

DBQUERYUI_SHARED_EXPORT QList<DBQ::ViewField *> GetFieldsFromViewSelected(const QString & viewPath, const QString & viewName, VIEW_FIELD_RETURN_TYPE returnType)
{
    QList<DBQ::ViewField *> fields;
    ViewFieldsDialog dlg;
    dlg.setReturnFieldType(returnType);
    dlg.LoadView(viewPath,viewName);
    if(dlg.exec() == QDialog::Accepted)
    {
       dlg.GetFields(fields);
    }
    return fields;
}

DBQUERYUI_SHARED_EXPORT void CallStandardRumtimeParmatersConfigUI(IMDDBQueryEngine * pEngine)
{
    if(!pEngine)
        return ; //TODO:log

    RuntimeFilterDialog dlg(pEngine);
    dlg.exec();
    if(dlg.isChanged())
        pEngine->updateAllViewCache();
}
}

