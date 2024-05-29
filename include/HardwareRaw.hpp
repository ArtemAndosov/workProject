#pragma once
#include <string>
struct hardwareRaw
{
    std::string m_direction;
    std::string m_eventName;
    std::string m_deviceName;
    std::string m_address;
    std::string m_port;
    std::string m_parameters;

    hardwareRaw() = default;
    ~hardwareRaw() = default;
};