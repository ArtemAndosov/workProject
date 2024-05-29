#pragma once
#include <string>
#include <device.hpp>
#include <HardwareRaw.hpp>

class Action
{
public:
    int m_eventID;
    bool m_isActive;
    std::string m_direction;
    std::string m_eventName;
    std::string m_address;
    std::string m_port;
    std::string m_parameters;
    device *m_pDevice;
    hardwareRaw *m_pHardWareRaw;
   
    Action() = default;
    ~Action() = default;
};
