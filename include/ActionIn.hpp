// #include <HardCommand.hpp>
// #include <device.hpp>
#include <Action.hpp>
#include <mutex>
template <typename HardCommandType>
class ActionIn : public Action
{
public:
  HardCommandType m_pLastCommand;

  bool m_probePacket(const HardCommand &HC)
  {
    if (HC.m_pDevice->m_deviceID == m_pDevice->m_ID) // если ИД девайса ХК в очереди совпал с ИД девайса в АктионИне
    {
      m_pLastCommand = HC; // Пишем ХК в Актион в евенте
      m_isActive = true;
      return true;
    }
    return false;
  };

  ActionIn() = default;
  ~ActionIn() = default;
};
