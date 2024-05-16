#pragma once
#include <ctime>
#include <iostream>
#include <mutex>
#include <vector>
#include <deviceRAW.hpp>
class HardCommand
{
public:
  deviceRAW *m_pDevice;
  std::vector<int> m_packet;
  time_t m_time;

  void print()
  {
    for (size_t i = 0; i < m_packet.size(); i++)
    {
      std::cout << m_packet.at(i);
    }
    std::cout << std::endl;
  };

  HardCommand()
  {
    m_time = std::time(NULL);
  };
  ~HardCommand() = default;
};