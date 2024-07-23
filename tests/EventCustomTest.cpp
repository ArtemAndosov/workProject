#define CATCH_CONFIG_MAIN
#include <Event.hpp>
#include <EventCustom.hpp>
#include <catch2/catch_test_macros.hpp>
#include <includes.hpp>

SCENARIO("setupPlugin test", "[EventCustom]") {
  GIVEN("пустой ActionIn, связанный с Event") {
    eventRaw Raw;
    EventCustom Event(Raw);
    hardwareRaw HwRaw;
    ActionIn<HardCommand> Ain(HwRaw);
    Event.m_actions.emplace_back(&Ain);

    THEN("статус ActionIn закрыто по умолчанию") {
      REQUIRE(Event.m_actions[0]->m_status == Action::EStatus::closed);
    }

    WHEN("вызываем setupPlugin()") {
      Event.setupPlugin();
      THEN("статус ActionIn стал открыто") {
        REQUIRE(Event.m_actions[0]->m_status == Action::EStatus::open);
      }
    }
  }
}
