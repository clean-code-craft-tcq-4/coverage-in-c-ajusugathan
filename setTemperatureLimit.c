#include "checkTemperatureAndAlert.h"

BreachType CheckUpperLimitBreach(double value, double upperLimit)
{
  BreachType UpperBreachType =NORMAL;
  if(value > upperLimit)
  {
    UpperBreachType = TOO_HIGH;
  }
  return(UpperBreachType);
}

BreachType CheckLowerLimitBreach(double value, double lowerLimit)
{
  BreachType LowerBreachType =NORMAL;
  if(value < lowerLimit)
  {
    LowerBreachType = TOO_LOW;
  }
  return(LowerBreachType);
}

TemperatureLimits SetPassiveCoolingLimit(CoolingType coolingTyp,TemperatureLimits passiveCoolingLimit)
{
	if(coolingTyp == PASSIVE_COOLING)
	{
		passiveCoolingLimit.upperLimit=35;
		passiveCoolingLimit.lowerLimit=0;
	}
	return(passiveCoolingLimit);
}
TemperatureLimits SetHiActiveCoolingLimit(CoolingType coolingTyp,TemperatureLimits HiActiveCoolingLimit)
{
	if(coolingTyp == HI_ACTIVE_COOLING)
	{
		HiActiveCoolingLimit.upperLimit=45;
		HiActiveCoolingLimit.lowerLimit=0;
	}
	return(HiActiveCoolingLimit);
}
TemperatureLimits SetMedActiveCoolingLimit(CoolingType coolingTyp,TemperatureLimits MedActiveCoolingLimit)
{
	if(coolingTyp == MED_ACTIVE_COOLING)
	{
		MedActiveCoolingLimit.upperLimit=40;
		MedActiveCoolingLimit.lowerLimit=0;
	}
	return(MedActiveCoolingLimit);
}