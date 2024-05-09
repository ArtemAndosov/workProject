#pragma once
#include <ctime>
class HardCommand {
public:
  int m_ID;
  int m_array[10];
  const time_t seconds = std::time(NULL);
  const tm *timeinfo = std::localtime(&seconds);

  // ф-ция принтует HardCommand
  void m_print() {
    std::cout << std::asctime(localtime(&seconds));
    std::cout << "device ID: " << m_ID << std::endl;
    std::cout << "array: ";
    for (int i = 0; i < 10; i++) {
      std::cout << m_array[i];
    };
    std::cout << std::endl;
    std::cout << std::endl;
  };

  HardCommand(const int id, const int (&b)[10]) {
    this->m_ID = id;
    for (int i = 0; i < 10; i++) {
      this->m_array[i] = b[i];
    };
  };
  ~HardCommand() = default;
};