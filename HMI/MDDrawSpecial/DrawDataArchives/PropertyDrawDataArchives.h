#ifndef PROPERTYDRAWDATAARCHIVES_H
#define PROPERTYDRAWDATAARCHIVES_H

#include "MDPropertyDrawObj.h"
class CMDDrawDataArchives;

class CPropertyDrawDataArchives : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString DILconfig READ config WRITE setConfig)
private:
    void setConfig(const QString& value);
    QString config();
public:
    CPropertyDrawDataArchives(CMDDrawDataArchives* parent);
    virtual ~CPropertyDrawDataArchives(){}
protected:
    CMDDrawDataArchives* object();
};

#endif // PROPERTYDRAWDATAARCHIVES_H
