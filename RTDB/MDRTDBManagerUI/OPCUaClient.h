#ifndef OPCUACLIENT_H
#define OPCUACLIENT_H

#include <map>
#include <vector>
#include <string>

#include "OpcUaGenericClient.hh"

class OPCUaClient : public OPCUA::GenericClient {
public:
        virtual void handleVariableValueUpdate(const std::string &entityName, const OPCUA::Variant &value) override
        {
                //std::cout << "handleVariableValueUpdate(" << entityName << "): " << value << std::endl;
                //spdlog::get("uacpp_logger")->info("handleVariableValueUpdate({0}) : {1}", entityName, value.toString());
        }

        virtual void handleVariableValueUpdates(const std::vector<std::string> &entityNames, const std::vector<OPCUA::Variant> &values) override
        {
                if (entityNames.size() != values.size())
                {
                        //std::cout << "handleVariableValueUpdates() entityNames.size != values.size" << std::endl;
                        //spdlog::get("uacpp_logger")->warn("handleVariableValueUpdates() entityNames.size != values.size");
                        return;
                }
                for (int i = 0; i < entityNames.size(); i++)
                {
                        //std::cout << "handleVariableValueUpdates(" << entityNames[i] << "): " << values[i] << std::endl;
                        //spdlog::get("uacpp_logger")->debug("handleVariableValueUpdates({0}): {1}", entityNames[i], values[i].toString());
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
};

#endif // OPCUACLIENT_H
