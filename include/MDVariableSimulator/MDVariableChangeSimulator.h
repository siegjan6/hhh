#ifndef MDVARIABLECHANGESIMULATOR_H
#define MDVARIABLECHANGESIMULATOR_H

#include "MDVariableChangeSimulator_Global.h"
#include <QVariant>
#include "../include/MDVariableHelper/MDVariable.h"

class CMDVariableChangeSimulatorImp;
class MDVARIABLECHANGESIMULATORSHARED_EXPORT CMDVariableChangeSimulator
{
public:
    CMDVariableChangeSimulator();
    ~CMDVariableChangeSimulator();
public:
    bool start(int speed = 1000, int type = 0);
    void stop();
public:
    bool registerVariable(const CMDVariable& variable, quint8 dataChangeMode = 0);
    void unRegisterVariable(const QString& name);
    void unRegisterAllVariable();
public:
    virtual void onChanged(const QString& name, MDVariableDataQuality quality, const QVariant& data);
protected:
    CMDVariableChangeSimulatorImp* m_imp;
};

#endif // MDVARIABLECHANGESIMULATOR_H
