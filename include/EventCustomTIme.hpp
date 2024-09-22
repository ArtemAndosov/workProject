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
  inline static std::vector<ActionOut*>* m_spActionsOut;  //!< указатель на m_ActionsOut события EXCHANGE
  int m_wordNumber{0};                                    //!< номер слова которое меняем в событии типа CHANGE_DATA
  int m_value{0};                                         //!< значение на которое меняем слово в событии типа CHANGE_DATA
  enum class EeventType { EXCHANGE, CHANGE_DATA };        //!< список типов событий
  EeventType m_eventType{};                               //!< тип текушего события
  uint64_t m_endTime{0};                                  //!< длительность события EXCHANGE
  uint64_t m_cyclePeriodSec{0};                           //!< частота перезахода в событие EXCHANGE

  /**
   * @brief логика события при входе по времени
   *
   */
  void logicInTime() {
    if (m_eventType == EeventType::EXCHANGE) {
      m_sendActions.emplace_back(m_ActionsOut.at(0));

      if (getTime() <= m_endTime) {
        m_pActionInTime->setTimeOutOn(m_cyclePeriodSec);
        m_pActionInTime->m_status = Action::EStatus::open;
      } else
        m_pActionInTime->m_status = Action::EStatus::closed;
      m_pActionInTime->m_isActive = false;
    } else if (m_eventType == EeventType::CHANGE_DATA) {
      (*(m_spActionsOut))[0]->m_sendCommand.m_packet[m_wordNumber] = this->m_value;
      m_pActionInTime->m_isActive = false;
      m_pActionInTime->m_status = Action::EStatus::closed;
    }
  }
  /**
   * @brief логика события при входе через интерфейс
   *
   */
  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        std::cout << std::endl;
        for (std::size_t i = 0; i < action->m_pLastCommand.m_packet.size(); i++) {
          std::printf("%x", action->m_pLastCommand.m_packet[i]);
        }
        std::cout << std::endl;
        action->m_status = Action::EStatus::open;
      };
    };
  }
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
   * @brief вторичная настройка переменных перед запуском программы
   *
   */
  void setupPlugin() {
    if (this->m_pEventRaw->m_parameters["MODE"][0] == "EXCHANGE") {
      m_spActionsOut = &this->m_ActionsOut;
      m_actions[0]->m_status = Action::EStatus::open;
      m_ActionsOut[0]->m_sendCommand.m_packet.resize(m_pEventRaw->m_parameters["ARRAY"].size());
      for (std::size_t i = 0; i < m_pEventRaw->m_parameters["ARRAY"].size(); i++) {
        m_ActionsOut[0]->m_sendCommand.m_packet[i] = std::stoi(m_pEventRaw->m_parameters["ARRAY"][i], nullptr, 16);
      }
      m_endTime = std::stoul(this->m_pEventRaw->m_parameters["TIME"][0]) * 1000000000;
      m_cyclePeriodSec = std::stoi(this->m_pEventRaw->m_parameters["CYCLE_PERIOD_S"][0]);
      m_eventType = EeventType::EXCHANGE;

    } else {
      m_eventType = EeventType::CHANGE_DATA;
      m_wordNumber = std::stoi(this->m_pEventRaw->m_parameters["WORD"][0]);
      m_value = std::stoi(this->m_pEventRaw->m_parameters["VALUE"][0], nullptr, 16);
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