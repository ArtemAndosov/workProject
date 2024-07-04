#pragma once
#include <ActionIn.hpp>
#include <ActionInTime.hpp>
#include <ActionOut.hpp>
#include <Event.hpp>
#include <EventRaw.hpp>
#include <includes.hpp>
/**
 * @brief обрабатывает входящие пакеты и генерирует пакеты на выдачу
 *
 */
class EventCustom : public Event {
 public:
  /**
   * @brief логика события при входе по времени
   *
   */
  void logicInTime() {}
  /**
   * @brief логика события при входе через интерфейс
   *
   */
  void logicInInterface() {}
  std::vector<ActionOut*>* probeAction() {
    m_sendActions.clear();
    if (m_pActionInTime->m_isActive) {
      logicInTime();
    } else {
      logicInInterface();
    }
    return &m_sendActions;
  };
  /**
   * @brief вторичная настройка переменных перед запуском программы
   *
   */
  void setupPlugin() {
    for (auto&& i : m_actions) {
      i->m_status = Action::EStatus::open;
    }
  };
  /**
   * @brief Construct a new Event object
   *
   * @param ERaw ссылка на класс с исходными данными
   */
  EventCustom(eventRaw& ERaw) {
    m_name = ERaw.m_eventName;
    m_pEventRaw = &ERaw;
  };
  /**
   * @brief Destroy the Event object
   *
   */
  ~EventCustom() = default;
};