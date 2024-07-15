#pragma once
#include <includes.hpp>
/**
 * @brief базовая структура с данными эвента, парсится из .csv
 *
 */
struct deviceRaw {
  //!< вектор предзаданых пакетов
  std::vector<std::vector<uint8_t>> m_packet{};
  int m_port;                                             //!< не используется
  std::string m_path;                                     //!< не используется
  std::string m_deviceName;                               //!< Имя девайса
  g_EInterfaceType m_interface{g_EInterfaceType::ERROR};  //!< тип интерфейса(тип входа)
  deviceRaw() = default;
  ~deviceRaw() = default;
};