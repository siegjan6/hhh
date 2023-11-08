#include "MDRTDBVariableUaBase.h"
#include "MDChangeVariable.h"
#include "MDVariableGroup.h"
#include "MDVariableDataConverter.h"
#include <QtCore/qmath.h>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

CMDRTDBVariable::CMDRTDBVariable(QUaServer *server)
    : QUaBaseObject(server), CMDRTDBVariableT()
{
    m_firstWrite = false;
    m_pTempUaBaseDataVariable = NULL;
    type()->setDataTypeEnum(QMetaEnum::fromType<MDDataValueType>());
    value()->setWriteAccess(true);

    connect(value(), &QUaBaseDataVariable::valueChanged, this, &CMDRTDBVariable::OnValueChanged);
}

CMDRTDBVariable::~CMDRTDBVariable()
{

}

void CMDRTDBVariable::OnValueChanged(const QVariant &value, const bool m_bInternalWrite)
{
	/*if (m_rtdbManager->getSwitchState(MDLogicSwitchType::Log))
	{
		QString str = QString("write var(fromClient:%1): %2=%3").arg(m_bInternalWrite).arg(name()).arg(value.toString());
		m_rtdbManager->m_logSwitch->LOG_INFO(str);
	}*/
    //qDebug() << "OnValueChanged New value:" << value << "m_bInternalWrite: " << m_bInternalWrite;
	if (m_bInternalWrite)
	{
		// write request from ua client
		// need to write to actual variable
		writeValue(value);
	}
}

QUaProperty * CMDRTDBVariable::type() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Type");
}

QUaProperty * CMDRTDBVariable::unit() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Unit");
}

QUaProperty * CMDRTDBVariable::description() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Description");
}

QUaProperty * CMDRTDBVariable::ext() const
{
    return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Ext");
}

QUaProperty * CMDRTDBVariable::precision() const
{
    return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Precision");
}

QUaBaseDataVariable * CMDRTDBVariable::value() const
{
    QUaBaseDataVariable* v = const_cast<CMDRTDBVariable*>(this)->browseChild<QUaBaseDataVariable>("Value");
    //v->setValue(getValue());
    return v;
}

QUaBaseDataVariable * CMDRTDBVariable::lastError() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaBaseDataVariable>("LastError");
}

QUaProperty * CMDRTDBVariable::hl() const
{
    return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("HL");
}

QUaProperty * CMDRTDBVariable::ll() const
{
    return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("LL");
}

void CMDRTDBVariable::deserialize(const QJsonObject &json)
{
    CMDRTDBVariableT::deserialize(json);

	// uaserver
    MDDataValueType mdDataType = (MDDataValueType)json.value("@DT").toString("3").toInt();
    setType(mdDataType);
	setUnit(m_unit);
	setDescription(m_description);
    setExt(m_ext);
    setPrecision(m_precision);
    //if(value()->value().isNull())
//    {
//        if(mdDataType == MDDataValueType::String)
//            this->value()->setValue(QVariant("0"));
//        else if(mdDataType == MDDataValueType::Float || mdDataType == MDDataValueType::Double)
//            this->value()->setValue(QVariant(0.0f));
//        else if(mdDataType == MDDataValueType::DateTime){
//            qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
//            this->value()->setValue(QVariant(timestamp));
//        }
//        else
//            this->value()->setValue(QVariant(0));
//    }
    if(m_writeProtectParameter.m_enableWrite)
    {
        this->value()->setWriteAccess(m_writeProtectParameter.m_enableWrite);
        if(m_writeProtectParameter.m_enableWriteProtect)
        {
            setHL(m_writeProtectParameter.m_maxValue.toDouble());
            setLL(m_writeProtectParameter.m_minValue.toDouble());
        }
    }
}

// uaserver
MDDataValueType CMDRTDBVariable::getType() const
{
    return this->type()->value().value<MDDataValueType>();
}

void CMDRTDBVariable::setType(const MDDataValueType & type)
{
	if (this->getType() == type)
	{
		return;
	}
	this->type()->setValue(type);
	//this->on_typeChanged(type, true);
}

QString CMDRTDBVariable::getUnit()
{
	return this->unit()->value().value<QString>();
}

void CMDRTDBVariable::setUnit(QString unit)
{
	/*if (this->getUnit() == unit)
	{
		return;
	}*/
	this->unit()->setValue(unit);
	m_unit = unit;
	//this->on_unitChanged(unit, true);
}

QString CMDRTDBVariable::getDescription()
{
    return m_description;
    //return this->description()->value().value<QString>();
}

void CMDRTDBVariable::setDescription(QString description)
{
	/*if (this->getDescription() == description)
	{
		return;
	}*/
	this->description()->setValue(description);
	m_description = description;
	//this->on_descriptionChanged(description, true);
}

QString CMDRTDBVariable::getExt()
{
    return m_ext;
}

void CMDRTDBVariable::setExt(QString ext)
{
    this->ext()->setValue(ext);
    m_ext = ext;
}

int CMDRTDBVariable::getPrecision()
{
    return this->precision()->value().value<int>();
}

void CMDRTDBVariable::setPrecision(int precision)
{
    /*if (this->getPrecision() == precision)
    {
        return;
    }*/
    this->precision()->setValue(precision);
    m_precision = precision;
    //this->on_precisionChanged(precision, true);
}

QVariant CMDRTDBVariable::getValue() const
{
    //return (CMDRTDBVariableT*)(this)->currentValue();
    //if(m_dataType == MDDataType::String && this->value()->dataType()==NULL)
    //    const_cast<CMDRTDBVariable*>(this)->value()->setDataType(dataType2MetaType());
    return const_cast<CMDRTDBVariable*>(this)->currentValue();
    //return this->value()->value();
}

void CMDRTDBVariable::setValue(const QVariant & value)
{
    //this->value()->setDataTypeEnum(QMetaEnum::fromType<CMDRTDBVariable::MDDataValueType>());
    if (!value.isValid() || value.isNull())
        return;
    QVariant newValue = value;
    QMetaType::Type tp = dataType2MetaType();
    if (newValue.canConvert(tp)) {
        this->value()->setDataType(tp);
        this->value()->setValue(value);
    }
    else {
        CMDRTDBManager* vm = getRTDBManager();
        if(vm){
            if (vm->getSwitchState(MDLogicSwitchType::Log))
            {
                QString str = QString("CMDRTDBVariable.setValue(from:[%1] to [%2] ,from type: [%3] to [%4])").arg(getValue().toString()).arg(value.toString()).arg(getType()).arg(tp);
                vm->m_logSwitch->LOG_ERROR(str);
            }
        }
    }


	//this->on_valueChanged(value, true);
}


QVariant CMDRTDBVariable::getHL() const
{
    return this->hl()->value();
}

void CMDRTDBVariable::setHL(const QVariant & value)
{
    this->hl()->setDataType(dataType2MetaType());
    this->hl()->setValue(value);
}

QVariant CMDRTDBVariable::getLL() const
{
    return this->ll()->value();
}

void CMDRTDBVariable::setLL(const QVariant & value)
{
    this->ll()->setDataType(dataType2MetaType());
    this->ll()->setValue(value);
}

//#include <QMessageBox>
void CMDRTDBVariable::setValue(const QUaBaseDataVariable *value)
{
	//QMessageBox::information(0,"", value->value().toString());
}

void CMDRTDBVariable::removeFromUa()
{
	/*QUaBaseObject* obj = (QUaBaseObject*)vartemp;
	if (obj)
		obj->*/
		//removeBaseObject();
	/*if ((QUaBaseObject*)this)
		delete (QUaBaseObject*)this;*/
}

QMetaType::Type CMDRTDBVariable::dataType2MetaType()
{
	QMetaType::Type ret;
	switch (m_dataType)
	{
	case MDDataType::Bool:
		ret = QMetaType::Bool;
		break;
	case MDDataType::SByte:
		ret = QMetaType::Char;
		break;
	case MDDataType::Byte:
		ret = QMetaType::UChar;
		break;
	case MDDataType::Int16:
		ret = QMetaType::Short;
		break;
	case MDDataType::UInt16:
		ret = QMetaType::UShort;
		break;
	case MDDataType::Int32:
		ret = QMetaType::Int;
		break;
	case MDDataType::UInt32:
		ret = QMetaType::UInt;
		break;
	case MDDataType::Int64:
		ret = QMetaType::Long;
		break;
	case MDDataType::UInt64:
		ret = QMetaType::ULong;
		break;
	case MDDataType::Float:
		ret = QMetaType::Float;
		break;
	case MDDataType::Double:
		ret = QMetaType::Double;
		break;
	case MDDataType::DateTime:
		ret = QMetaType::QDateTime;
		break;
	case MDDataType::String:
		ret = QMetaType::QString;
		break;
	default:
		break;
	}
	return ret;
}

//bool CMDRTDBVariable::writeValue(QVariant varValue)
//{
//    if(!m_firstWrite) {
//        setValue(varValue);
//        m_firstWrite = true;
//        qDebug() << "first write " << name() << varValue;
//    }
//    return CMDRTDBVariableT::writeValue(varValue);
//}

bool CMDRTDBVariable::refreshData(QVariant dataValue,QDateTime TimeStamp,MDVariableDataQuality quality,bool OnlyChangedQuality)
{
    if(!m_firstWrite && dataValue.isValid()) {
        setValue(dataValue);
        const_cast<CMDRTDBVariable*>(this)->browseChild<QUaBaseDataVariable>("LastError")->setValue(0);
        m_firstWrite = true;
        //qDebug() << "first refreshData " << name() << dataValue;
    }
    return CMDRTDBVariableT::refreshData(dataValue,TimeStamp,quality,OnlyChangedQuality);
}

void* CMDRTDBVariable::uaBaseDataVariable() {
    if(m_pTempUaBaseDataVariable)
        return m_pTempUaBaseDataVariable;
    return value();
}
void CMDRTDBVariable::setUaBaseDataVariable(void* pUaBaseDataVariable)
{
    m_pTempUaBaseDataVariable = pUaBaseDataVariable;
}


//TemperatureSensor::TemperatureSensor(QUaServer *server)
//    : QUaBaseObject(server)
//{
//    // set defaults
//    model()->setValue("TM35");
//    brand()->setValue("Texas Instruments");
//    units()->setDataTypeEnum(QMetaEnum::fromType<TemperatureSensor::Units>());
//    units()->setValue(Units::C);
//    status()->setValue("Off");
//    currentValue()->setValue(0.0);
//    currentValue()->setDataType(QMetaType::Double);
//}

//QUaProperty * TemperatureSensor::model()
//{
//    return this->browseChild<QUaProperty>("model");
//}

//QUaProperty * TemperatureSensor::brand()
//{
//    return this->browseChild<QUaProperty>("brand");
//}

//QUaProperty * TemperatureSensor::units()
//{
//    return this->browseChild<QUaProperty>("units");
//}

//QUaBaseDataVariable * TemperatureSensor::status()
//{
//    return this->browseChild<QUaBaseDataVariable>("status");
//}

//QUaBaseDataVariable * TemperatureSensor::currentValue()
//{
//    return this->browseChild<QUaBaseDataVariable>("currentValue");
//}

//void TemperatureSensor::turnOn()
//{
//    status()->setValue("On");
//}

//void TemperatureSensor::turnOff()
//{
//    status()->setValue("Off");
//}

//void TemperatureSensor::remove()
//{
//    this->deleteLater();
//}
