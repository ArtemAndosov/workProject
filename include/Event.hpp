#pragma once
#include <ActionIn.hpp>
#include <ActionInTime.hpp>
#include <ActionOut.hpp>
#include <EventRaw.hpp>
#include <includes.hpp>

/**
 * @brief обрабатывает входящие пакеты и генерирует пакеты на выдачу
 *
 */
class Event {
 public:
  inline static std::vector<std::string>* m_spArrayOut;  //
  int m_eventID{0};
  bool m_eventType{false};       // false - EXCHANGE,  true - CHANGE_DATA;
  uint64_t m_endTime{0};         // длительность события
  uint64_t m_cyclePeriodSec{0};  // частота перезахода в событие
  std::string m_name;
  eventRaw* m_pEventRaw;
  std::vector<ActionIn<HardCommand>*> m_actions;
  std::vector<ActionOut*> m_ActionsOut;
  std::vector<ActionOut*> m_sendActions;
  ActionInTime* m_pActionInTime{nullptr};
  /**
   * @brief логика события при входе по времени
   *
   */
  void logicInTime() {
    if (!m_eventType) {
      for (size_t i = 0; i < m_spArrayOut->size(); i++) {
        m_ActionsOut[0]->m_sendCommand.m_packet[i] = std::stoi((*(m_spArrayOut))[i], nullptr, 16);
      }
      m_sendActions.emplace_back(m_ActionsOut.at(0));
      if (getTime() <= m_endTime) {
        m_pActionInTime->setTimeOutOn(m_cyclePeriodSec);
      } else
        m_pActionInTime->m_status = Action::EStatus::closed;
      m_pActionInTime->m_isActive = false;
    } else {
      (*m_spArrayOut)[std::stoi(this->m_pEventRaw->m_parameters["WORD"][0])] = this->m_pEventRaw->m_parameters["VALUE"][0];
      std::cout << std::endl;
      m_pActionInTime->m_isActive = false;
      m_pActionInTime->m_status = Action::EStatus::closed;
    };
  }
  /**
   * @brief логика события при входе через интерфейс
   *
   */
  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        std::cout << std::endl;
        for (size_t i = 0; i < action->m_pLastCommand.m_packet.size(); i++) {
          std::printf("%x", action->m_pLastCommand.m_packet[i]);
        }
        std::cout << std::endl;
      };
    };
  }
  /**
   * @brief вызывает логику события
   *
   */
  std::vector<ActionOut*>* probeAction() {
    if (m_pActionInTime->m_isActive) {
      logicInTime();
    } else {
      logicInInterface();
    }
    return &m_sendActions;
  };

  /**
   * @brief первоначальная настройка переменных
   *
   */
  void setupPlugin() {
    if (this->m_pEventRaw->m_parameters["MODE"][0] == "EXCHANGE") {
      this->m_spArrayOut = &this->m_pEventRaw->m_parameters["ARRAY"];
      m_ActionsOut[0]->m_sendCommand.m_packet.resize(m_spArrayOut->size());
      this->m_endTime = std::stoul(this->m_pEventRaw->m_parameters["TIME"][0]) * 1000000000;
      m_cyclePeriodSec = std::stoi(this->m_pEventRaw->m_parameters["CYCLE_PERIOD_S"][0]);
    } else
      this->m_eventType = true;
  };
  Event(eventRaw& ERaw) : m_name{ERaw.m_eventName}, m_pEventRaw{&ERaw} {};
  ~Event() = default;
};