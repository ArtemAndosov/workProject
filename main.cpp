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

std::cout << "queue size: " << m_Queue.size() << std::endl; // проверяем что очередь не пуста


//поехал процесс
// TODO пока запускается вечный процесс(мб зацикливание)

while (!m_Queue.empty())  // пока очередь не пуста
    {
      for (auto action : Ev1.m_actions){  //бежим по АктионИнам в евенте1
        if (m_Queue.front().m_ID == action.dev->m_ID){ // если ИД девайса ХК в очереди совпал с ИД девайса в АктионИне в евенте1
action.m_lastCommand = &(m_Queue.front());  // Пишем ХК в Актион в евенте
        action.m_Active = true; // активируем АктионИн
Ev1.print(); // принтуем
        m_Queue.pop();  }; // удаляем ХК из очереди
    }};



//тест вывода
/*while (!m_Queue.empty())
{m_Queue.front().print();
m_Queue.pop();
std::cout << "queue size: " << m_Queue.size() << std::endl; 
}*/
  
  std::cout << "queue size: " << m_Queue.size() << std::endl;
  
   
    return 0;
}