/*!

\author emhhbmdsZQ==

\section 功能
    批量操作导出类
*/

#ifndef MDOBJBATCHOPERATIONEXPERT_H
#define MDOBJBATCHOPERATIONEXPERT_H
#include <QtCore/qglobal.h>
#include <QJsonObject>
#include <QObject>
#if defined(MDOBJBATCHOPERATIONEXPERT_LIBRARY)
#  define MDOBJBATCHOPERATIONEXPERT_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDOBJBATCHOPERATIONEXPERT_SHARED_EXPORT Q_DECL_IMPORT
#endif

class CMDBatchAddWidget;
class MDOBJBATCHOPERATIONEXPERT_SHARED_EXPORT CMDObjBatchOperationExpert:public QObject
{

    Q_OBJECT
public:
    CMDObjBatchOperationExpert();
    ~CMDObjBatchOperationExpert();

public:
    void setRefJson(const QJsonObject& json);
    void setErrorInfor(const QString& str);

signals:
    void newJsonCreate(const QJsonObject &json);

private:
    CMDBatchAddWidget *m_batchAddWidget;

};

#endif // MDOBJBATCHOPERATIONEXPERT_H
