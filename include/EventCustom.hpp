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
  int m_cod1{0};          //!< код1
  int m_errorCounter{0};  //!< счетчик сбоев

  /**
   * @brief логика события при входе через интерфейс
   *
   */
  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        if (((action->m_pLastCommand.m_packet[0] >> 7) & 0xF)) {  // если признак ошибки
          std::printf("\nPacket has error\n");
        }
        // если контрольная сумма не верна или если признак ошибки
        if (CRC8(&(action->m_pLastCommand.m_packet[0]), 10) != action->m_pLastCommand.m_packet[10] || ((action->m_pLastCommand.m_packet[0] >> 7) & 0xF)) {
          std::printf("\ncheck sum fault");
          std::printf("\npacket number is:: %u\n", action->m_pLastCommand.m_packet[9]);
          std::printf("cod1: %u\n", m_cod1);
          std::printf("cod2: %u\n", (action->m_pLastCommand.m_packet[0] & 0xF));
          m_errorCounter++;
        } else {
          m_cod1 = (((action->m_pLastCommand.m_packet[0]) >> 4) & 0x7);
          std::printf("\npacket number is:: %u\n", action->m_pLastCommand.m_packet[9]);
          std::printf("cod1: %u\n", m_cod1);
          std::printf("cod2: %u", (action->m_pLastCommand.m_packet[0] & 0xF));
          m_ActionsOut[0]->m_sendCommand.m_pDevice = action->m_pLastCommand.m_pDevice;
          m_sendActions.emplace_back(m_ActionsOut.at(0));
          m_sendActions[0]->m_sendCommand.m_packet.resize(3);
          m_sendActions[0]->m_sendCommand.m_packet[0] = m_errorCounter;
          m_sendActions[0]->m_sendCommand.m_packet[0] <<= 3;
          m_sendActions[0]->m_sendCommand.m_packet[0] |= m_cod1;
          m_sendActions[0]->m_sendCommand.m_packet[1] = (action->m_pLastCommand.m_packet[9]) - m_errorCounter + 1;
          m_sendActions[0]->m_sendCommand.m_packet[2] = action->m_pLastCommand.m_packet[10];
        }
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

    logicInInterface();

    return &m_sendActions;
  };
  /**
   * @brief вторичная настройка переменных перед запуском программы
   *
   */
  void setupPlugin() {
    for (auto&& i : m_actions) {
      i->m_status = Action::EStatus::open;
    }
  };
  /**
   * @brief функция рассчета контрольной суммы
   *
   * @tparam T
   * @param data указатель на массив данных
   * @param length_bytes нужное количество байт для обработки
   * @return T контрольная сумма
   */
  template <typename T>
  T CRC8(T* data, size_t length_bytes) {
    T crc = 0x00;
    T extract;
    T sum;
    for (size_t i = 0; i < length_bytes; i++) {
      extract = *data;
      for (T tempI = 8; tempI; tempI--) {
        sum = (crc ^ extract) & 0x01;
        crc >>= 1;
        if (sum)
          crc ^= 0x8C;
        extract >>= 1;
      }
      data++;
    }
    return crc;
  }
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