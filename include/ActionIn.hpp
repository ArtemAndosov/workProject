#include <HardCommand.hpp>
#include <device.hpp>
#include <mutex>

std::mutex atcmute;
class ActionIn {
public:
  int m_EventID;
  bool m_Active = false;
  device *m_dev;
  HardCommand *m_lastCommand;

  // забираем ХК из очереди в m_lastCommand при соответствии ИД
  void m_probePacket(const int id) {
    if (id == m_dev->m_ID) { // если ИД девайса ХК в очереди совпал с ИД девайса
                             // в АктионИне
                             // atcmute.lock();
      m_lastCommand = &(m_dev->m_pQueue->front()); // Пишем ХК в Актион в евенте
      m_Active = true;
      // atcmute.unlock();// активируем АктионИн
    }
  };

  ActionIn() = default;
  ~ActionIn() = default;
};
