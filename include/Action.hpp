#pragma once
#include <HardwareRaw.hpp>
#include <device.hpp>
#include <includes.hpp>
#include <string>
/**
 * @brief базовый класс для ActionIn и ActionOut
 *
 */
class Action {
 public:
  int m_eventID;                                                  //!< ИД связанного события
  bool m_isActive{false};                                         //!< Флаг активности
  std::string m_direction;                                        //!< ActionIn или ActionOut
  std::string m_eventName;                                        //!< Имя связанного события
  std::string m_address;                                          //!< не используется
  std::string m_port;                                             //!< не используется
  std::string m_parameters;                                       //!< не используется
  device* m_pDevice;                                              //!< Указатель на связанный девайс
  hardwareRaw* m_pHardWareRaw;                                    //!< указатель на класс с исходными данными
  g_EInterfaceType m_interface{g_EInterfaceType::ERROR};          //!< Тип интерфейса
  enum class EStatus : uint8_t { open, closed, sleep, deleted };  //!< список статусов для Action
  EStatus m_status{EStatus::closed};                              //!< статус текущего Action
                                                                  /**
                                                                   * @brief Construct a new Action object
                                                                   *
                                                                   */
  Action() = default;
  /**
   * @brief Destroy the Action object
   *
   */
  ~Action() = default;
};
