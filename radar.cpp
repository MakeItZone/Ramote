#include "radar.hh"
#include "network.hh"

using namespace std;

extern H4P_GPIOManager h4gm; 

const int config_start_code = 0xCC;
// jog code 1 is most sensitive, and 10 is least
uint8_t config_jog_code = 4;
uint8_t config_delay_code = 0;

uint8_t begin_code;
uint8_t state_code;
uint8_t jog_code;
uint8_t delay_code;
uint8_t check_code;

bool radarState = false;

void updateRadarValueMQTT(string topic, uint8_t value) {
    char buf[4]; // max two digits
    snprintf(buf, 4, "%d", value);
    string status = string(buf);
    mqttPublish(topic, status);
}

void updateRadarValuesMQTT() {
    updateRadarValueMQTT("begin_code", begin_code);
    updateRadarValueMQTT("state_code", state_code);
    updateRadarValueMQTT("jog_code", jog_code);
    updateRadarValueMQTT("delay_code", delay_code);
    updateRadarValueMQTT("check_code", check_code);    
}

void updateRadarConfigMQTT() {
    updateRadarValueMQTT("config_jog_code", config_jog_code);
    updateRadarValueMQTT("config_delay_code", config_delay_code);
}

void updateRadarConfig() {
    const int config_start_code = 0xCC;
    int config_check_code = config_start_code+config_jog_code+config_delay_code;

    Serial.write(config_start_code);
    delay(1);
    Serial.write(config_jog_code);
    delay(1);
    Serial.write(config_delay_code);
    delay(1);
    Serial.write(config_check_code);

    updateRadarConfigMQTT();
}

void updateRadarJogCode(uint8_t jogCode) {
    config_jog_code = jogCode;
    updateRadarConfig();
}

void updateRadarDelayCode(uint8_t delayCode){
    config_delay_code = delayCode;
    updateRadarConfig();
}


void updateRadarSenseMQTT() {
    string topic = string("status");
    string status;

    if (radarState) {
        status = string("triggered");
    } else {
        status = string("clear");
    }
    mqttPublish(topic, status);
}

void updateRadarMQTT(){
    updateRadarSenseMQTT();
    updateRadarValuesMQTT();
    updateRadarConfigMQTT();
}

void radarTriggerCallback(H4GPIOPin* ptr){
    H4GM_PIN(Retriggering); // Create the correct pointer type in 'pin'
    h4gm.logicalWrite(LED_BUILTIN,pin->state);
    radarState = pin->state;
    updateRadarSenseMQTT();
}

void getRadarState() {
    if (Serial.available()){
        //Serial.println("data be ready to present");
        uint8_t input_begin_code = Serial.read();
        delay(10);
        uint8_t input_state_code = Serial.read();
        delay(10);
        uint8_t input_jog_code = Serial.read();
        delay(10);
        uint8_t input_delay_code = Serial.read();
        delay(10);
        uint8_t input_check_code = Serial.read();

        if (input_begin_code == 0xAA) {
            begin_code = input_begin_code;
            state_code = input_state_code;
            jog_code = input_jog_code;
            delay_code = input_delay_code;
            check_code = input_check_code;
            updateRadarValuesMQTT();
        } else {
            while(Serial.read()>=0); //clear any garbled data
        }
    }
}
