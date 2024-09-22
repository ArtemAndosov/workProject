#ifndef EVENTCUSTOM_20COPY
#define EVENTCUSTOM_20COPY
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
  uint8_t m_cod1{0};        //!< код1
  int m_errorCounter{0};    //!< счетчик пакетов со сбоем
  int m_answersCounter{1};  //!< счетчик ответных пакетов
  /**
   * @brief логика события при входе через интерфейс
   *
   */
  void logicInInterface() {
    for (auto& action : m_actions) {
      if (action->m_isActive == true) {
        std::printf("\nReceived packet: ");  //!< при получении принтуем пакет и счетчик ошибок
        for (std::size_t i = 0; i < action->m_pLastCommand.m_packet.size(); i++) {
          std::printf("0x%X,", action->m_pLastCommand.m_packet[i]);
        }
        std::printf("\nm_errorCounter: %d\n", m_errorCounter);

        if (getErr(action->m_pLastCommand.m_packet[0])) {  //!< принтуем наличие ошибки в принятом пакете
          std::printf("Packet has error");
        }
        if (checkCRC(action->m_pLastCommand.m_packet) || getErr(action->m_pLastCommand.m_packet[0])) {
          if (checkCRC(action->m_pLastCommand.m_packet)) {
            std::printf("check sum fault");
          }
          std::printf("\npacket number is:: %u\n", action->m_pLastCommand.m_packet[9]);
          std::printf("cod1: %u\n", m_cod1);
          std::printf("cod2: %u\n", (action->m_pLastCommand.m_packet[0] & 0xF));
          m_errorCounter++;
        } else {  //!< если пакет принят без ошибок
          m_cod1 = (((action->m_pLastCommand.m_packet[0]) >> 4) & 0x7);
          std::printf("packet number is:: %u\n", action->m_pLastCommand.m_packet[9]);
          std::printf("cod1: %u\n", m_cod1);
          std::printf("cod2: %u", (action->m_pLastCommand.m_packet[0] & 0xF));
          answer();
          m_answersCounter++;
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
  T CRC8(T* data, std::size_t length_bytes) {
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
   * @brief проверка наличия признака ошибки в пакете
   *
   * @param v нулевое СД в принятом пакете
   * @return true
   * @return false
   */
  bool getErr(uint8_t v) { return ((v >> 7) & 0xF); };

  /**
   * @brief проверка контрольной суммы пакета
   *
   * @param vv пакет для которого проверяем контрольную сумму
   * @return true
   * @return false
   */
  bool checkCRC(std::vector<uint8_t>& vv) { return (CRC8(&vv[0], 10)) != vv[10]; };

  /**
   * @brief формирование ответного пакета
   *
   * @param action  текущий принятый пакет
   */
  void answer() {
    m_sendActions.emplace_back(m_ActionsOut.at(0));
    m_sendActions[0]->m_sendCommand.m_packet.resize(3);
    m_sendActions[0]->m_sendCommand.m_packet[0] = m_errorCounter;
    m_sendActions[0]->m_sendCommand.m_packet[0] <<= 3;
    m_sendActions[0]->m_sendCommand.m_packet[0] |= m_cod1;
    m_sendActions[0]->m_sendCommand.m_packet[1] = m_answersCounter;
    m_sendActions[0]->m_sendCommand.m_packet[2] = CRC8(&(m_sendActions[0]->m_sendCommand.m_packet[0]), 2);
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
