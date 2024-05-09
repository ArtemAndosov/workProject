#include <HardCommand.hpp>
#include <chrono>
#include <mutex>
#include <queue>
#include <thread>
class device {
public:
  std::queue<HardCommand> *m_pQueue;
  std::mutex *mute;
  int m_ID;
  int m_array[10];

  // ф-ция генерирует массив случ.чисел
  void m_generate_massive() {
    srand(time(0));
    for (int i = 0; i < 10; i++)
      m_array[i] = 1 + rand() % 5;
  };

  // ф-ция генерирует HardCommand и добавляет в очередь
  void m_create_Hc() {
    while (true) {
      // mute->lock();
      m_generate_massive();
      HardCommand Hc1(m_ID, m_array);
      m_pQueue->push(Hc1);
      // mute->unlock();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    };
  };

  // запускаем поток девайса
  void m_start() {
    std::thread create_Hc(&device::m_create_Hc, this);
    create_Hc.detach();
  };

  // в конструктор передаем ИД девайса
  device(int a) { this->m_ID = a; }

  // device () = default;
  ~device() = default;
};