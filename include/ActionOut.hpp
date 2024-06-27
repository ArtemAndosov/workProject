#pragma once
#include <Action.hpp>
#include <HardCommand.hpp>
#include <device.hpp>
#include <includes.hpp>
class ActionOut : public Action {
 public:
  device* m_pDevice;
  HardCommand m_sendCommand;

  void sendData() { m_pDevice->sendData(m_sendCommand); };

  ActionOut(hardwareRaw& raw) {
    HardCommand HC;
    m_sendCommand = HC;
    m_sendCommand.m_pDevice = raw.m_pDeviceRaw;
    this->m_interface = raw.m_pDeviceRaw->m_interface;
    this->m_eventName = raw.m_eventName;
    this->m_pHardWareRaw = &raw;
  };
  ~ActionOut() = default;
};