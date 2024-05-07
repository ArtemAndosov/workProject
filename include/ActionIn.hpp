#include <device.hpp>
#include <HardCommand.hpp>

class ActionIn {
    public:
    int m_EventID;
    bool m_Active = false;
     device* dev;
HardCommand* m_lastCommand;
void m_probePacket(int id){ // копируем ХК из очереди в m_lastCommand при соответствии ИД
    if (id == dev->m_ID){ // если ИД девайса ХК в очереди совпал с ИД девайса в АктионИне
m_lastCommand = &(dev->m_pQueue->front());  // Пишем ХК в Актион в евенте
        m_Active = true; // активируем АктионИн
}};
ActionIn() = default;
~ActionIn()= default;
};
