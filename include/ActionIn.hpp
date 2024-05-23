// #include <HardCommand.hpp>
// #include <device.hpp>
#include <Action.hpp>
#include <mutex>
template <typename HardCommandType>
class ActionIn : public Action
{
public:
  HardCommandType m_pLastCommand;
  enum class EStatus : uint8_t
  {
    open,
    closed,
    sleep,
    deleted
  };
  EStatus m_status{EStatus::open};

  bool probePacket(const HardCommandType &HC)
  {
    if (m_status == EStatus::open)
    {
      if (HC.m_pDevice->m_deviceName == m_pDevice->m_deviceName) // если ИД девайса ХК в очереди совпал с ИД девайса в АктионИне
      {
        m_pLastCommand = HC; // Пишем ХК в Актион в евенте
        m_isActive = true;
        return true;
      }
      return false;
    };
    return false;
  }

  ActionIn() = default;
  ~ActionIn() = default;
};
