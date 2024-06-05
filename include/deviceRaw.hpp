#pragma once
#include <string>
#include <map>
struct deviceRaw
{
    int m_port;
    std::string m_deviceName;
    std::string m_path;
    enum class EInterfaceType : uint8_t
    {
        COM,
        MAC,
        UDP
    };
    std::map<std::string, EInterfaceType> StringToEnum{{"COM", EInterfaceType::COM}, {"MAC", EInterfaceType::MAC}, {"UDP", EInterfaceType::UDP}};
    EInterfaceType m_interface;
    deviceRaw() = default;
    ~deviceRaw() = default;
};