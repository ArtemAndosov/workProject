#include <Event.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <rapidcsv.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

std::mutex queueMutex;
std::queue<HardCommand> queue;
std::vector<deviceRaw> deviceRaws;
std::vector<device> devices;
std::vector<eventRaw> eventRaws;
std::vector<Event> events;
std::vector<hardwareRaw> hardWares;
std::vector<ActionIn<HardCommand>> actionsIn;

void parseTable(std::vector<eventRaw> &Raw);
void parseTable(std::vector<deviceRaw> &Raw);
void parseTable(std::vector<hardwareRaw> &Raw);
void LoadConfig();
void thr();

int main()
{
  LoadConfig();
  thr();
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
    int timeStart = doc.GetCell<int>("timeStart", i);
    Raw[i].m_timeStart = timeStart;
    std::string parameters = doc.GetCell<std::string>("parameters", i);
    Raw[i].m_parametersName = parameters;
  }

  rapidcsv::Document parametersEv("../csv/paramEv.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < parametersEv.GetRowCount(); i++)
  {
    std::string json = parametersEv.GetCell<std::string>("parameters", i);
    std::string parameters = parametersEv.GetCell<std::string>("name", i);
    rapidjson::Document d;
    d.Parse(json.c_str());
    for (auto &event : Raw)
    {
      if (event.m_parametersName == parameters)
      {
        for (rapidjson::Value::ConstMemberIterator itr = d.MemberBegin();
             itr != d.MemberEnd(); ++itr)
        {
          std::vector<std::string> vec;
          switch (itr->value.GetType())
          {
          case 5:
            vec.push_back(itr->value.GetString());
            event.m_parameters.emplace(itr->name.GetString(), vec);
            break;
          case 6:
            vec.push_back(std::to_string(itr->value.GetInt()));
            event.m_parameters.emplace(itr->name.GetString(), vec);
            break;
          case 4:
          {
            for (size_t i = 0; i < itr->value.Size(); i++)
              vec.push_back(itr->value[i].GetString());
            event.m_parameters.emplace(itr->name.GetString(), vec);
          }
          break;
          default:
            break;
          }
        }
      }
    }
  }
}

void parseTable(std::vector<deviceRaw> &Raw)
{

  rapidcsv::Document doc("../csv/devices.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < doc.GetRowCount(); i++)
  {
    Raw.emplace_back();
    std::string name = doc.GetCell<std::string>("name", i);
    Raw[i].m_deviceName = name;
    std::string interface = doc.GetCell<std::string>("interface", i);
    Raw[i].m_interface = Raw[i].StringToEnum[interface];
    std::string path = doc.GetCell<std::string>("path", i);
    Raw[i].m_path = path;
    int port = doc.GetCell<int>("port", i);
    Raw[i].m_port = port;
  };
}

void parseTable(std::vector<hardwareRaw> &Raw)
{
  rapidcsv::Document doc("../csv/hardware.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < doc.GetRowCount(); i++)
  {
    Raw.emplace_back();
    std::string eventName = doc.GetCell<std::string>("commandName", i);
    Raw[i].m_eventName = eventName;
    std::string device = doc.GetCell<std::string>("device", i);
    Raw[i].m_deviceName = device;
    std::string port = doc.GetCell<std::string>("port", i);
    Raw[i].m_port = port;
    std::string address = doc.GetCell<std::string>("address", i);
    Raw[i].m_address = address;
    std::string parameters = doc.GetCell<std::string>("parameters", i);
    Raw[i].m_parameters = parameters;
    std::string direction = doc.GetCell<std::string>("direction", i);
    Raw[i].m_direction = direction;
  };
}

void LoadConfig()
{

  // создали девайсроу

  parseTable(deviceRaws);

  // cоздаем евентроу
  parseTable(eventRaws);

  // cоздаем евентроу
  parseTable(hardWares);

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
  // не работает если у девайсов одинаковые имена
  for (size_t i = 0; i < hardWares.size(); i++)
  {
    if (hardWares[i].m_direction == "upload")
      actionsIn.emplace_back(ActionIn<HardCommand>(hardWares.at(i)));
  }

  for (auto &actIn : actionsIn)
  {
    for (auto &dev : devices)
    {
      if (dev.m_deviceName == actIn.m_pHardWareRaw->m_deviceName)
        actIn.m_pDevice = &dev;
    }
  }

  // создали эвенты
  for (size_t i = 0; i < eventRaws.size(); i++)
  {
    events.emplace_back(Event(eventRaws[i]));
    events[i].m_eventID = i;
  }
  // ссвязали ActionIn и эвенты
  for (size_t i = 0; i < actionsIn.size(); i++)
  {
    for (auto &event : events)
    {
      if (actionsIn[i].m_eventName == event.m_name)
      {
        event.m_actions.emplace_back(&actionsIn[i]);
        actionsIn[i].m_eventID = event.m_eventID;
      }
    }
  }
}

void thr() // поехал основной процессs
{
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
      }
      queue.pop();
    }
    queueMutex.unlock();
  }
}