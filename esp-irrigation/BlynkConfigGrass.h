
#ifndef BlynkConfigGrass_h
#define BlynkConfigGrass_h

const char blynk_auth[] = "he08DnzJLYbZI9F5WaKQZ2h5VWIYNPkF";    //home@hollo.cc - Irrigation Grass
#define HOSTNAME "ESP-grass"

// LOCATION/FLOOR/ROOM/DEVICE/SENSOR
#define LOCATION "home/"
#define FLOOR "garden/"
#define ROOM "grass/"

#define TOPIC_CONNECTION "/connection"
#define TOPIC_STATUS "/status"
#define TOPIC_VALVE1 "/valve1"
#define TOPIC_VALVE2 "/valve2"
#define TOPIC_VALVE3 "/valve3"
#define TOPIC_VALVE4 "/valve4"
#define TOPIC_VALVE5 "/valve5"
#define TOPIC_VALVE6 "/valve6"
#define TOPIC_VALVE7 "/valve7"
#define TOPIC_VALVE8 "/valve8"

const String TOPIC_DEVICE_ROOT = LOCATION FLOOR ROOM HOSTNAME;

const uint8_t PIN_TERMINAL = V0;
WidgetTerminal terminal(PIN_TERMINAL);

const uint8_t PIN_RESTART = V1;

int virtualPins[] = {};
const uint8_t PIN_RELAY_01 = V11;


BLYNK_CONNECTED() {
  // Request the latest state from the server
  //Blynk.syncVirtual(V2);     
       
  // Alternatively, you could override server state using:
  Blynk.virtualWrite(PIN_RESTART, HIGH); 
}

// When App button is pushed - switch the state
BLYNK_WRITE(PIN_RELAY_01) {
  virtualPins[PIN_RELAY_01] = param.asInt();
  Serial.println("virtual pin1 changed:"+String(virtualPins[PIN_RELAY_01]));
}

#endif
