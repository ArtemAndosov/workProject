#include <ActionIn.hpp>
#include <vector>
class Event{
    public: 
    int m_EventID;   
 std::vector <ActionIn> m_actions;

    void m_probeAction(const int id){
        for (auto& action : m_actions) //перебираем АктионИны
        {
         action.m_probePacket(id);   // если совпал - активируем
    if (action.m_Active == true) // для активного
    {
    action.m_lastCommand->m_print(); // принтуем
    action.m_Active = false;  // дизактивируем
    if(!action.m_dev->m_pQueue->empty())  // если очередь не пустая
   // std::cout << "queue size: " << action.m_dev->m_pQueue->size() << std::endl <<  std::endl;  // если раскомментить эту строку, вылезет ошибка double free or corruption (out)
    action.m_dev->m_pQueue->pop(); //удаляем принтованный
    std::cout << "queue size: " << action.m_dev->m_pQueue->size() << std::endl <<  std::endl;
        };};
    };

Event () = default;
~Event () = default;
};