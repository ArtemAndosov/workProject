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
  inline static std::vector<std::string>* m_spArrayOut;
  int m_eventID;
  std::string m_name;
  eventRaw* m_pEventRaw;
  std::vector<ActionIn<HardCommand>*> m_actions;
  std::vector<ActionOut*> m_ActionsOut;
  std::vector<ActionOut*> m_sendActions;
  ActionInTime* m_pActionInTime{nullptr};

  void logicInTime() {
    // std::cout << "logicInTime activated" << std::endl;
    if (this->m_pEventRaw->m_parameters["MODE"][0] == "EXCHANGE") {
      std::cout << "send packet: ";
      for (size_t i = 0; i < this->m_pEventRaw->m_parameters["ARRAY"].size(); i++) {
        std::cout << this->m_pEventRaw->m_parameters["ARRAY"][i];
      }
      std::cout << std::endl;
      if (getTime() <= std::stoul(this->m_pEventRaw->m_parameters["TIME"][0]) * 1000000000) {
        m_pActionInTime->setTimeOutOn(std::stoi(this->m_pEventRaw->m_parameters["CYCLE_PERIOD_S"][0]));
      } else
        m_pActionInTime->m_status = Action::EStatus::closed;

    } else if (this->m_pEventRaw->m_parameters["MODE"][0] == "CHANGE_DATA") {
      (*m_spArrayOut)[std::stoi(this->m_pEventRaw->m_parameters["WORD"][0])] = this->m_pEventRaw->m_parameters["VALUE"][0];

      std::cout << std::endl;
      m_pActionInTime->m_status = Action::EStatus::closed;
    };
  }

  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        std::cout << "packet in hex: ";
        for (size_t i = 0; i < action->m_pLastCommand.m_packet.size(); i++) {
          std::cout << std::hex << action->m_pLastCommand.m_packet[i] << std::dec << "  ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
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