#pragma once
#include <deviceRaw.hpp>
#include <includes.hpp>
/**
 * @brief Содержит пакет и инфу по нему
 *
 */
class HardCommand {
 public:
  deviceRaw* m_pDevice = nullptr;
  std::vector<int> m_packet;
  time_t m_time;

  void print() {
    for (size_t i = 0; i < m_packet.size(); i++) {
      std::cout << m_packet.at(i) << ",";
    }
    std::cout << std::endl;
  };

  HardCommand() {
    this->m_time = std::time(NULL);
    this->m_packet = std::vector<int>(10);
  };
  ~HardCommand() = default;
};