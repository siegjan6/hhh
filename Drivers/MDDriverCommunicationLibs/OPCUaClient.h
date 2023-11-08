#ifndef OPCUACLIENT_H
#define OPCUACLIENT_H

#include <map>
#include <vector>
#include <string>

#include "OpcUaGenericClient.hh"
#include <QDebug>
#include <QThread>
#include <QString>
#include "../../include/MDDriverInterfaces/ICommunicationComponent.h"
#include "../../include/interface/Common/IMDDataType.h"

class OPCUaClient : public OPCUA::GenericClient {
public:
        virtual void handleVariableValueUpdate(const std::string &entityName, const OPCUA::Variant &value) override
        {
            //std::cout << "handleVariableValueUpdate(" << entityName << "): " << value << std::endl;
            //spdlog::get("uacpp_logger")->info("handleVariableValueUpdate({0}) : {1}", entityName, value.toString());

            QString strbuf;
            if(value.isScalar())
            {
                QVariant destvar;
                convertOPCVar2QVariant(value, destvar);
                if(callback)
                    callback(QString(entityName.c_str()), destvar, 0, contex);
                //qDebug() << m_entityBrowseNames[i].data() << values[i].toString().data();
                //strbuf.append(QString("%1=%2").arg(QString::fromStdString(entityName)).arg(QString::fromStdString(value.toString())));
                //strbuf.append("|");

//                qDebug() << strbuf;
//                QByteArray ba = strbuf.toLatin1();
//                char* mm = ba.data();
//                if(ba.length() <=65535){
//                    memcpy(rcvBuffer+9, mm, ba.length());
//                    rcvBuffer[2] = 0XFD;
//                    *(quint16*)&rcvBuffer[4] = ba.length();
//                    qDebug() << "var change send length=" << *(quint16*)&rcvBuffer[4];
//                }
            }
        }

        virtual void handleVariableValueUpdates(const std::vector<std::string> &entityNames, const std::vector<OPCUA::Variant> &values) override
        {
            if (entityNames.size() != values.size())
            {
                    //std::cout << "handleVariableValueUpdates() entityNames.size != values.size" << std::endl;
                    //spdlog::get("uacpp_logger")->warn("handleVariableValueUpdates() entityNames.size != values.size");
                    return;
            }
            for (uint i = 0; i < entityNames.size(); i++)
            {
                    //std::cout << "handleVariableValueUpdates(" << entityNames[i] << "): " << values[i] << std::endl;
                    //spdlog::get("uacpp_logger")->debug("handleVariableValueUpdates({0}): {1}", entityNames[i], values[i].toString());
                    handleVariableValueUpdate(entityNames[i], values[i]);
            }
        }

        virtual void handleEvent(const unsigned int &eventId, const std::map<std::string, OPCUA::Variant> &eventData) override
        {
                //std::cout << "handleEvent(" << eventId << "): " << std::endl;
                //spdlog::get("uacpp_logger")->debug("handleEvent({0}):", eventId);
                std::map<std::string, OPCUA::Variant>::const_iterator it;
                for (it = eventData.begin(); it != eventData.end(); ++it) {
                        std::string k = it->first;
                        OPCUA::Variant v = it->second;
                        //std::cout << "k=" << k << " v=" << v << std::endl;
                        //spdlog::get("uacpp_logger")->debug("k={0} v={1}",k,v.toString());
                }
        }

        virtual void handleStatusCallback(const int status) override
        {
            if(callback)
                callback("", QVariant(), status, contex);
        }

        virtual void handleInvokeMethodCallback(const UA_UInt32 &reqId, const std::vector<OPCUA::Variant> &outputs) override
        {
        }

        /*virtual bool createClientSpace(const bool activateUpcalls=true) override
        {
                std::chrono::steady_clock::duration interval = std::chrono::milliseconds(100);
                return addVariableNode("MyVar", activateUpcalls, interval);
        }*/
public:
        OPCUaClient() { }
        virtual ~OPCUaClient() { }

        static void run(OPCUaClient* client) {

            OPCUA::StatusCode status;
            do {
//                std::cout << "run_once()..." <<std::endl;
                status = client->run_once();
                //UA_sleep_ms(100);
                //Sleep(500);
                //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            } while (running == true);
        }

        void thread_run(quint8* pbuf) {
#ifdef _WIN32
            ua_runthread = QThread::create(run, this);
#elif __linux__
            ua_runthread = QThread::create(run,this);
#endif
            rcvBuffer = pbuf;
            ua_runthread->start();
//            std::thread th(run, this);
        }

        void thread_run(subCallbackFunc cb, void* ctx) {
#ifdef _WIN32
            ua_runthread = QThread::create(run, this);
#elif __linux__
            ua_runthread = QThread::create(run,this);
#endif
            callback = cb;
            contex = ctx;
            ua_runthread->start();
//            std::thread th(run, this);
        }

        void thread_stop() {
            running = false;
            if(ua_runthread)
                ua_runthread->wait(3000);
        }


        void convertOPCVar2QVariant(const OPCUA::Variant &src, QVariant &dest) {
            if (src.isEmpty()) {
                return;
            }

            int typeIndex = src.getTypeIndex();
            //std::cout << "OPCUA::typeIndex=" << typeIndex << std::endl;
            //OS::get_singleton()->print("convertOPCVar2GDVar OPCUA::typeIndex=%d\n", typeIndex);
            if (src.isScalar()) {
                //std::cout << "typeIndex: " << typeIndex << std::endl;
                if (typeIndex == UA_TYPES_BOOLEAN) {
                    auto boolValue = src.getValueAs<bool>();
                    dest = QVariant(boolValue);
                } else if (typeIndex == UA_TYPES_SBYTE) {
                    // all numeric types that fit into 32 bit
                    auto intValue = src.getValueAs<int16_t>();
                    //std::cout << "intValue: " << intValue << std::endl;
                    dest = QVariant(intValue);
                } else if (typeIndex == UA_TYPES_BYTE) {
                    // all numeric types that fit into 32 bit
                    auto intValue = src.getValueAs<uint16_t>();
                    //std::cout << "intValue: " << intValue << std::endl;
                    dest = QVariant(intValue);
                } else if (typeIndex == UA_TYPES_INT16) {
                    // all numeric types that fit into 32 bit
                    auto intValue = src.getValueAs<int16_t>();
                    //std::cout << "intValue: " << intValue << std::endl;
                    dest = QVariant(intValue);
                } else if (typeIndex == UA_TYPES_UINT16) {
                    // all numeric types that fit into 32 bit
                    auto intValue = src.getValueAs<uint16_t>();
                    //std::cout << "intValue: " << intValue << std::endl;
                    dest = QVariant(intValue);
                } else if (typeIndex == UA_TYPES_INT32) {
                    // all numeric types that fit into 32 bit
                    auto intValue = src.getValueAs<int32_t>();
                    //std::cout << "intValue: " << intValue << std::endl;
                    dest = QVariant(intValue);
                } else if (typeIndex == UA_TYPES_UINT32) {
                    auto uintValue = src.getValueAs<uint32_t>();
                    dest = QVariant(uintValue);
                } else if (typeIndex == UA_TYPES_INT64) {
                    auto intValue = src.getValueAs<int64_t>();
                    dest = QVariant(intValue);
                } else if (typeIndex == UA_TYPES_UINT64) {
                    auto uintValue = src.getValueAs<uint64_t>();
                    dest = QVariant(uintValue);
                } else if (typeIndex <= UA_TYPES_FLOAT) {
                    // floating types
                    auto floatValue = src.getValueAs<float>();
                    dest = QVariant(floatValue);
                } else if (typeIndex <= UA_TYPES_DOUBLE) {
                    // double types
                    auto dblValue = src.getValueAs<double>();
                    dest = QVariant(dblValue);
                } else if (typeIndex == UA_TYPES_STRING) {
                    // case 1:
                    UA_String *uaStringPtr = static_cast<UA_String *>(src.getInternalValuePtr()->data);

                    // case 2:
                    //UA_String *uaStringPtr = static_cast<UA_String *>(src.getInternalValueCopy().data);

                    // case 3:
                    //std::string s = src.toString();
                    //std::cout << "OPCUA::src.tostring()=" << s1 << std::endl;

                    // reinterpret cast is quite a sledge hammer here
                    std::string s(reinterpret_cast<const char *>(uaStringPtr->data), uaStringPtr->length);
                    //std::cout << "uastring to std::string=" << s << std::endl;
                    dest = QVariant(s.c_str());
                } else if (typeIndex == UA_TYPES_QUALIFIEDNAME) {
                    UA_QualifiedName *uaQname = static_cast<UA_QualifiedName *>(src.getInternalValuePtr()->data);
                    std::string index = std::to_string(uaQname->namespaceIndex);
                    std::string simple_name(reinterpret_cast<const char *>(uaQname->name.data), uaQname->name.length);
                    std::string s = index + ":" + simple_name;
                    dest = QVariant(s.c_str());

                } else if (typeIndex == UA_TYPES_LOCALIZEDTEXT) {
                    UA_LocalizedText *uaText = static_cast<UA_LocalizedText *>(src.getInternalValuePtr()->data);
                    // reinterpret cast is quite a sledge hammer here
                    std::string s = std::string(reinterpret_cast<const char *>(uaText->text.data), uaText->text.length);
                    dest = QVariant(s.c_str());
                } else if (typeIndex == UA_TYPES_DATETIME) {
                    UA_DateTime now = UA_DateTime_now();
                    //std::cout << "now=" << now << std::endl;
                    auto raw_date = src.getValueAs<UA_DateTime>();
                    //std::cout << "dtUTCValue=" << raw_date << std::endl;
                    UA_Int64 tOffset = UA_DateTime_localTimeUtcOffset();
                    UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date + tOffset);
                    /*printf("date is: %04u-%02u-%02u %02u:%02u:%02u.%03u",
                            dts.year, dts.month, dts.day, dts.hour, dts.min, dts.sec, dts.milliSec);*/
                    //UA_DateTime dt = UA_DateTime_fromUnixTime(raw_date);
                    UA_DateTime dtlocal = UA_DateTime_fromStruct(dts);
                    //printf("%Id-%lld-%I64d", dtlocal, dtlocal, dtlocal);
                    UA_Int64 unix_time = UA_DateTime_toUnixTime(dtlocal);

                    // UA_DateTime_now()加上偏移值，得到正确的系统时间
                    //UA_DateTimeStruct dts = UA_DateTime_toStruct(dtUTCValue + tOffset);

                    dest = QVariant(unix_time);
                } else if (typeIndex == UA_TYPES_BYTESTRING) {
                    //std::cout << "UA_TYPES_BYTESTRING" << std::endl;
                    UA_ByteString *uaByteStringPtr = static_cast<UA_ByteString *>(src.getInternalValuePtr()->data);

                    // case1: base64
                    size_t size = UA_calcSizeBinary(uaByteStringPtr, &UA_TYPES[UA_TYPES_BYTESTRING]);
                    //std::cout << "ua bytestring length=" << size << std::endl;
                    //OS::get_singleton()->print("ua bytestring length=%d\n", size);
                    UA_String *uaString = new UA_String;
                    //UA_StatusCode rv = UA_ByteString_toBase64(uaByteStringPtr, uaString);
                    //std::cout << "UA_ByteString_toBase64 rv=" << rv << std::endl;
                    //if (rv == UA_STATUSCODE_GOOD) {
                    UA_StatusCode rv = UA_decodeBinary(uaByteStringPtr, uaString, &UA_TYPES[UA_TYPES_STRING], NULL);
                    //std::cout << "UA_decodeBinary rv=" << rv << std::endl;
                    //OS::get_singleton()->print("UA_decodeBinary rv=%d\n", rv);
                    //UA_String_decodeBinary(uaByteStringPtr, 0, &uaString);

                    std::string s(reinterpret_cast<const char *>(uaString->data), uaString->length);
                    dest = QVariant(s.c_str());

                    //}

                    // case2: string
                    //std::string s(reinterpret_cast<const char *>(uaByteStringPtr->data), uaByteStringPtr->length);
                    ////std::cout << "uastring to std::string=" << s << std::endl;
                    //dest = Variant(s.c_str());

                    //UA_ByteString myByteString = UA_BYTESTRING(reinterpret_cast<const char *>(uaStringPtr->data));
                } else if (typeIndex == UA_TYPES_NODEID) {
                    UA_NodeId *uaNodeIdPtr = static_cast<UA_NodeId *>(src.getInternalValuePtr()->data);

                    // case 1: 按照类型转换
                    if (uaNodeIdPtr->identifierType == UA_NODEIDTYPE_STRING) {
                        std::string s((const char *)uaNodeIdPtr->identifier.string.data, uaNodeIdPtr->identifier.string.length);
                        dest = QVariant(s.c_str());
                    } else if (uaNodeIdPtr->identifierType == UA_NODEIDTYPE_NUMERIC) {
                        dest = QVariant(uaNodeIdPtr->identifier.numeric);
                    }

                    // case 2: 强制字符串转换
                    //UA_String uaString;
                    //UA_StatusCode rv = UA_NodeId_print(uaNodeIdPtr, &uaString);
                    //std::string s(reinterpret_cast<const char *>(uaString.data), uaString.length);
                    //dest = Variant(s.c_str());
                }
            } else {
                // is array type
                //std::string result;
                //if( typeIndex == UA_TYPES_BOOLEAN) {
                //	auto boolValues = getArrayValuesAs<bool>();
                //	for(size_t i=0; i<boolValues.size(); ++i) {
                //		if(boolValues[i] == true) {
                //			result = result + std::string("true");
                //		} else {
                //			result = result + std::string("false");
                //		}
                //		if(i < boolValues.size()-1) {
                //			result = result + ", ";
                //		}
                //	}
                //} else if( typeIndex <= UA_TYPES_INT32 ) {
                //	// all numeric types that fit into 32 bit
                //	auto intValues = getArrayValuesAs<int>();
                //	for(auto &ival: intValues) {
                //		result = result + std::to_string(ival);
                //		if(&ival != &intValues.back()) {
                //			result = result + ", ";
                //		}
                //	}
                //} else if( typeIndex == UA_TYPES_UINT32 ) {
                //	auto uintValues = getArrayValuesAs<unsigned int>();
                //	for(auto &uival: uintValues) {
                //		result = result + std::to_string(uival);
                //		if(&uival != &uintValues.back()) {
                //			result = result + ", ";
                //		}
                //	}
                //} else if( typeIndex == UA_TYPES_INT64 ) {
                //	auto intValues = getArrayValuesAs<long int>();
                //	for(auto &ival: intValues) {
                //		result = result + std::to_string(ival);
                //		if(&ival != &intValues.back()) {
                //			result = result + ", ";
                //		}
                //	}
                //} else if( typeIndex == UA_TYPES_UINT64 ) {
                //	auto uintValues = getArrayValuesAs<unsigned long int>();
                //	for(auto &uival: uintValues) {
                //		result = result + std::to_string(uival);
                //		if(&uival != &uintValues.back()) {
                //			result = result + ", ";
                //		}
                //	}
                //} else if( typeIndex <= UA_TYPES_DOUBLE ) {
                //	// floating types
                //	auto dblValues = getArrayValuesAs<double>();
                //	for(auto &dblval: dblValues) {
                //		result = result + std::to_string(dblval);
                //		if(&dblval != &dblValues.back()) {
                //			result = result + ", ";
                //		}
                //	}
                //} else if(typeIndex == UA_TYPES_STRING) {
                //	auto strValues = getArrayValuesAs<std::string>();
                //	for(auto &str: strValues) {
                //		result = result + str;
                //		if(&str != &strValues.back()) {
                //			result = result + ", ";
                //		}
                //	}
                //}
                //return result;
            }

            //return std::string();
        }

//        Bool = 0,
//        SByte = 1,
//        Byte = 2,
//        Int16 = 3,
//        UInt16 = 4,
//        Int32 = 5,
//        UInt32 = 6,
//        Int64 = 7,
//        UInt64 = 8,
//        Float = 9,
//        Double = 10,
//        DateTime = 11,
//        String = 12,
        void convertQVariant2OPCVar(const QVariant &src, MDDataType type, OPCUA::Variant &dest) {

            if (type == MDDataType::Bool) {
                dest.setValueFrom<bool>(src.toBool());
            } else if (type == MDDataType::SByte) {
                dest.setValueFrom<int8_t>(src.toChar().toLatin1());
            } else if (type == MDDataType::Byte) {
                dest.setValueFrom<uint8_t>(src.toChar().toLatin1());
            } else if (type == MDDataType::Int16) {
                dest.setValueFrom<int16_t>(src.toInt());
            } else if (type == MDDataType::UInt16) {
                dest.setValueFrom<uint16_t>(src.toUInt());
            } else if (type == MDDataType::Int32) {
                dest.setValueFrom<int32_t>(src.toInt());
            } else if (type == MDDataType::UInt32) {
                dest.setValueFrom<uint32_t>(src.toUInt());
            } else if (type == MDDataType::Int64) {
                dest.setValueFrom<int64_t>(src.toLongLong());
            } else if (type == MDDataType::UInt64) {
                dest.setValueFrom<uint64_t>(src.toULongLong());
            } else if (type == MDDataType::Float) {
                dest.setValueFrom<float>(src.toFloat());
            } else if (type == MDDataType::Double) {
                dest.setValueFrom<double>(src.toDouble());
            } else if (type == MDDataType::DateTime) {
//                UA_DateTime now = UA_DateTime_now();
//                //std::cout << "now=" << now << std::endl;
//                auto raw_date = src.getValueAs<UA_DateTime>();
//                //std::cout << "dtUTCValue=" << raw_date << std::endl;
//                UA_Int64 tOffset = UA_DateTime_localTimeUtcOffset();
//                UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date + tOffset);
//                /*printf("date is: %04u-%02u-%02u %02u:%02u:%02u.%03u",
//                        dts.year, dts.month, dts.day, dts.hour, dts.min, dts.sec, dts.milliSec);*/
//                //UA_DateTime dt = UA_DateTime_fromUnixTime(raw_date);
//                UA_DateTime dtlocal = UA_DateTime_fromStruct(dts);
//                //printf("%Id-%lld-%I64d", dtlocal, dtlocal, dtlocal);
//                UA_Int64 unix_time = UA_DateTime_toUnixTime(dtlocal);

//                // UA_DateTime_now()加上偏移值，得到正确的系统时间
//                //UA_DateTimeStruct dts = UA_DateTime_toStruct(dtUTCValue + tOffset);

//                dest = QVariant(unix_time);
            } else if (type == MDDataType::String) {
                dest.setValueFrom<std::string>(src.toString().toStdString());
            }
        }

        void asyncWriteVariable(QString address, MDDataType type, QVariant value){
            OPCUA::Variant destVar;
            convertQVariant2OPCVar(value, type, destVar);
            //writeValue(address.toStdString(),destVar);
            writeValue_async(address.toStdString(),destVar);
        }

        void asyncWriteVariables(QList<QString> addresses, QList<MDDataType> types, QList<QVariant> values){
            if(addresses.length() != values.length() != types.length())
                return;

            std::vector<std::string> nodePaths;
            std::vector<OPCUA::Variant> nodeValues;
            for (int i = 0;i<addresses.length();) {
                std::string address = addresses[i].toStdString();
                OPCUA::Variant destVar;
                MDDataType type = types[i];
                QVariant value = values[i];
                convertQVariant2OPCVar(value, type, destVar);
                nodePaths.push_back(address);
                nodeValues.push_back(destVar);
                writeValues_async(nodePaths, nodeValues);
            }
        }

private:
        static bool running;
        static quint8* rcvBuffer;
        static void* contex;
        static subCallbackFunc callback;

        QThread* ua_runthread = nullptr;
};

bool OPCUaClient::running = true;
quint8* OPCUaClient::rcvBuffer = nullptr;
void* OPCUaClient::contex = nullptr;
subCallbackFunc OPCUaClient::callback = nullptr;

#endif // OPCUACLIENT_H
