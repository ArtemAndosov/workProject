#pragma once
#include <HardwareRaw.hpp>
#include <device.hpp>
#include <includes.hpp>
#include <string>

class Action {
 public:
  int m_eventID;
  bool m_isActive{false};
  std::string m_direction;
  std::string m_eventName;
  std::string m_address;
  std::string m_port;
  std::string m_parameters;
  device* m_pDevice;
  hardwareRaw* m_pHardWareRaw;
  g_EInterfaceType m_interface{g_EInterfaceType::ERROR};

  Action() = default;
  ~Action() = default;
};
