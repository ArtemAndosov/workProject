#pragma once
#include <deviceRaw.hpp>
#include <includes.hpp>
struct hardwareRaw {
  std::string m_direction;
  std::string m_eventName;
  std::string m_deviceName;
  std::string m_address;
  std::string m_port;
  std::string m_parameters;
  deviceRaw* m_pDeviceRaw;
  hardwareRaw() = default;
  ~hardwareRaw() = default;
};