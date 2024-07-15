#pragma once
#include <HardCommand.hpp>
#include <deviceRaw.hpp>
#include <includes.hpp>
/**
 * @brief создает пакеты, формирует ХК и пишет их в очередь раз в сек
 *
 */
class device {
 public:
  int m_packetCounter{0};                      //!< счетчик пакетов (если они предзаданы)
  std::mutex* m_pQueueMutex{nullptr};          //!< Указатель на очередь мьютекс в мейне
  std::queue<HardCommand>* m_pQueue{nullptr};  //!< Указатель на очередь ХК в мейне
  deviceRaw* m_pDeviceRaw;                     //!< Указатель на класс с исходными данными
  std::string m_deviceName;                    //!< Имя девайса
  uint8_t m_array[11];                         //!< Условный пакет

  /**
   * @brief генерирует массив случ.чисел(пакет)
   *
   */
  void generateMassive() {
    std::srand(time(0));
    for (int i = 0; i < 10; i++)
      m_array[i] = 1 + rand() % 20;
  };

  /**
   * @brief заполняет массив из готового пакета
   *
   * @param packet  вектор предзаданых пакетов
   */
  void generateMassive(std::vector<uint8_t>& packet) {
    for (size_t i = 0; i < sizeof(m_array); i++)
      m_array[i] = packet[i];
  };

  /**
   * @brief заполняет HardCommand и добавляет в очередь раз в секунду
   *
   */
  void listen() {
    std::thread start([this]() {
      while (true) {
        // generateMassive();  // для генерации случайного массива
        device::generateMassive(m_pDeviceRaw->m_packet[m_packetCounter]);  // для получения предзаданного массива
        m_packetCounter++;
        HardCommand Hc1;
        Hc1.m_pDevice = this->m_pDeviceRaw;
        Hc1.m_packet.assign(m_array, m_array + (std::size(m_array)));
        m_pQueueMutex->lock();
        m_pQueue->push(Hc1);
        m_pQueueMutex->unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });
    start.detach();
  };
  /**
   * @brief Принтует(условно получает) полученный(от ActionOut) пакет
   *
   * @param HC
   */
  void sendData(HardCommand HC) {
    std::cout << std::endl;
    std::cout << std::asctime(localtime(&HC.m_time));
    std::cout << "device name: " << HC.m_pDevice->m_deviceName << std::endl;
    HC.print();
  };

  /**
   * @brief Construct a new device object
   *
   * @param dev ссылка на класс с исходными данными
   */
  device(deviceRaw& dev) : m_pDeviceRaw{&dev} { this->m_deviceName = dev.m_deviceName; };
  /**
   * @brief Destroy the device object
   *
   */
  ~device() = default;
};