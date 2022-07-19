#include "checkTemperatureAndAlert.h"
#include <stdio.h>

alertFunction fpSendAlert[2] = {sendToController,sendToEmail};

char * BreachInfo[]={"NORMAL","TOO_LOW","TOO_HIGH"};
char Alert_Info[50];

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  BreachType UpperBreachType;
  BreachType LowerBreachType;
  UpperBreachType = CheckLowerLimitBreach(value,lowerLimit);
  LowerBreachType = CheckUpperLimitBreach(value,upperLimit);
  return (BreachType)((int)UpperBreachType|(int)LowerBreachType);
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC)
{
  TemperatureLimits setTemperatureLimits;
  setTemperatureLimits.lowerLimit = 0;
  setTemperatureLimits.upperLimit = 0;

  setTemperatureLimits = SetPassiveCoolingLimit(coolingType,setTemperatureLimits);
  setTemperatureLimits = SetHiActiveCoolingLimit(coolingType,setTemperatureLimits);
  setTemperatureLimits = SetMedActiveCoolingLimit(coolingType,setTemperatureLimits);

  return inferBreach(temperatureInC,setTemperatureLimits.lowerLimit,setTemperatureLimits.upperLimit);
}

AlertTarget checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) 
{
  AlertTarget checkResultType;
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  checkResultType = print_AlertInfo(alertTarget,breachType,fpSendAlert);
  return(checkResultType);
}

AlertTarget print_AlertInfo(AlertTarget alertTarget,BreachType breachType,alertFunction *SendAlert)
{
  AlertTarget alertTarget;
  alertTarget =(*SendAlert[alertTarget])(breachType);
  if((breachType != NORMAL)||(alertTarget!=TO_CONTROLLER))
  {
    printf("%s",Alert_Info);
  }
  return(alertTarget);
}

AlertTarget sendToController(BreachType breachType)
{
  const unsigned short header = 0xfeed;
  Alert_Info[0]='\0';
  sprintf(Alert_Info,"%x : %x\n", header, breachType);
  return(TO_CONTROLLER);
}

AlertTarget sendToEmail(BreachType breachType)
{
  const char* recepient = "a.b@c.com";
  Alert_Info[0]='\0';
  sprintf(Alert_Info,"To: %s\nHi, the temperature is %s\n",recepient,BreachInfo[breachType]);
  return (TO_EMAIL);
}
