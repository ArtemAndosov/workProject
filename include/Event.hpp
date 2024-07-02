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
  inline static std::vector<int> m_spArrayOut;
  int m_eventID{0};
  int m_wordNumber{0};  // номер слова которое меняем
  int m_value{0};       // значение на которое меняем слово
  enum class EeventType { EXCHANGE, CHANGE_DATA };
  EeventType m_eventType{};
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
    if (m_eventType == EeventType::EXCHANGE) {
      for (size_t i = 0; i < m_spArrayOut.size(); i++) {
        m_ActionsOut[0]->m_sendCommand.m_packet[i] = m_spArrayOut[i];
      }
      m_sendActions.emplace_back(m_ActionsOut.at(0));

      if (getTime() <= m_endTime) {
        m_pActionInTime->setTimeOutOn(m_cyclePeriodSec);
        m_pActionInTime->m_status = Action::EStatus::open;
      } else
        m_pActionInTime->m_status = Action::EStatus::closed;
      m_pActionInTime->m_isActive = false;
    } else if (m_eventType == EeventType::CHANGE_DATA) {
      m_spArrayOut[m_wordNumber] = this->m_value;
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
        action->m_status = Action::EStatus::open;
      };
    };
  }
  /**
   * @brief вызывает логику события
   *
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
   * @brief первоначальная настройка переменных
   *
   */
  void setupPlugin() {
    if (this->m_pEventRaw->m_parameters["MODE"][0] == "EXCHANGE") {
      m_actions[0]->m_status = Action::EStatus::open;

      m_spArrayOut.resize(m_pEventRaw->m_parameters["ARRAY"].size());
      for (size_t i = 0; i < m_pEventRaw->m_parameters["ARRAY"].size(); i++) {
        m_spArrayOut[i] = std::stoi(m_pEventRaw->m_parameters["ARRAY"][i], nullptr, 16);
      }
      m_ActionsOut[0]->m_sendCommand.m_packet.resize(m_spArrayOut.size());
      m_endTime = std::stoul(this->m_pEventRaw->m_parameters["TIME"][0]) * 1000000000;
      m_cyclePeriodSec = std::stoi(this->m_pEventRaw->m_parameters["CYCLE_PERIOD_S"][0]);
      m_eventType = EeventType::EXCHANGE;

    } else {
      m_eventType = EeventType::CHANGE_DATA;
      m_wordNumber = std::stoi(this->m_pEventRaw->m_parameters["WORD"][0]);
      m_value = std::stoi(this->m_pEventRaw->m_parameters["VALUE"][0], nullptr, 16);
    }
  };
  Event(eventRaw& ERaw) : m_name{ERaw.m_eventName}, m_pEventRaw{&ERaw} {};
  ~Event() = default;
};
