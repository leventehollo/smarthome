/*
  Rest.h - Library for exposing important things via REST API.
*/
#ifndef BlynkConfigGreenhouse_h
#define BlynkConfigGreenhouse_h

const char blynk_auth[] = "rpHU4_BDa053QAIRKy-GStguhZid1FzF";    //home@hollo.cc - Irrigation Greenhouse
const char* HOSTNAME = "ESP-greenhouse";

const uint8_t PIN_TERMINAL = V0;
const uint8_t PIN_LED1 = V1;

WidgetTerminal terminal(PIN_TERMINAL);
WidgetLED LED1(PIN_LED1);

#endif
