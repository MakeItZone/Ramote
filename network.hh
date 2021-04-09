#ifndef network_h
#define network_h
#include <Arduino.h>

using namespace std;

void onWiFiConnect(void);
void onWiFiDisconnect(void);
void onMqttConnect(void);
void onMqttDisconnect(void);
void mqttPublish(string topic, string payload );
#endif
