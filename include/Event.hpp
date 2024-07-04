#pragma once
#include <ActionIn.hpp>
#include <ActionInTime.hpp>
#include <ActionOut.hpp>
#include <EventRaw.hpp>
#include <includes.hpp>

/**
 * @brief базовый класс для событий
 *
 */
class Event {
 public:
  int m_eventID{0};                               //!< ИД события
  std::string m_name;                             //!< имя события
  eventRaw* m_pEventRaw;                          //!< указатель на класс с исходными данными для текущего события
  std::vector<ActionIn<HardCommand>*> m_actions;  //!< список связанных с текущим событием ActionIn"ов
  std::vector<ActionOut*> m_ActionsOut;           //!< список связанных с текущим событием ActionOut"ов
  std::vector<ActionOut*> m_sendActions;          //!< список исходящих ActionOut"ов для дальнейшего исполнения
  ActionInTime* m_pActionInTime{nullptr};         //!< указатель на связанный с текущим событием ActionInTime
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
  /**
   * @brief вызывает логику события
   *
   * @return std::vector<ActionOut*>*  указатель на список
   */
  std::vector<ActionOut*>* probeAction() {
    m_sendActions.clear();
    return &m_sendActions;
  };

  /**
   * @brief вторичная настройка переменных перед запуском программы
   *
   */
  void setupPlugin() {

  };
  /**
   * @brief Construct a new Event object
   *
   * @param ERaw ссылка на класс с исходными данными
   */

  Event() = default;
  /**
   * @brief Destroy the Event object
   *
   */
  ~Event() = default;
};
