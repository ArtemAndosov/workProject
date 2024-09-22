#ifndef ACTIONOUTEVENT
#define ACTIONOUTEVENT
#pragma once
#include <Action.hpp>
// #include <EventCustom.hpp>
#include <HardCommand.hpp>
#include <device.hpp>
#include <includes.hpp>
/**
 * @brief передает пакеты от события к событию(выход в другое событие)
 * (реализован выход через единственный ActionOutEvent с вектором дочерних)
 *
 */
class ActionOutEvent : public ActionOut {
 public:
  std::vector<int> m_DstId{};  //!< ИД дочерних событий для выхода в них

  /**
   * @brief вызывает дочернее событие
   *
   */

  /**
   * @brief Construct a new Action Out Event object
   *
   * @param raw ссылка на класс с исходными данными
   */
  ActionOutEvent(hardwareRaw& raw) {
    this->Action::m_interface = g_EInterfaceType::EVENT;
    this->ActionOut::m_eventName = raw.m_eventName;
    this->ActionOut::m_pHardWareRaw = &raw;
  };
  /**
   * @brief Destroy the Action Out object
   *
   */
  ~ActionOutEvent() = default;
};

#endif /* ACTIONOUTEVENT */
