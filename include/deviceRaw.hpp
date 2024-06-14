#pragma once
#include <includes.hpp>
struct deviceRaw {
  int m_port;
  std::string m_deviceName;
  std::string m_path;
  g_EInterfaceType m_interface{g_EInterfaceType::ERROR};
  deviceRaw() = default;
  ~deviceRaw() = default;
};