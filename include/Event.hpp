#include <ActionIn.hpp>
#include <vector>
class Event{
    public: 
    int m_EventID;   
 std::vector <ActionIn> m_actions;
    void m_probeAction(){
        for (auto& action : m_actions){
if (action.m_Active == true)
action.m_lastCommand->print();
action.m_Active = false;
        };
    };
};