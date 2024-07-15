#pragma once
#include <deviceRaw.hpp>
#include <includes.hpp>
/**
 * @brief Содержит пакет и инфу по нему
 *
 */
class HardCommand {
 public:
  deviceRaw* m_pDevice = nullptr;  //!< Указатель на десайв от которого получен пакет
  std::vector<uint8_t> m_packet;   //!< Условный пакет
  time_t m_time;                   //!< Время создания ХК
                                   /**
                                    * @brief принтует только пакет
                                    *
                                    */
  void print() {
    for (size_t i = 0; i < m_packet.size(); i++) {
      printf("0x%X  ", m_packet[i]);
    }
    std::cout << std::endl;
  };
  /**
   * @brief Construct a new Hard Command object
   *
   */
  HardCommand() {
    this->m_time = std::time(NULL);
    this->m_packet = std::vector<uint8_t>(10);
  };
  /**
   * @brief Destroy the Hard Command object
   *
   */
  ~HardCommand() = default;
};