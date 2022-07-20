

typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

BreachType inferBreach(double value, double lowerLimit, double upperLimit);
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);

typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

typedef struct{
	int upperLimit;
	int lowerLimit;
}TemperatureLimits;

typedef AlertTarget (*alertFunction) (BreachType breachType);
extern char Alert_Info[50];

AlertTarget sendToController(BreachType breachType);
AlertTarget sendToEmail(BreachType breachType);
BreachType CheckUpperLimitBreach(double value, double upperLimit);
BreachType CheckLowerLimitBreach(double value, double lowerLimit);
TemperatureLimits SetPassiveCoolingLimit(CoolingType coolingTyp,TemperatureLimits passiveCoolingLimit);
TemperatureLimits SetHiActiveCoolingLimit(CoolingType coolingTyp,TemperatureLimits HiActiveCoolingLimit);
TemperatureLimits SetMedActiveCoolingLimit(CoolingType coolingTyp,TemperatureLimits MedActiveCoolingLimit);
AlertTarget checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);
AlertTarget print_AlertInfo(AlertTarget alertTarget,BreachType breachType,alertFunction *SendAlert);
