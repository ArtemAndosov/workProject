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
    if (m_pEventRaw->m_parameters["MODE"][0] == "EXCHANGE") {
      for (size_t i = 0; i < m_spArrayOut->size(); i++) {
        int a = std::stoi((*(m_spArrayOut))[i], nullptr, 16);
        m_ActionsOut[0]->m_sendCommand.m_packet[i] = a;  // array из 16 в 10 и в паккет интов
      }
      m_sendActions.emplace_back(m_ActionsOut.at(0));

      std::cout << " cp 1" << std::endl;
      if (getTime() <= std::stoul(this->m_pEventRaw->m_parameters["TIME"][0]) * 1000000000) {
        m_pActionInTime->setTimeOutOn(std::stoi(this->m_pEventRaw->m_parameters["CYCLE_PERIOD_S"][0]));

      } else
        m_pActionInTime->m_status = Action::EStatus::closed;

    } else if (this->m_pEventRaw->m_parameters["MODE"][0] == "CHANGE_DATA") {
      (*m_spArrayOut)[std::stoi(this->m_pEventRaw->m_parameters["WORD"][0])] = this->m_pEventRaw->m_parameters["VALUE"][0];
      std::cout << std::endl;
      m_pActionInTime->m_isActive = false;
      m_pActionInTime->m_status = Action::EStatus::closed;
    };
  }

  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        for (size_t i = 0; i < action->m_pLastCommand.m_packet.size(); i++) {
          std::printf("%x", action->m_pLastCommand.m_packet[i]);
        }
        std::cout << std::endl;
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
    //  static array y event
    if (this->m_pEventRaw->m_parameters["MODE"][0] == "EXCHANGE")
      this->m_spArrayOut = &this->m_pEventRaw->m_parameters["ARRAY"];
    HardCommand HC;
    m_ActionsOut[0]->m_sendCommand = HC;
    m_ActionsOut[0]->m_sendCommand.m_packet.resize(m_spArrayOut->size());
  };
  Event(eventRaw& ERaw) : m_name{ERaw.m_eventName}, m_pEventRaw{&ERaw} {};
  ~Event() = default;
};