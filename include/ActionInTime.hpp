#pragma once
#include <Action.hpp>
#include <includes.hpp>
/**
 * @brief проверка и активация входа по времени
 *
 */
class ActionInTime : public Action {
 public:
  uint64_t m_timeStart_ns{0};    //!< когда надо начать эвент
  uint64_t m_currentTime_ns{0};  //!< как долго работает программа

  /**
   * @brief проверяет подошло ли время открытия события и активирует его
   *
   * @return true
   * @return false
   */
  bool probeTime() {
    if (m_status == EStatus::open) {
      m_currentTime_ns = getTime();
      if (m_currentTime_ns >= m_timeStart_ns) {
        m_isActive = true;
        m_status = EStatus::closed;
        return true;
      }
      return false;
    }
    return false;
  };
  /**
   * @brief Устанавливает интервал перезахода в событие по времени
   *
   * @param timeout время перезахода в секундах
   */
  void setTimeOutOn(uint64_t timeout) { m_timeStart_ns = getTime() + timeout * 1000000000; }
  /**
   * @brief Construct a new Action In Time object
   *
   */
  ActionInTime() {
    this->m_interface = g_EInterfaceType::TIME;
    this->m_pHardWareRaw = nullptr;
  };

  /**
   * @brief Destroy the Action In Time object
   *
   */
  ~ActionInTime() = default;
};