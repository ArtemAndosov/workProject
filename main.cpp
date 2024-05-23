#include <Event.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <rapidcsv.h>

std::mutex queueMutex;

void parseTable(std::vector<eventRaw> &Raw);
void parseTable(std::vector<deviceRaw> &Raw);

int main()
{
  std::queue<HardCommand> queue;
  std::vector<deviceRaw> deviceRaws;
  std::vector<device> devices;
  std::vector<eventRaw> eventRaws;
  std::vector<Event> events;
  std::vector<ActionIn<HardCommand>> actionsIn;

  parseTable(deviceRaws);
  // создали девайсроу
  // deviceRaws.emplace_back(1);
  // deviceRaws.emplace_back(2);
  // deviceRaws.emplace_back(3);

  // cоздаем евентроу
  parseTable(eventRaws);

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
}

void parseTable(std::vector<eventRaw> &Raw)
{
  rapidcsv::Document doc("../csv/events.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < doc.GetRowCount(); i++)
  {
    Raw.emplace_back();
    std::string name = doc.GetCell<std::string>("name", i);
    Raw[i].m_eventName = name;
  };
}

void parseTable(std::vector<deviceRaw> &Raw)
{
  rapidcsv::Document doc("../csv/devices.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < doc.GetRowCount(); i++)
  {
    Raw.emplace_back();
    std::string name = doc.GetCell<std::string>("name", i);
    Raw[i].m_deviceName = name;
  };
}