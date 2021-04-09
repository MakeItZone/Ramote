#include <H4P_WiFi.h>
#include <H4P_AsyncMQTT.h>
#include "network.hh"
#include "status.hh"
#include "radar.hh"
#include "secrets.h"

using namespace std;

extern H4P_WiFi h4wifi;
extern H4P_AsyncMQTT h4mqtt;

void onWiFiConnect(void)
{
    // Serial.println("USER: WiFi connected!");
    // Serial.printf("USER: Connected SSID is %s\n", WiFi.SSID().c_str());
    // Serial.printf("USER: IP is %s\n", WiFi.localIP().toString().c_str());
    // do things on WiFi connect...
    // like reconnect to MQTT (not clear if MQTT auto-reconnect includes wifi failure)
    updateStatus(Status::running);
}

void onWiFiDisconnect(void)
{
    // do things on WiFi disconnect...
    // like shut down mqtt connections?
    updateStatus(Status::disconnected);
}

// Note, H4P has some utility functions for retrieving integers from topics and payloads
// (int PARAM(n), and int H4PAYLOAD_INT - see H4P_MQTT_Wildcards.ino in the examples for details.)
uint32_t myMqttCallback(vector<string> vs)
{
    //Serial.printf("USER: Msg received with %d parts and payload=%s\n", vs.size(), H4PAYLOAD.c_str()); // convert payload to C-style string
    //dumpvs(vs);
    if (vs.size() == 2) {
        if (vs[0]=="jog") {
            updateRadarJogCode(PARAM_INT(1));
            return H4_CMD_OK;
        } else if (vs[0]=="delay") {
            updateRadarDelayCode(PARAM_INT(1));
            return H4_CMD_OK;
        }
    }
    return H4_CMD_PAYLOAD_FORMAT;
}

void onMqttConnect()
{
    //Serial.print("USER: MQTT connected\n");
    // add any desired subscription topics
    //h4mqtt.subscribeDevice("topic", myMqttCallback);
    h4mqtt.subscribeDevice("config", myMqttCallback);    
    h4mqtt.subscribeDevice("config/jog", myMqttCallback);
    h4mqtt.subscribeDevice("config/delay", myMqttCallback);
    updateRadarMQTT();
}

void onMqttDisconnect()
{
    //Serial.print("USER: MQTT disconnected\n");
    // don't forget to unsubscribe from any topics subscribed to in connect
    //h4mqtt.unsubscribeDevice("topic");
    h4mqtt.unsubscribeDevice("config/delay");
    h4mqtt.unsubscribeDevice("config/jog");
    h4mqtt.unsubscribeDevice("config");    
}

void mqttPublish(string topic, string payload) {
    h4mqtt.publishDevice(topic, payload);
}
