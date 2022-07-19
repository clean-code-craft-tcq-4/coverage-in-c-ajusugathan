#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "checkTemperatureAndAlert.h"

TEST_CASE("Check the breach type for upper limit") 
{
  REQUIRE(CheckUpperLimitBreach(12, 30) == NORMAL);
  REQUIRE(CheckUpperLimitBreach(31, 30) == TOO_HIGH);
  REQUIRE(CheckUpperLimitBreach(30, 30) == NORMAL);  
}

TEST_CASE("Check the breach type for lower limit") 
{
  REQUIRE(CheckLowerLimitBreach(12, 20) == TOO_LOW);
  REQUIRE(CheckLowerLimitBreach(31, 20) == NORMAL);
  REQUIRE(CheckLowerLimitBreach(20, 20) == NORMAL);  
}

TEST_CASE("Check the limits of passive cooling") 
{
  TemperatureLimits passiveLimit={25,35};
  TemperatureLimits passiveLimit_expected={35,0};
  TemperatureLimits passiveLimit_return;
  passiveLimit_return=SetPassiveCoolingLimit(PASSIVE_COOLING,passiveLimit);
  REQUIRE(passiveLimit_return.upperLimit == passiveLimit_expected.upperLimit);
  REQUIRE(passiveLimit_return.lowerLimit == passiveLimit_expected.lowerLimit);
  passiveLimit_return=SetPassiveCoolingLimit(HI_ACTIVE_COOLING,passiveLimit);
  REQUIRE(passiveLimit_return.upperLimit == passiveLimit.upperLimit);
  REQUIRE(passiveLimit_return.lowerLimit == passiveLimit.lowerLimit); 
}
