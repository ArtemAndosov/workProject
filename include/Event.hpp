#pragma once
#include <ActionIn.hpp>
#include <ActionInTime.hpp>
#include <ActionOut.hpp>
#include <EventRaw.hpp>
#include <includes.hpp>

/**
 * @brief принтует активные ActionIn
 *
 */
class Event {
 public:
  int m_eventID;
  std::string m_name;
  eventRaw* m_pEventRaw;
  std::vector<ActionIn<HardCommand>*> m_actions;
  std::vector<ActionOut*> m_ActionsOut;
  std::vector<ActionOut*> m_sendActions;
  ActionInTime* m_pActionInTime{nullptr};
  // TODO узнать как работает
  void logicInTime() {
    std::cout << "logicInTime activated" << std::endl;
    m_pActionInTime->m_isDone = true;
  }

  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        action->m_pLastCommand.m_packet[0] = 0;
        m_ActionsOut[0]->m_sendCommand = action->m_pLastCommand;
        m_sendActions.emplace_back(m_ActionsOut.at(0));
      };
    };
  }
  /**
   * @brief для активных ActionIn из своего вектора принтует
   *
   */
  std::vector<ActionOut*>* probeAction() {
    if (m_pActionInTime->m_isActive) {
      logicInTime();
      m_pActionInTime->m_isActive = false;
    } else {
      logicInInterface();
    }
    return &m_sendActions;
  };

  // первоначальная настройка переменных
  void setupPlugin() {

  };
  Event(eventRaw& ERaw) : m_name{ERaw.m_eventName}, m_pEventRaw{&ERaw} {};
  ~Event() = default;
};