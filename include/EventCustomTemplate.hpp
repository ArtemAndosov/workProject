#ifndef EVENTCUSTOM_20COPY
#define EVENTCUSTOM_20COPY
#include <ActionIn.hpp>
#include <ActionInTime.hpp>
#include <ActionOut.hpp>
#include <ActionOutEvent.hpp>
#include <Event.hpp>
#include <EventRaw.hpp>
#include <includes.hpp>

// У РАБОЧЕГО СОБЫТИЯ ИМЯ ФАЙЛА EventCustom.hpp

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
  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        action->m_status = Action::EStatus::open;
      };
    };
  }

  /**
   * @brief логика события при входе по времени
   *
   */
  void logicInTime() {
    if (getTime() <= m_endTime) {
      m_pActionInTime->setTimeOutOn(m_cyclePeriodSec);
      m_pActionInTime->m_status = Action::EStatus::open;
    } else
      m_pActionInTime->m_status = Action::EStatus::closed;
    m_pActionInTime->m_isActive = false;
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
    for (auto& i : m_actions) {
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

#endif /* EVENTCUSTOM_20COPY */
