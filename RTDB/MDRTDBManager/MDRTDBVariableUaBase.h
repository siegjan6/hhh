/*!
author enli 2015.01.08
\section 实时数据库内部对象
*/
#ifndef CMDRTDBVARIABLEBASE_H
#define CMDRTDBVARIABLEBASE_H

#include <QList>
#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QDomElement>
#include "IMDRTDBVariable.h"
#include "MDRTDBVariable.h"
#include "IMDTreeItem.h"
#include "MDRTDBManager.h"
#include "MDExpression.h"
#include "MDActionSP.h"
#include "SortJsonObject.h"
#include <QUaServer>
#include <QUaProperty>
#include <QUaBaseObject>
#include <QUaBaseDataVariable>


class CMDVariableGroup;
//实时数据库变量
class CMDRTDBVariable: public QUaBaseObject, public CMDRTDBVariableT
{
	Q_OBJECT

	// UA properties
	Q_PROPERTY(QUaProperty * Type              READ type);
	/*Q_PROPERTY(QUaProperty * RegistersUsed     READ registersUsed)
	Q_PROPERTY(QUaProperty * AddressOffset     READ addressOffset)*/
	Q_PROPERTY(QUaProperty * Unit READ unit)
	Q_PROPERTY(QUaProperty * Description READ description)
    Q_PROPERTY(QUaProperty * Ext READ ext)
    Q_PROPERTY(QUaProperty * Precision READ precision)
    Q_PROPERTY(QUaProperty * HL READ hl)
    Q_PROPERTY(QUaProperty * LL READ ll)

	// UA variables
	Q_PROPERTY(QUaBaseDataVariable * Value     READ value		WRITE setValue)
	Q_PROPERTY(QUaBaseDataVariable * LastError READ lastError)
	
public:
    Q_INVOKABLE explicit CMDRTDBVariable(QUaServer *server=nullptr);

public:
    //CMDRTDBVariable();
    virtual ~CMDRTDBVariable();

/*!
\section UA 接口
*/
public:
	// UA properties

	QUaProperty * type() const;
	QUaProperty * unit() const;
	QUaProperty * description() const;
    QUaProperty * ext() const;
    QUaProperty * precision() const;
    QUaProperty * hl() const;
    QUaProperty * ll() const;
	/*QUaProperty * registersUsed() const;
	QUaProperty * addressOffset() const;*/

	// UA variables

	QUaBaseDataVariable * value() const;
	void setValue(const QUaBaseDataVariable *value);
	void OnValueChanged(const QVariant &value, const bool m_bInternalWrite);
	QUaBaseDataVariable * lastError() const;

	enum ValueType
	{
		Bool = 0,
		SByte = 1,
		Byte = 2,
		Int16 = 3,
		UInt16 = 4,
		Int32 = 5,
		UInt32 = 6,
		Int64 = 7,
		UInt64 = 8,
		Float = 9,
		Double = 10,
		DateTime = 11,
		String = 12,
	};
	Q_ENUM(ValueType)
    typedef CMDRTDBVariable::ValueType MDDataValueType;

	// C++ API
    MDDataValueType getType() const;
    void setType(const MDDataValueType &type);
	QMetaType::Type dataType2MetaType();
	
	QString getUnit();
	void setUnit(QString unit);

	QString getDescription();
	void setDescription(QString description);

    QString getExt();
    void setExt(QString ext);

    int getPrecision();
    void setPrecision(int precision);

	QVariant getValue() const;
    void setValue(const QVariant &value);

    QVariant getHL() const;
    void setHL(const QVariant &value);

    QVariant getLL() const;
    void setLL(const QVariant &value);

	void removeFromUa();

    void* uaBaseDataVariable();
    void setUaBaseDataVariable(void* pUaBaseDataVariable);

public:
    void deserialize(const QJsonObject &json) override;
    //bool writeValue(QVariant varValue) override;
    bool refreshData(QVariant dataValue,QDateTime TimeStamp,MDVariableDataQuality quality,bool OnlyChangedQuality=false) override;

private:
    bool m_firstWrite;
    void* m_pTempUaBaseDataVariable;
};

typedef CMDRTDBVariable::ValueType MDDataValueType;


//class TemperatureSensor : public QUaBaseObject
//{
//    Q_OBJECT
//    // properties
//    Q_PROPERTY(QUaProperty * model READ model)
//    Q_PROPERTY(QUaProperty * brand READ brand)
//    Q_PROPERTY(QUaProperty * units READ units)
//    // variables
//    Q_PROPERTY(QUaBaseDataVariable * status       READ status      )
//    Q_PROPERTY(QUaBaseDataVariable * currentValue READ currentValue)
//public:
//    Q_INVOKABLE explicit TemperatureSensor(QUaServer *server=nullptr);

//    enum Units
//    {
//        C = 0,
//        F = 1
//    };
//    Q_ENUM(Units)
////    typedef TemperatureSensor::Units MDUnitsType;

//    QUaProperty * model();
//    QUaProperty * brand();
//    QUaProperty * units();

//    QUaBaseDataVariable * status      ();
//    QUaBaseDataVariable * currentValue();

//    Q_INVOKABLE void turnOn();
//    Q_INVOKABLE void turnOff();

//    Q_INVOKABLE void remove();

//};

//typedef TemperatureSensor::Units MDUnitsType;


#endif // CMDRTDBVARIABLEBASE_H
