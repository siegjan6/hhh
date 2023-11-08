#include "webobjectinstance.h"
#include <QMessageBox>

WebObjectInstance *WebObjectInstance::instance()
{
    static WebObjectInstance obj;
    return &obj;
}

WebObjectInstance::WebObjectInstance(QObject *parent) : QObject(parent)
{

}

void WebObjectInstance::slotCplusplusUpdate(const QVariant &var)
{

}


