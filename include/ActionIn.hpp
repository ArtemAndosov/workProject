#pragma once
#include <Action.hpp>
#include <includes.hpp>
#include <mutex>
/**
 * @brief Каждый экземпляр привязан к определенному девайсу и эвенту.
 * Раскидывает ХК из очереди по эвентам
 *
 * @tparam HardCommandType
 */
template <typename HardCommandType>
class ActionIn : public Action {
 public:
  HardCommandType m_pLastCommand;

  /**
   * @brief проверяет соответствие ИД девайса из конца очереди и своего
   * при соответствии пишет в себя ХК из очереди и активирует
   *
   * @param HC
   * @return true
   * @return false
   */
  bool probePacket(const HardCommand& HC) {
        if (m_status == EStatus::open) {
      if (HC.m_pDevice->m_deviceName == m_pDevice->m_deviceName)  // если ИД девайса ХК в очереди совпал с ИД девайса в АктионИне
      {
        m_pLastCommand = HC;  // Пишем ХК в Актион в евенте
        m_isActive = true;
        m_status = Action::EStatus::closed;
        return true;
      }
      return false;
    };
    return false;
  }
  ActionIn(hardwareRaw& raw) {
    this->m_interface = raw.m_pDeviceRaw->m_interface;
    this->m_eventName = raw.m_eventName;
    this->m_pHardWareRaw = &raw;
  };

  ~ActionIn() = default;
};
