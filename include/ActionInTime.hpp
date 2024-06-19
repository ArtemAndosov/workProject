#pragma once
#include <Action.hpp>
#include <includes.hpp>
class ActionInTime : public Action {
 public:
  uint64_t m_timeStart_ns{0};    // когда надо начать эвент
  uint64_t m_currentTime_ns{0};  // как долго работает программа

  bool probeTime() {
    if (m_status == EStatus::open) {
      m_currentTime_ns = getTime();
      if (m_currentTime_ns >= m_timeStart_ns) {
        m_isActive = true;
        return true;
      }
    }
    return false;
  };

  void setTimeOutOn(uint64_t timeout) {
    m_timeStart_ns = getTime() + timeout * 1000000000;
   // m_status == EStatus::open;
    m_isActive = true;
  }

  ActionInTime() {
    this->m_interface = g_EInterfaceType::TIME;
    this->m_pHardWareRaw = nullptr;
  };

  // ActionInTime() = default;
  ~ActionInTime() = default;
};