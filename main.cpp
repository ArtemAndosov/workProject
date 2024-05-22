#include <Event.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <rapidcsv.h>
std::mutex queueMutex;
void parseTable(std::vector<eventRaw> &eventRaws);
int main()
{
  std::queue<HardCommand> queue;
  std::vector<deviceRaw> deviceRaws;
  std::vector<device> devices;
  std::vector<eventRaw> eventRaws;
  std::vector<Event> events;
  std::vector<ActionIn<HardCommand>> actionsIn;

  // cоздаем евентроу
  parseTable(eventRaws);

  eventRaws.emplace_back();
  eventRaws.emplace_back();

  // создали девайсроу
  deviceRaws.emplace_back(1);
  deviceRaws.emplace_back(2);
  deviceRaws.emplace_back(3);

  // создали девайсы
  for (size_t i = 0; i < deviceRaws.size(); i++)
  {
    devices.emplace_back(deviceRaws[i]);
    devices[i].m_pQueue = &queue;
    devices[i].m_pQueueMutex = &queueMutex;
  }
  // запускаем девайсы
  for (size_t i = 0; i < devices.size(); i++)
  {
    devices[i].start();
  }

  // создали ActionIn
  for (size_t i = 0; i < devices.size(); i++)
  {
    actionsIn.emplace_back(ActionIn<HardCommand>());
    actionsIn[i].m_pDevice = &devices[i];
  }

  // создали эвенты
  for (size_t i = 0; i < eventRaws.size(); i++)
  {
    events.emplace_back(Event(eventRaws[i]));
    events[i].m_eventID = i;
  }
  // ссвязали ActionIn и эвенты
  events[0].m_actions.emplace_back(&actionsIn[0]);
  events[1].m_actions.emplace_back(&actionsIn[1]);
  events[1].m_actions.emplace_back(&actionsIn[2]);

  for (auto &event : events)
  {
    for (auto &action : event.m_actions)
    {
      action->m_eventID = event.m_eventID;
    }
  }

  // std::this_thread::sleep_for(std::chrono::milliseconds(4000));

  // поехал основной процесс
  while (true)
  {
    queueMutex.lock();
    if (!queue.empty())
    {
      for (auto &action : actionsIn)
      {
        if (action.probePacket(queue.front()))
        {

          events[action.m_eventID].probeAction();
          action.m_isActive = false;
        }
      };
      queue.pop();
    }
    queueMutex.unlock();
  }
  return 0;
};

void parseTable(std::vector<eventRaw> &eventRaws)
{
  rapidcsv::Document doc("../csv/events.csv", rapidcsv::LabelParams(0, -1));

  for (size_t i = 0; i < doc.GetRowCount(); i++)
  {
    std::string name = doc.GetCell<std::string>("name", i);
    eventRaws.emplace_back();
    eventRaws[i].name = name;
  }
};