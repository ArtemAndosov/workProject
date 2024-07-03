#pragma once
#include <deviceRaw.hpp>
#include <includes.hpp>
/**
 * @brief базовая структура с данными Action"ов(In and Out), парсится из .csv
 *
 */
struct hardwareRaw {
  std::string m_direction;   //!< определяет ActionIn или ActionOut
  std::string m_eventName;   //!< Имя связанного события
  std::string m_deviceName;  //!< Имя связанного девайса
  std::string m_address;     //!< не используется
  std::string m_port;        //!< не используется
  std::string m_parameters;  //!< не используется
  deviceRaw* m_pDeviceRaw;   //!< Указатель на класс с исходными данными для связанного девайса
  /**
   * @brief Construct a new hardware Raw object
   *
   */
  hardwareRaw() = default;
  /**
   * @brief Destroy the hardware Raw object
   *
   */
  ~hardwareRaw() = default;
};