#include <Event.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::mutex mtx;
int main() {

  // создали очередь
  std::queue<HardCommand> m_Queue;
  std::vector<Event> Events;

  // создали девайсы
  device a(1);
  a.m_pQueue = &m_Queue;
  a.mute = &mtx;
  device b(2);
  b.m_pQueue = &m_Queue;
  b.mute = &mtx;
  device c(3);
  c.m_pQueue = &m_Queue;
  c.mute = &mtx;

  // запускаем девайсы
  a.m_start();
  b.m_start();
  c.m_start();

  // создали ActionIn
  ActionIn dev1;
  dev1.m_dev = &a;
  ActionIn dev2;
  dev2.m_dev = &b;
  ActionIn dev3;
  dev3.m_dev = &c;

  // создали эвенты
  Event Ev1;
  Ev1.m_actions.push_back(dev1);
  Event Ev2;
  Ev2.m_actions.push_back(dev2);
  Ev2.m_actions.push_back(dev3);

  // заполняем вектор эвентов, проставляем ИД у ActionIn == ИД эвента
  Ev1.m_EventID = 0;
  for (ActionIn &action : Ev1.m_actions) {
    action.m_EventID = 0;
  };
  Events.push_back(Ev1);

  Ev2.m_EventID = 1;
  for (ActionIn &action : Ev2.m_actions) {
    action.m_EventID = 1;
  };
  Events.push_back(Ev2);

  // std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  // поехал основной процесс
  while (true) {
    while (!m_Queue.empty()) {
      for (auto &event : Events) {
        event.m_probeAction(m_Queue.front().m_ID);
        // mtx.unlock();
      };
    };
    std::this_thread::sleep_for(std::chrono::milliseconds(
        1000)); // если очередь пустая, ждем сек чтобы заполнилась
  };
  return 0;
}