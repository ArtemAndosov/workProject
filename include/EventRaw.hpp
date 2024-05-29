#pragma once
#include <string>
struct eventRaw
{
    int m_timeStart;
    std::string m_eventName;
    std::string m_parameters;
    eventRaw() = default;
    ~eventRaw() = default;
};