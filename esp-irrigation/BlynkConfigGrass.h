
#ifndef BlynkConfigGrass_h
#define BlynkConfigGrass_h

const char blynk_auth[] = "he08DnzJLYbZI9F5WaKQZ2h5VWIYNPkF";    //home@hollo.cc - Irrigation Grass
const char* HOSTNAME = "ESP-grass";

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
