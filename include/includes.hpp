#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include "rapidcsv.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
uint64_t programmStartTime_ns = std::chrono::nanoseconds(std::chrono::system_clock::now().time_since_epoch()).count();
enum class g_EInterfaceType : uint8_t { ERROR, COM, MAC, UDP, TIME };

std::map<std::string, g_EInterfaceType> StringToEnum{{"ERROR", g_EInterfaceType::ERROR},
                                                     {"COM", g_EInterfaceType::COM},
                                                     {"MAC", g_EInterfaceType::MAC},
                                                     {"UDP", g_EInterfaceType::UDP},
                                                     {"TIME", g_EInterfaceType::TIME}};

// как долго работает программа
uint64_t getTime() {
  uint64_t now = std::chrono::nanoseconds(std::chrono::system_clock::now().time_since_epoch()).count();
  uint64_t getTime = now - programmStartTime_ns;
  return getTime;
};
