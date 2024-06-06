#pragma once
#include <HardCommand.hpp>
#include <chrono>
#include <ctime>
#include <mutex>
#include <queue>
#include <thread>
#include <string>
#include <deviceRaw.hpp>
/**
 * @brief создает пакеты, формирует ХК и пишет их в очередь раз в сек
 *
 */
class device
{
public:
  std::mutex *m_pQueueMutex = nullptr;
  std::queue<HardCommand> *m_pQueue;
  deviceRaw *m_pDeviceRaw;
  std::string m_deviceName;
  int m_array[10];
  enum class EInterfaceType : uint8_t
  {
    error,
    test
  };
  EInterfaceType m_interface;

  /**
   * @brief генерирует массив случ.чисел(пакет)
   *
   */
  void generateMassive()
  {
    std::srand(time(0));
    for (int i = 0; i < 10; i++)
      m_array[i] = 1 + rand() % 5;
  };

  /**
   * @brief заполняет HardCommand и добавляет в очередь раз в секунду
   *
   */
  void listen()
  {
    std::thread start([this]()
                      {
    while (true)
    {
      generateMassive();
      HardCommand Hc1;
      Hc1.m_pDevice = this->m_pDeviceRaw;
      Hc1.m_packet.assign(m_array, m_array + (std::size(m_array)));
      m_pQueueMutex->lock();
      m_pQueue->push(Hc1);
      m_pQueueMutex->unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } });
    start.detach();
  };

  // в конструктор передаем ИД девайса
  device(deviceRaw &dev) : m_pDeviceRaw{&dev} { this->m_deviceName = dev.m_deviceName; };
  ~device() = default;
};