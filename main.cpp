#include <iostream>
#include <ctime>
#include <queue>
#include <Event.hpp>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex mtx;
int main (){
//создали очередь
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
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
    b.m_start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
     c.m_start();
//std::this_thread::sleep_for(std::chrono::milliseconds(100));
// создали экшнъины
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

// заполняем вектор эвентов, проставляем ИД у Акшн==ИД эвента
Ev1.m_EventID = 0;
for (ActionIn& action : Ev1.m_actions){
   action.m_EventID = 0;
};
Events.push_back(Ev1);

Ev2.m_EventID = 1;
for (ActionIn& action : Ev2.m_actions){
   action.m_EventID = 1;
};
Events.push_back(Ev2);

std::cout << "queue size: " << m_Queue.size() << std::endl << std::endl; // проверяем что очередь не пуста
//поехал процесс
while (true)  
 {
    for (auto& event : Events){   // перебираем эвенты
         //mtx.lock();
         event.m_probeAction(m_Queue.front().m_ID);
           // mtx.unlock();  
        }; 
             };
      

/*while (true)  
 {
  mtx.lock();
    for (auto& event : Events){   // перебираем эвенты
    for (auto& action : event.m_actions){ //перебираем АктионИны
      if (m_Queue.front().m_ID == action.m_dev->m_ID)   // проверяем соответствие ИД из очереди и ИД девайса в АктионИне
         {action.m_probePacket(m_Queue.front().m_ID); };// активируем совпавший АктионИн
     if (action.m_Active == true) // для активированных АктионИн
     {
       Events[action.m_EventID].m_probeAction(); // принтуем
              m_Queue.pop();  // удаляем ХК из очереди
        std::cout << "queue size: " << m_Queue.size() << std::endl <<  std::endl; 
     };
        }; 
 };
      mtx.unlock();
      };*/

    return 0;
}