#pragma once
#include <ActionIn.hpp>
#include <ActionInTime.hpp>
#include <ActionOut.hpp>
#include <ActionOutEvent.hpp>
#include <Event.hpp>
#include <EventRaw.hpp>
#include <includes.hpp>
/**
 * @brief обрабатывает входящие пакеты и генерирует пакеты на выдачу
 *
 */
class EventCustom : public Event {
 public:
  uint64_t m_endTime{0};         //!< длительность события
  uint64_t m_cyclePeriodSec{0};  //!< частота перезахода в событие

  /**
   * @brief логика события при входе через интерфейс
   *
   */
  void logicInInterface() {}

  /**
   * @brief логика события при входе по времени
   *
   */
  void logicInTime() { m_sendActions.emplace_back(m_ActionsOutEvent[0]); }

  /**
   * @brief выполнение логики события
   *
   * @return std::vector<ActionOut*>* список объектов с пакетами от события к девайсу(выход из события)
   */
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
   * @brief Set the Data object
   *
   */
  void setData() { printf("My name is %s \n", this->m_name.data()); };

  /**
   * @brief вторичная настройка переменных перед запуском программы
   *
   */
  void setupPlugin() {};

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