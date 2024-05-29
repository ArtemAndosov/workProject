#pragma once
#include <string>
struct deviceRaw
{
    std::string m_deviceName;
    std::string m_interface;
    std::string m_path;
    int m_port;
    deviceRaw() = default;
    ~deviceRaw() = default;
};