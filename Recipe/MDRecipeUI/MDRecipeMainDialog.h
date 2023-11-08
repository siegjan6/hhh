#ifndef MDRECIPEMAINDIALOG_H
#define MDRECIPEMAINDIALOG_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include "MDObjectPropertyMainDialog.h"

class CMDProjectConfiger;
class CMDRecipeMainDialog : public CMDObjectPropertyMainDialog
{
    Q_OBJECT
public:
    CMDRecipeMainDialog(QWidget *parent = 0);
    ~CMDRecipeMainDialog();
public:
    void setProjectConfiger(CMDProjectConfiger* pConfiger);
protected:
    CMDProjectConfiger* m_pConfiger;
public:
    void setTheOnlyPage(int pageID);
protected:
    int m_pageID;
protected:
    virtual void* newProperty(qint16 subType, void* sourceProperty);
    virtual void initPropertyManagerUI(QListWidget* listWidget, QStackedWidget* stackWidget, void* sourceProperty, void* destProperty);
    virtual bool modifyProperty(void* configer, void* sourceProperty, void* destProperty, QString& resultMessage);
    virtual bool addProperty(void* configer, void* destProperty, QString& resultMessage);
};

#endif // MDRECIPEMAINDIALOG_H
