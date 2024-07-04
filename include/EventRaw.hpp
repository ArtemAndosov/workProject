#pragma once
#include <includes.hpp>
/**
 * @brief базовая структура с данными эвента, парсится из .csv
 *
 */
struct eventRaw {
  uint64_t m_timeStart{0};                                       //!< время старта события от начала работы программы в сек
  std::string m_eventName{};                                     //!< имя события
  std::string m_parametersName{};                                //!< имя набора параметров
  std::map<std::string, std::vector<std::string>> m_parameters;  //!< карта параметров
  eventRaw() = default;
  ~eventRaw() = default;
};