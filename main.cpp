// #include <HardCommand.hpp>
#include <Event.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

int main()
{

  // создали очередь
  std::queue<HardCommand> queue;
  std::vector<Event> events;
  deviceROW DR1(1);
  deviceROW DR2(2);
  deviceROW DR3(3);
  // создали девайсы
  device a(DR1);
  a.m_pQueue = &queue;

  device b(DR2);
  b.m_pQueue = &queue;

  device c(DR3);
  c.m_pQueue = &queue;

  // запускаем девайсы
  a.m_start();
  b.m_start();
  c.m_start();

  // создали ActionIn
  ActionIn<HardCommand> dev1;
  dev1.m_pDevice = &a;
  ActionIn<HardCommand> dev2;
  dev2.m_pDevice = &b;
  ActionIn<HardCommand> dev3;
  dev3.m_pDevice = &c;

  // создали эвенты
  Event Ev1;
  Ev1.m_actions.push_back(&dev1);
  Event Ev2;
  Ev2.m_actions.push_back(&dev2);
  Ev2.m_actions.push_back(&dev3);

  // заполняем вектор эвентов, проставляем ИД у ActionIn == ИД эвента
  Ev1.m_eventID = 0;
  for (auto action : Ev1.m_actions)
  {
    action->m_eventID = 0;
  };
  events.push_back(Ev1);

  Ev2.m_eventID = 1;
  for (auto action : Ev2.m_actions)
  {
    action->m_eventID = 1;
  };
  events.push_back(Ev2);

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  // поехал основной процесс
  while (true)
  {
    for (auto event : events)
      for (auto action : event.m_actions)
      {
        if (action->m_probePacket(queue.front()))
        {
          event.m_probeAction();
          action->m_isActive = false;
        }
      };
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // если очередь пустая, ждем сек чтобы заполнилась
  };
  return 0;
}