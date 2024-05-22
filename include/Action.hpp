#include <device.hpp>
// #include <HardCommand.hpp>

class Action
{
public:
    int m_eventID;
    bool m_isActive;
    device *m_pDevice;
    Action() = default;
    ~Action() = default;
};
