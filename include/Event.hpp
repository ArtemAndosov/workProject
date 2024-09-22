#pragma once
#include <ActionIn.hpp>
#include <ActionInTime.hpp>
#include <ActionOut.hpp>
#include <ActionOutEvent.hpp>
#include <EventRaw.hpp>
#include <includes.hpp>

/**
 * @brief базовый класс для событий
 *
 */
class Event {
 public:
  int m_eventID{0};                                //!< ИД события
  std::string m_name;                              //!< имя события
  eventRaw* m_pEventRaw;                           //!< указатель на класс с исходными данными для текущего события
  std::vector<ActionIn<HardCommand>*> m_actions;   //!< список связанных с текущим событием ActionIn"ов
  std::vector<ActionOut*> m_ActionsOut;            //!< список связанных с текущим событием ActionOut"ов
  std::vector<ActionOutEvent*> m_ActionsOutEvent;  //!< список связанных с текущим событием ActionOutEvent"ов
  std::vector<ActionOut*> m_sendActions;           //!< список исходящих ActionOut"ов для дальнейшего исполнения
  ActionInTime* m_pActionInTime{nullptr};          //!< указатель на связанный с текущим событием ActionInTime

  /**
   * @brief Construct a new Event object
   *
   */
  Event() = default;
  /**
   * @brief Destroy the Event object
   *
   */
  ~Event() = default;
};
