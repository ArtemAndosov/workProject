#pragma once
#include <HardCommand.hpp>
#include <chrono>
#include <ctime>
#include <mutex>
#include <queue>
#include <thread>
#include <deviceRAW.hpp>

class device
{
public:
  std::queue<HardCommand> *m_pQueue;
  deviceRAW *m_deviceRAW;
  int m_ID;
  int m_array[10];
  enum class m_interface;
  // ф-ция генерирует массив случ.чисел
  void m_generateMassive()
  {
    std::srand(time(0));
    for (int i = 0; i < 10; i++)
      m_array[i] = 1 + rand() % 5;
  };

  // ф-ция заполняет HardCommand и добавляет в очередь
  void m_listen()
  {
    while (true)
    {
      m_generateMassive();
      HardCommand Hc1;
      Hc1.m_pDevice = m_deviceRAW;
      Hc1.m_packet.assign(m_array, m_array + (std::size(m_array)));
      m_pQueue->push(Hc1);
      std::this_thread::sleep_for(std::chrono::seconds(1));
    };
  };

  // запускаем поток девайса
  void m_start()
  {
    std::thread t_listen(&device::m_listen, this);
    t_listen.detach();
  };

  // в конструктор передаем ИД девайса
  device(deviceRAW &dev) : m_deviceRAW{&dev} { this->m_ID = dev.m_deviceID; };
  ~device() = default;
};