#ifndef radar_h
#define radar_h
#include<H4P_GPIOManager.h>

using namespace std;

void updateRadarMQTT();

void updateRadarConfig();
void updateRadarJogCode(uint8_t jogCode);
void updateRadarDelayCode(uint8_t delayCode);

void radarTriggerCallback(H4GPIOPin* ptr);

void getRadarState();

#endif