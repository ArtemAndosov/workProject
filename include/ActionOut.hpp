#pragma once
#include <Action.hpp>
#include <HardCommand.hpp>
#include <device.hpp>
#include <includes.hpp>
/**
 * @brief передает пакеты от события к девайсу(выход из события)
 *
 */
class ActionOut : public Action {
 public:
  device* m_pDevice{nullptr};   //!< Указатель на связанный девайс
  HardCommand m_sendCommand{};  //!< Хк полученный от события для передачи в девайс

  /**
   * @brief передает в девайс ХК от события
   *
   */
  void sendData() { m_pDevice->sendData(m_sendCommand); };
  /**
   * @brief Construct a new Action Out object
   *
   * @param raw ссылка на класс с исходными данными
   */
  ActionOut(hardwareRaw& raw) {
    m_sendCommand.m_pDevice = raw.m_pDeviceRaw;
    this->m_interface = raw.m_pDeviceRaw->m_interface;
    this->m_eventName = raw.m_eventName;
    this->m_pHardWareRaw = &raw;
  };
  /**
   * @brief Destroy the Action Out object
   *
   */
  ActionOut() = default;
  ~ActionOut() = default;
};