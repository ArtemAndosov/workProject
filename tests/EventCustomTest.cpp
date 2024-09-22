#define CATCH_CONFIG_MAIN
#include <Event.hpp>
#include <EventCustom.hpp>
#include <catch2/catch_test_macros.hpp>
#include <includes.hpp>

TEST_CASE("задача 2", "[EventCustom]") {
  GIVEN("пустой ActionIn, связанный с Event") {
    eventRaw Raw;
    EventCustom Event(Raw);
    hardwareRaw HwRaw;
    ActionIn<HardCommand> Ain(HwRaw);
    Event.m_actions.emplace_back(&Ain);

    SECTION("setupPlugin") {
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

    SECTION("CRC8") {
      GIVEN("ss") {
        std::vector<uint8_t> vec{0x0, 0x6B, 0xC2, 0xDD, 0x3, 0xCD, 0x4C, 0x71, 0xEC, 0x0, 0xC8};
        THEN("") {
          REQUIRE(Event.m_actions[0]->m_status == Action::EStatus::closed);
        }

        WHEN("") {
          // Event.CRC8(&vec[0], 10);
          THEN("") {
            REQUIRE(Event.m_actions[0]->m_status == Action::EStatus::open);
          }
        }
      }
    }
  }
}
