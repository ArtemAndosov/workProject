#include <ActionIn.hpp>
#include <vector>
class Event{
    public:    
 std::vector <ActionIn> m_actions;
    void print(){
        for (auto action : m_actions){
if (action.m_Active == false)
action.m_lastCommand->print();
        };
    };
};