#include <ActionIn.hpp>
#include <vector>
#include <mutex>
class Event
{
public:
  int m_EventID;
  std::vector<ActionIn> m_actions;

  // ф-ция принтует все активные ActionIn текущего Event
  void m_probeAction(const int id)
  {
    for (auto &action : m_actions)
    {
      // забираем ХК из очереди в m_lastCommand при соответствии ИД
      action.m_probePacket(id);
      if (action.m_Active == true)
      {
        action.m_lastCommand->m_print();
        action.m_Active = false;
        if (!action.m_dev->m_pQueue->empty()) // если очередь не пустая
          // если раскомментить эту строку, вылезет ошибка double free or corruption (out)
          // std::cout << "queue size: " << action.m_dev->m_pQueue->size() << std::endl<< std::endl;
          action.m_dev->m_pQueue->pop(); // удаляем принтованный
      };
    };
  };

  Event() = default;
  ~Event() = default;
};