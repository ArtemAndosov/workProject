#include <iostream>
#include <ctime>
#include <queue>

//#include <device.hpp>
//#include <HardCommand.hpp>
//#include <ActionIn.hpp>
#include <Event.hpp>

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
a.create_Hc();
b.create_Hc();
c.create_Hc();
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

// проверка ИД
/*std::cout <<  "size " << Events[0].m_actions.size() << std::endl; 
std::cout <<  "size " << Events[1].m_actions.size() << std::endl; 
std::cout <<  "Ev1 m_EventID " << Ev1.m_EventID << std::endl; 
std::cout <<  "Ev2 m_EventID " << Ev2.m_EventID << std::endl; 
std::cout <<  "m_EventID " << Events[0].m_actions[0].m_EventID << std::endl; 
std::cout <<  "m_EventID " << Events[1].m_actions[0].m_EventID << std::endl; 
std::cout <<  "m_EventID " << Events[1].m_actions[1].m_EventID << std::endl; */

std::cout << "queue size: " << m_Queue.size() << std::endl << std::endl; // проверяем что очередь не пуста


//поехал процесс

while (!m_Queue.empty())  // пока очередь не пуста
 {
  for (auto& event : Events){   // перебираем эвенты
    for (auto& action : event.m_actions){  //перебираем АктионИны
        action.m_probePacket(); // проверяем соответствие ИД из очереди и ИД девайса в АктионИне
      event.m_probeAction(); // принтуем
        m_Queue.pop();  // удаляем ХК из очереди
        std::cout << "queue size: " << m_Queue.size() << std::endl << std::endl; 
        }; 
    };
    
 };



   
//тест вывода
/*while (!m_Queue.empty())
{m_Queue.front().print();
m_Queue.pop();
std::cout << "queue size: " << m_Queue.size() << std::endl; 
}*/
  
  std::cout << "queue size: " << m_Queue.size() << std::endl;
  
   
    return 0;
}