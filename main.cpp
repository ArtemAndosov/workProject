#include <Event.hpp>
#include <includes.hpp>

std::mutex queueMutex;
std::mutex eventMutex;
std::queue<HardCommand> queue;
std::vector<deviceRaw> deviceRaws;
std::vector<device> devices;
std::vector<eventRaw> eventRaws;
std::vector<Event> events;
std::vector<hardwareRaw> hardWares;
std::vector<ActionIn<HardCommand>> actionsIn;
std::vector<ActionOut> actionsOut;
std::vector<ActionInTime> actionsInTime;

void parseTable(std::vector<eventRaw>& Raw);
void parseTable(std::vector<deviceRaw>& Raw);
void parseTable(std::vector<hardwareRaw>& Raw);
void LoadConfig();
void thrReactions(std::vector<ActionOut*>* result);
void thr();
void thrTime();

int main() {
  LoadConfig();

  for (auto& ev : events) {
    ev.setupPlugin();
  }

  std::thread start(thr);
  start.detach();
  std::thread startInTime(thrTime);
  startInTime.detach();

  while (1) {
  }

  return 0;
}

void parseTable(std::vector<eventRaw>& Raw) {
  rapidcsv::Document doc("../csv/events.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < doc.GetRowCount(); i++) {
    Raw.emplace_back();
    std::string name = doc.GetCell<std::string>("name", i);
    Raw[i].m_eventName = name;
    int timeStart = doc.GetCell<int>("timeStart", i);
    Raw[i].m_timeStart = timeStart;
    std::string parameters = doc.GetCell<std::string>("parameters", i);
    Raw[i].m_parametersName = parameters;
  }
  rapidcsv::Document parametersEv("../csv/paramEv.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < parametersEv.GetRowCount(); i++) {
    std::string json = parametersEv.GetCell<std::string>("parameters", i);
    std::string parameters = parametersEv.GetCell<std::string>("name", i);
    rapidjson::Document d;
    d.Parse(json.c_str());
    for (auto& event : Raw) {
      if (event.m_parametersName == parameters) {
        for (rapidjson::Value::ConstMemberIterator itr = d.MemberBegin(); itr != d.MemberEnd(); ++itr) {
          std::vector<std::string> vec;
          switch (itr->value.GetType()) {
            case 5:
              vec.push_back(itr->value.GetString());
              event.m_parameters.emplace(itr->name.GetString(), vec);
              break;
            case 6:
              vec.push_back(std::to_string(itr->value.GetInt()));
              event.m_parameters.emplace(itr->name.GetString(), vec);
              break;
            case 4: {
              for (size_t i = 0; i < itr->value.Size(); i++)
                vec.push_back(itr->value[i].GetString());
              event.m_parameters.emplace(itr->name.GetString(), vec);
            } break;
            default:
              break;
          }
        }
      }
    }
  }
}

void parseTable(std::vector<deviceRaw>& Raw) {
  rapidcsv::Document doc("../csv/devices.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < doc.GetRowCount(); i++) {
    Raw.emplace_back();
    std::string name = doc.GetCell<std::string>("name", i);
    Raw[i].m_deviceName = name;
    std::string interface = doc.GetCell<std::string>("interface", i);
    Raw[i].m_interface = StringToEnum[interface];
    std::string path = doc.GetCell<std::string>("path", i);
    Raw[i].m_path = path;
    int port = doc.GetCell<int>("port", i);
    Raw[i].m_port = port;
  };
}

void parseTable(std::vector<hardwareRaw>& Raw) {
  rapidcsv::Document doc("../csv/hardware.csv", rapidcsv::LabelParams(0, -1));
  for (size_t i = 0; i < doc.GetRowCount(); i++) {
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

void LoadConfig() {
  // заполняем девайсроу

  parseTable(deviceRaws);
  // заполняем евентроу
  parseTable(eventRaws);

  // заполняем hardwareRaw
  parseTable(hardWares);

  //
  for (auto& h : hardWares) {
    for (auto& d : deviceRaws) {
      if (h.m_deviceName == d.m_deviceName) {
        h.m_pDeviceRaw = &d;
      }
    }
  }
  // создали девайсы

  for (size_t i = 0; i < deviceRaws.size(); i++) {
    devices.emplace_back(deviceRaws[i]);
    devices[i].m_pQueue = &queue;
    devices[i].m_pQueueMutex = &queueMutex;
  }
  // запускаем девайсы
  for (size_t i = 0; i < devices.size(); i++) {
    devices[i].listen();
  }

  // создали ActionIn и ActionOut
  // не работает если у девайсов одинаковые имена
  for (size_t i = 0; i < hardWares.size(); i++) {
    if (hardWares[i].m_direction == "upload")
      actionsIn.emplace_back(ActionIn<HardCommand>(hardWares.at(i)));
    else if (hardWares[i].m_direction == "download")
      actionsOut.emplace_back(ActionOut(hardWares.at(i)));
  }

  // связали ActionIn и Девайс
  for (auto& actIn : actionsIn) {
    for (auto& dev : devices) {
      if (dev.m_deviceName == actIn.m_pHardWareRaw->m_deviceName)
        actIn.m_pDevice = &dev;
    }
  }
  // связали ActionOut и Девайс
  for (auto& actOut : actionsOut) {
    for (auto& dev : devices) {
      if (dev.m_deviceName == actOut.m_pHardWareRaw->m_deviceName)
        actOut.m_pDevice = &dev;
    }
  }
  // создали эвенты
  for (size_t i = 0; i < eventRaws.size(); i++) {
    events.emplace_back(Event(eventRaws[i]));
    events[i].m_eventID = i;
  }

  // создали actionsInTime
  for (size_t i = 0; i < events.size(); i++) {
    actionsInTime.emplace_back();
  }
  for (size_t i = 0; i < events.size(); i++) {
    events[i].m_pActionInTime = &actionsInTime[i];
    actionsInTime[i].m_eventID = i;
    actionsInTime[i].m_status = Action::EStatus::open;
    actionsInTime[i].m_eventName = events[i].m_name;
    actionsInTime[i].m_timeStart_ns = events[i].m_pEventRaw->m_timeStart * 1000000000;
  }
  // ссвязали ActionIn и эвенты
  for (size_t i = 0; i < actionsIn.size(); i++) {
    for (auto& event : events) {
      if (actionsIn[i].m_eventName == event.m_name) {
        event.m_actions.emplace_back(&actionsIn[i]);
        actionsIn[i].m_eventID = event.m_eventID;
      }
    }
  }
  // ссвязали ActionOut и эвенты
  for (size_t i = 0; i < actionsOut.size(); i++) {
    for (auto& event : events) {
      if (actionsOut[i].m_eventName == event.m_name) {
        event.m_ActionsOut.emplace_back(&actionsOut[i]);
        actionsOut[i].m_eventID = event.m_eventID;
      }
    }
  }
}

void thr()  // поехал основной процесс интерфейса в потоке
{
  while (true) {
    queueMutex.lock();
    if (!queue.empty()) {
      for (auto& action : actionsIn) {
        if (action.probePacket(queue.front())) {
          eventMutex.lock();
          auto result = events[action.m_eventID].probeAction();
          thrReactions(result);
          eventMutex.unlock();
          action.m_isActive = false;
        }
      }
      queue.pop();
    }
    queueMutex.unlock();
  };
}

void thrTime()  // поехал основной процесс по времени в потоке
{
  while (true) {
    for (auto& action : actionsInTime) {
      if (action.probeTime()) {
        eventMutex.lock();
        auto result = events[action.m_eventID].probeAction();
        thrReactions(result);
        eventMutex.unlock();
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  };
}

void thrReactions(std::vector<ActionOut*>* result) {
  for (auto& i : *result) {
    i->sendData();
  }
}
