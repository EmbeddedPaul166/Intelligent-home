#ifndef __ACTUATORS_H
#define __ACTUATORS_H

void handleRegulation(void);
void handleUART(void);
void heatingOn(void);
void coolingOn(void);
void temperatureRegulationOff(void);
void lightsOn(void);
void lightsOff(void);
void alarmOn(void);
void alarmOff(void);
void setTresholdValue(void);
void errorHandler(void);

#endif /*__ACTUATORS_H*/
