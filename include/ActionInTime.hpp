#pragma once
#include <Action.hpp>
#include <includes.hpp>
class ActionInTime : public Action {
 public:
  uint64_t m_timeStart_ns{0};
  uint64_t m_currentTime_ns{0};
  bool m_isDone{false};

  bool probeTime() {
    if (!m_isDone) {
      m_currentTime_ns = getTime();
      if (m_currentTime_ns >= m_timeStart_ns) {
        m_isActive = true;
        return true;
      }
    }
    return false;
  };

  void setTimeOutOn(uint64_t timeout) {
    m_timeStart_ns = getTime() + timeout;
    // m_isActive = true;
  }

  ActionInTime() {
    this->m_interface = g_EInterfaceType::TIME;
    this->m_pHardWareRaw = nullptr;
  };

  // ActionInTime() = default;
  ~ActionInTime() = default;
};