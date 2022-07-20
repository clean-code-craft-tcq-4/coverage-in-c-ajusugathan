#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "string.h"
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

TEST_CASE("Check the limits of High Active cooling") 
{
  TemperatureLimits hiActiveLimit={25,35};
  TemperatureLimits hiActiveLimit_expected={45,0};
  TemperatureLimits hiActiveLimit_return;
  
  hiActiveLimit_return=SetHiActiveCoolingLimit(HI_ACTIVE_COOLING,hiActiveLimit);
  REQUIRE(hiActiveLimit_return.upperLimit == hiActiveLimit_expected.upperLimit);
  REQUIRE(hiActiveLimit_return.lowerLimit == hiActiveLimit_expected.lowerLimit);
  
  hiActiveLimit_return=SetHiActiveCoolingLimit(MED_ACTIVE_COOLING,hiActiveLimit);
  REQUIRE(hiActiveLimit_return.upperLimit == hiActiveLimit.upperLimit);
  REQUIRE(hiActiveLimit_return.lowerLimit == hiActiveLimit.lowerLimit); 
}

TEST_CASE("Check the limits of Medium Active cooling") 
{
  TemperatureLimits medActiveLimit={25,35};
  TemperatureLimits medActiveLimit_expected={40,0};
  TemperatureLimits medActiveLimit_return;
  
  medActiveLimit_return=SetMedActiveCoolingLimit(MED_ACTIVE_COOLING,medActiveLimit);
  REQUIRE(medActiveLimit_return.upperLimit == medActiveLimit_expected.upperLimit);
  REQUIRE(medActiveLimit_return.lowerLimit == medActiveLimit_expected.lowerLimit);
  
  medActiveLimit_return=SetMedActiveCoolingLimit(HI_ACTIVE_COOLING,medActiveLimit);
  REQUIRE(medActiveLimit_return.upperLimit == medActiveLimit.upperLimit);
  REQUIRE(medActiveLimit_return.lowerLimit == medActiveLimit.lowerLimit); 
}

TEST_CASE("infers the breach according to limits") 
{
  REQUIRE(inferBreach(9, 10, 25) == TOO_LOW);
  REQUIRE(inferBreach(30, 10, 25) == TOO_HIGH);
  REQUIRE(inferBreach(20, 10, 25) == NORMAL);
  REQUIRE(inferBreach(10, 10, 25) == NORMAL);
  REQUIRE(inferBreach(25, 10, 25) == NORMAL);
}

TEST_CASE("check the classification of temperature breach") 
{
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 45) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 35) == NORMAL);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 30) == NORMAL);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 0) == NORMAL);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, -1) == TOO_LOW);
  
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 50) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 45) == NORMAL);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 30) == NORMAL);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 0) == NORMAL);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1) == TOO_LOW);
  
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 50) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 40) == NORMAL);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 30) == NORMAL);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 0) == NORMAL);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1) == TOO_LOW);
  
  REQUIRE(classifyTemperatureBreach(DEFAULT_STATE, 45) == TOO_HIGH);
}

TEST_CASE("check the alert type for the breach") 
{
  BatteryCharacter battery_one = {PASSIVE_COOLING,"info"};
  REQUIRE(checkAndAlert(TO_CONTROLLER, battery_one, 40) == TO_CONTROLLER);
  REQUIRE(checkAndAlert(TO_EMAIL, battery_one, 40) == TO_EMAIL);
  
  BatteryCharacter battery_two = {HI_ACTIVE_COOLING,"info"};
  REQUIRE(checkAndAlert(TO_CONTROLLER, battery_two, 48) == TO_CONTROLLER);
  REQUIRE(checkAndAlert(TO_EMAIL, battery_two, 48) == TO_EMAIL);
  
  BatteryCharacter battery_three = {MED_ACTIVE_COOLING,"info"};
  REQUIRE(checkAndAlert(TO_CONTROLLER, battery_three, 45) == TO_CONTROLLER);
  REQUIRE(checkAndAlert(TO_EMAIL, battery_three, 45) == TO_EMAIL);
}

TEST_CASE("check the alert type and printing info") 
{
  alertFunction fpSendAlert_test[2] = {sendToController,sendToEmail};
  char msg[50]={"feed : 0\n"};
  REQUIRE(print_AlertInfo(TO_CONTROLLER,NORMAL,fpSendAlert_test) == TO_CONTROLLER);
  REQUIRE(strcmp(msg,Alert_Info)==0);
  
  char msg_one[50]={"feed : 1\n"};
  REQUIRE(print_AlertInfo(TO_CONTROLLER,TOO_LOW,fpSendAlert_test) == TO_CONTROLLER);
  REQUIRE(strcmp(msg_one,Alert_Info)==0);
  
  char msg_two[50]={"feed : 2\n"};
  REQUIRE(print_AlertInfo(TO_CONTROLLER,TOO_HIGH,fpSendAlert_test) == TO_CONTROLLER);
  REQUIRE(strcmp(msg_two,Alert_Info)==0);
  
  char EmailMsg[50]={"To: a.b@c.com\nHi, the temperature is NORMAL\n"};
  REQUIRE(print_AlertInfo(TO_EMAIL,NORMAL,fpSendAlert_test) == TO_EMAIL);
  REQUIRE(strcmp(EmailMsg,Alert_Info)==0);
  
  char EmailMsg_one[50]={"To: a.b@c.com\nHi, the temperature is TOO_LOW\n"};
  REQUIRE(print_AlertInfo(TO_EMAIL,TOO_LOW,fpSendAlert_test) == TO_EMAIL);
  REQUIRE(strcmp(EmailMsg_one,Alert_Info)==0);
  
  char EmailMsg_two[50]={"To: a.b@c.com\nHi, the temperature is TOO_HIGH\n"};
  REQUIRE(print_AlertInfo(TO_EMAIL,TOO_HIGH,fpSendAlert_test) == TO_EMAIL);
  REQUIRE(strcmp(EmailMsg_two,Alert_Info)==0);
}
