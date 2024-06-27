#pragma once
#include <includes.hpp>
struct eventRaw {
  uint64_t m_timeStart{0};
  std::string m_eventName{};
  std::string m_parametersName{};
  std::map<std::string, std::vector<std::string>> m_parameters;
  eventRaw() = default;
  ~eventRaw() = default;
};