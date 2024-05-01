#include <device.hpp>
#include <HardCommand.hpp>

class ActionIn {
    public:
    bool m_Active = false;
     device* dev;
HardCommand* m_lastCommand;
ActionIn() = default;
~ActionIn()= default;
};
