// #include <HardCommand.hpp>
#include <Event.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
std::mutex mtx_queue;
int main()
{

  // создали очередь
  std::queue<HardCommand> queue;
  std::vector<deviceRAW> deviceRAWs;
  std::vector<device> devices;
  std::vector<Event> events;
  std::vector<ActionIn<HardCommand>> ActionsIn;

  deviceRAWs.emplace_back(1);
  deviceRAWs.emplace_back(2);
  deviceRAWs.emplace_back(3);

  // создали девайсы
  for (size_t i = 0; i < deviceRAWs.size(); i++)
  {
    devices.emplace_back(deviceRAWs[i]);
    devices[i].m_pQueue = &queue;
    devices[i].mtx_queue = &mtx_queue;
  };
  // запускаем девайсы
  for (size_t i = 0; i < devices.size(); i++)
  {
    devices[i].m_start();
  };

  // создали ActionIn
  for (size_t i = 0; i < devices.size(); i++)
  {
    ActionsIn.emplace_back(ActionIn<HardCommand>());
    ActionsIn[i].m_pDevice = &devices[i];
  };

  // создали эвенты
  for (size_t i = 0; i < 2; i++)
  {
    events.emplace_back(Event());
    events[i].m_eventID = i;
  }
  // ссвязали ActionIn и эвенты
  events[0].m_actions.emplace_back(&ActionsIn[0]);
  events[1].m_actions.emplace_back(&ActionsIn[1]);
  events[1].m_actions.emplace_back(&ActionsIn[2]);

  for (auto &event : events)
  {
    for (auto &action : event.m_actions)
    {
      action->m_eventID = event.m_eventID;
    }
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  // поехал основной процесс
  while (true)
  {
    mtx_queue.lock();
    if (!queue.empty())
    {
      // std::cout << "if " << std::endl;
      for (auto &action : ActionsIn)
      {
        if (action.m_probePacket(queue.front()))
        {

          events[action.m_eventID].m_probeAction();
          action.m_isActive = false;
        }
      };
      queue.pop();
    };
    mtx_queue.unlock();
  };
  return 0;
};