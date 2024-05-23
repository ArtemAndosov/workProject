#pragma once
#include <HardCommand.hpp>
#include <chrono>
#include <ctime>
#include <mutex>
#include <queue>
#include <thread>
#include <string>
#include <deviceRaw.hpp>

class device
{
public:
  std::mutex *m_pQueueMutex = nullptr;
  std::queue<HardCommand> *m_pQueue;
  deviceRaw *m_deviceRaw;
  std::string m_deviceName;
  int m_array[10];
  enum class EInterfaceType : uint8_t
  {
    error,
    test
  };
  EInterfaceType m_interface;
  // ф-ция генерирует массив случ.чисел
  void generateMassive()
  {
    std::srand(time(0));
    for (int i = 0; i < 10; i++)
      m_array[i] = 1 + rand() % 5;
  };

  // ф-ция заполняет HardCommand и добавляет в очередь
  void listen()
  {
    while (true)
    {
      generateMassive();
      HardCommand Hc1;
      Hc1.m_pDevice = m_deviceRaw;
      Hc1.m_packet.assign(m_array, m_array + (std::size(m_array)));
      m_pQueueMutex->lock();
      m_pQueue->push(Hc1);
      m_pQueueMutex->unlock();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    };
  };

  // запускаем поток девайса
  void start()
  {
    std::thread t_listen(&device::listen, this);
    t_listen.detach();
  };

  // в конструктор передаем ИД девайса
  device(deviceRaw &dev) : m_deviceRaw{&dev} { this->m_deviceName = dev.m_deviceName; };
  ~device() = default;
};