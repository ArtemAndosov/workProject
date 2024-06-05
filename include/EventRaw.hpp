#pragma once
#include <string>
#include <map>
struct eventRaw
{
    int m_timeStart;
    std::string m_eventName;
    std::string m_parametersName;
    std::map<std::string, std::vector<std::string>> m_parameters;
    eventRaw() = default;
    ~eventRaw() = default;
};