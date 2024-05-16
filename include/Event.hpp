#include <ActionIn.hpp>
#include <vector>
#include <mutex>
class Event
{
public:
  int m_eventID;
  std::vector<ActionIn<HardCommand> *> m_actions;

  // ф-ция принтует все активные ActionIn текущего Event
  void m_probeAction()
  {
    for (auto &action : m_actions)
    {
      if (action->m_isActive == true)
      {
        std::cout << std::asctime(localtime(&(*action).m_pLastCommand.m_time)) << std::endl;
        std::cout << "device ID: " << action->m_pDevice->m_ID << std::endl;
        action->m_pLastCommand.print();
      };
    };
  };
  Event() = default;
  ~Event() = default;
};