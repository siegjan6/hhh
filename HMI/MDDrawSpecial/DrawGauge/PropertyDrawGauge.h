#ifndef PROPERTYDRAWGAUGE_H
#define PROPERTYDRAWGAUGE_H
//////////////////////////////////////////////////////////////////////////
#include "MDPropertyDrawVector.h"

class CMDDrawGauge;
//////////////////////////////////////////////////////////////////////////
class CPropertyDrawGauge : public CMDPropertyDrawVector
{
    Q_OBJECT
//////////////////////////////////////////////////////////////////////////
    Q_PROPERTY(QString GUID_Appearance)
    Q_PROPERTY(QString DILconfig READ config WRITE setConfig)

    Q_PROPERTY(QString $DILbrush)
    Q_PROPERTY(QString $DILline)
    Q_PROPERTY(QString $DILdynamicBrush)
    Q_PROPERTY(QString $DILdynamicLine)
    Q_PROPERTY(QString $DILfill)
//////////////////////////////////////////////////////////////////////////
public:
    CPropertyDrawGauge(CMDDrawObj *parent);
    virtual ~CPropertyDrawGauge(){}
protected:
    CMDDrawGauge* object();
//////////////////////////////////////////////////////////////////////////
private:
    QString config();
    void setConfig(const QString& value);
//////////////////////////////////////////////////////////////////////////
};

#endif // PROPERTYDRAWGAUGE_H
