#include <ActionIn.hpp>
#include <vector>
class Event{
    public: 
    int m_EventID;   
 std::vector <ActionIn> m_actions;

// ф-ция принтует все активные ActionIn текущего Event
    void m_probeAction(const int id){
        for (auto& action : m_actions) 
        {
         action.m_probePacket(id);   // забираем ХК из очереди в m_lastCommand при соответствии ИД
    if (action.m_Active == true) // для активного
    {
    action.m_lastCommand->m_print(); // принтуем
    action.m_Active = false;  // дизактивируем
    if(!action.m_dev->m_pQueue->empty())  // если очередь не пустая
    //std::cout << "queue size: " << action.m_dev->m_pQueue->size() << std::endl <<  std::endl;  // если раскомментить эту строку, вылезет ошибка double free or corruption (out)
    action.m_dev->m_pQueue->pop(); //удаляем принтованный
           };};
    };

Event () = default;
~Event () = default;
};