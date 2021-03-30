#define BLYNK_PRINT Serial  // Comment this out to disable prints and save space
#define BLYNK_DEBUG

extern "C" {
#include "user_interface.h"
}

const String VERSION = "1.9";
const String VERSION_DATE = "2021";


#include <ArduinoJson.h>

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>

#include <WiFiUdp.h>

#include <BlynkSimpleEsp8266.h>

#include "BlynkConfigGreenhouse.h"
//#include "BlynkConfigGrass.h"

BlynkTimer timer;

#include "MqttConfig.h"
#include "Device_Lolin_NodeMCU_v3.h"
#include "MyEnv.h"
#include "MyServer.h"

#include <ESP8266WebServer.h>
ESP8266WebServer server(WEB_SERVER_PORT);

#include <SoftwareSerial.h>

//SoftwareSerial mySerial(3, 2); // RX, TX

void msgCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(SERIAL_DATA_RATE);
  Serial.setDebugOutput(DEBUG);
  delay(10);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //mySerial.begin(SERIAL_DATA_RATE);
  timer.setInterval(1000, checkVirtualPins);

  Blynk.begin(blynk_auth, ssid, password);
  while (Blynk.connect() == false) {}
  setupOTA();

  mqtt.setCallback(msgCallback);
  if (mqtt.connect(HOSTNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
    log("Connected to MQTT: " + String(MQTT_SERVER));
    publishConnected();
    mqtt.subscribe("inTopic");
  }
  Serial.println("rising star: "+String(RISING) + " " + String(FALLING) + " " + String(CHANGE));

  publishStatus("setting_up");
  initPins();
  attachInterrupts();

  defineServerEndpoints();

  server.begin();
  Serial.println("Server started at: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("Version: " + VERSION);
  Serial.println("Released: " + VERSION_DATE);
  publishStatus("setup_complete");
}


void loop() {
  ArduinoOTA.handle();
  Blynk.run();
  server.handleClient();
  mqttCheckConnection();
}

void defineServerEndpoints() {
  server.on("/", []() {
    server.send(200, "text/html", "OK");
  });

  server.on("/version", []() {
    server.send(200, "text/html", VERSION + " " + VERSION_DATE);
  });

  server.on("/gpio", []() {
    String pinParam = server.arg("pin");
    if (pinParam == "") {
      server.send(200, "text/json", gpioJson());
      delay(1000);
    } else {
      if (!isNumber(pinParam)) {
        server.send(400, "text/json", messageJson("PIN " + pinParam + " is not a number"));
      }
      else if (pinParam.toInt() > GPIO_SIZE - 1) {
        server.send(400, "text/json", messageJson("PIN " + pinParam + " is out of bound (There are " + String(GPIO_SIZE)) + " GPIOs configured");
      }
      else {
        int pin = pinParam.toInt();
        int previous_state = digitalRead(PINS[0][pin]);
        int new_state = -1;
        if (server.arg("mode") == "ON")  {
          digitalWrite(PINS[0][pin], LOW);
          new_state = LOW;
        }
        if (server.arg("mode") == "OFF")  {
          digitalWrite(PINS[0][pin], HIGH);
          new_state = HIGH;
        }
        server.send(200, "text/json", pinsJson(pin, previous_state, new_state));
      }
    }
    delay(1000);
  });
}

void initPins() {
  INPUT_PULLDOWN_16;
  log("There are " + String(GPIO_SIZE) + " pins configured:");
  for (int i = 0; i < GPIO_SIZE; i++) {
    pinMode(PINS[0][i], OUTPUT);
    digitalWrite(PINS[0][i], HIGH);
    log("PINS[0][" + String(i) + "]: " + String(PINS[0][i]));
  }
}

String pinsJson(int pin_changed, int previous_state, int new_state) {
  StaticJsonDocument<JSON_DOC_CAPACITY> doc;
  JsonObject object = doc.to<JsonObject>();
  JsonObject pins = object.createNestedObject("pins");
  object["pin_changed"] = pin_changed;
  object["gpio_changed"] = PINS[0][pin_changed];
  object["previous_state"] = previous_state;
  object["new_state"] = new_state;

  for (int i = 0; i < GPIO_SIZE; i++) {
    int pinValue = digitalRead(PINS[0][i]);
    pins[String(i)] = String(pinValue);
  };
  String response;
  serializeJsonPretty(doc, response);
  return response;
}

String gpioJson() {
  StaticJsonDocument<JSON_DOC_CAPACITY> doc;
  JsonObject object = doc.to<JsonObject>();
  object["GPIO_SIZE"] = GPIO_SIZE;
  JsonObject pins = object.createNestedObject("pins");
  for (int i = 0; i < GPIO_SIZE; i++) {
    pins[String(i)] = PINS[0][i];
  };
  String response;
  serializeJsonPretty(doc, response);
  return response;
}

String messageJson(String msg) {
  StaticJsonDocument<JSON_DOC_CAPACITY> doc;
  JsonObject object = doc.as<JsonObject>();
  object["message"] = msg;
  String response;
  serializeJsonPretty(doc, response);
  return response;
}

void setupOTA() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  ArduinoOTA.setHostname(HOSTNAME);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

/*
  // Subrutine ( Sent_serial ) data to blynk terminal widget
  void Sent_serial() {
  // Sent serial data to Blynk terminal - Unlimited string readed
  String content = "";  //null string constant ( an empty string )
  char character;
  while (mySerial.available()) {
    character = mySerial.read();
    content.concat(character);
    Serial.println();
  }
  if (content != "") {
    Blynk.virtualWrite(PIN_TERMINAL, content);
  }
  }*/

void checkVirtualPins() {
  if (virtualPins[PIN_RELAY_01]) {
    log("V11 is on");
    Serial.println("asd");

  }
  else {
    log("V11 is off");
    Serial.println("dfg");
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

boolean isNumber(String str) {
  for (byte i = 0; i < str.length(); i++)
  {
    if (isDigit(str.charAt(i))) return true;
  }
  return false;
}

void log(String msg) {
  Serial.println(msg);
  /*if (Blynk.connected()) {
    Blynk.virtualWrite(PIN_TERMINAL, msg);
    }*/
}
