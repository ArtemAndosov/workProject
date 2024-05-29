#pragma once
#include <ActionIn.hpp>
#include <vector>
#include <mutex>
#include <string>
#include <EventRaw.hpp>
class Event
{
public:
  std::string m_name;
  std::string m_timeStart;
  std::string m_parameters;
  int m_eventID;
  eventRaw *m_pEventRaw;
  std::vector<ActionIn<HardCommand> *> m_actions;

  // ф-ция принтует все активные ActionIn текущего Event
  void probeAction()
  {
    for (auto &action : m_actions)
    {
      if (action->m_isActive == true)
      {
        std::cout << std::asctime(localtime(&(*action).m_pLastCommand.m_time));
        std::cout << "device name: " << action->m_pDevice->m_deviceName << std::endl;
        action->m_pLastCommand.print();
        std::cout << std::endl;
      };
    };
  };
  Event(eventRaw &ERaw) : m_name{ERaw.m_eventName}, m_pEventRaw{&ERaw} {};
  ~Event() = default;
};