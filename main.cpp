#include <iostream>
#include <ctime>
#include <queue>
#include <Event.hpp>
#include <chrono>
#include <thread>
#include <mutex>
//тест
std::mutex mtx;
int main (){
//создали очередь
   std::queue<HardCommand> m_Queue;
   std::vector<Event> Events;
// создали девайсы 
    device a(1);
     a.m_pQueue = &m_Queue;
    device b(2);
     b.m_pQueue = &m_Queue;
    device c(3);
     c.m_pQueue = &m_Queue;
// создали хк
  std::thread create_Hc([&](){while (true){
    mtx.lock();
  a.create_Hc();
b.create_Hc();
c.create_Hc();
mtx.unlock();
std::this_thread::sleep_for(std::chrono::seconds(1));
};});
/*a.create_Hc();
b.create_Hc();
c.create_Hc();
c.create_Hc();
b.create_Hc();
a.create_Hc();*/

// создали экшнъины
ActionIn dev1;
  dev1.dev = &a;
//dev1.m_lastCommand = &(m_Queue.front());
//m_Queue.pop();
ActionIn dev2;
  dev2.dev = &b;
ActionIn dev3;
  dev3.dev = &c;

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

//std::cout << "queue size: " << m_Queue.size() << std::endl << std::endl; // проверяем что очередь не пуста

//поехал процесс
std::this_thread::sleep_for(std::chrono::milliseconds(1100));
while (true)  // пока очередь не пуста
 {
   mtx.lock();
  for (auto& event : Events){   // перебираем эвенты
    for (auto& action : event.m_actions){  //перебираем АктионИны
         action.m_probePacket(m_Queue.front().m_ID); // проверяем соответствие ИД из очереди и ИД девайса в АктионИне
     if (action.m_Active == true){
      event.m_probeAction(); // принтуем
        m_Queue.pop();  // удаляем ХК из очереди
        std::cout << "queue size: " << m_Queue.size() << std::endl <<  std::endl; 
     };
   }; 
 };
        mtx.unlock();
 };
create_Hc.join();
   
//тест вывода
/*while (!m_Queue.empty())
{m_Queue.front().print();
m_Queue.pop();
std::cout << "queue size: " << m_Queue.size() << std::endl; 
}*/
  
 // std::cout << "queue size: " << m_Queue.size() << std::endl;
  
   
    return 0;
}