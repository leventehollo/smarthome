#define BLYNK_PRINT Serial  // Comment this out to disable prints and save space
#define BLYNK_DEBUG

extern "C" {
#include "user_interface.h"
}

const boolean DEBUG = true;
const String VERSION = "1.3";

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
const char* HOSTNAME = "ESP-greenhouse";
const char* ssid = "grabowski";
const char* password = "Daisy160601";

#include <ESP8266WebServer.h>
const int WEB_SERVER_PORT = 80;
ESP8266WebServer server(WEB_SERVER_PORT);

#include <ArduinoJson.h>
const size_t JSON_DOC_CAPACITY = JSON_OBJECT_SIZE(1);

#include <SoftwareSerial.h>
const int SERIAL_DATA_RATE = 9600;
//const int SERIAL_DATA_RATE = 115200;
SoftwareSerial mySerial(3, 2); // RX, TX

#include <BlynkSimpleEsp8266.h>
//char blynk_auth[] = "rpHU4_BDa053QAIRKy-GStguhZid1FzF";    //home@hollo.cc - Irrigation Greenhouse
char blynk_auth[] = "he08DnzJLYbZI9F5WaKQZ2h5VWIYNPkF";    //home@hollo.cc - Irrigation Grass

const uint8_t PIN_TERMINAL = V0;
const uint8_t PIN_LED1 = V1;
WidgetTerminal terminal(PIN_TERMINAL);
WidgetLED LED1(PIN_LED1);

#include <SimpleTimer.h>
SimpleTimer timer;


const int GPIO_SIZE = 9;
int D[] = {16, 5, 4, 0, 2, 14, 12, 13, 15};

void setup() {
  Serial.begin(SERIAL_DATA_RATE);
  Serial.setDebugOutput(DEBUG);
  delay(10);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  mySerial.begin(SERIAL_DATA_RATE);
  timer.setInterval(500, Sent_serial);


  Blynk.begin(blynk_auth, ssid, password);
  while (Blynk.connect() == false) {}

  initPins();

  connectToWifi();
  setupOTA();

  defineServerEndpoints();

  server.begin();
  Serial.println("Server started at: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());

  Serial.println("version: " + VERSION);
}

void loop() {
  ArduinoOTA.handle();
  Blynk.run();
  server.handleClient();
}

void defineServerEndpoints() {
  server.on("/", []() {
    server.send(200, "text/html", "OK");
  });

  server.on("/version", []() {
    server.send(200, "text/html", VERSION);
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
        if (server.arg("mode") == "ON")  {
          digitalWrite(D[pin], LOW);
          LED1.on();
        }
        if (server.arg("mode") == "OFF")  {
          digitalWrite(D[pin], HIGH);
          LED1.off();
        }
        server.send(200, "text/json", pinsJson());
      }
    }
    delay(1000);
  });
}

void initPins() {
  Serial.println("There are " + String(GPIO_SIZE) + " pins configured:");
  for (int i = 0; i < GPIO_SIZE; i++) {
    pinMode(D[i], OUTPUT);
    digitalWrite(D[i], HIGH);
    Serial.println("D[" + String(i) + "]: " + String(D[i]));
  }
}

String pinsJson() {
  StaticJsonDocument<JSON_DOC_CAPACITY> doc;
  JsonObject object = doc.as<JsonObject>();
  for (int i = 0; i < GPIO_SIZE; i++) {
    int pinValue = digitalRead(D[i]);
    object[String(i)] = String(pinValue);
  };
  String response;
  serializeJsonPretty(doc, response);
  return response;
}

String gpioJson() {
  StaticJsonDocument<JSON_DOC_CAPACITY> doc;
  JsonObject object = doc.as<JsonObject>();
  for (int i = 0; i < GPIO_SIZE; i++) {
    object[String(i)] = D[i];
  };
  String response;
  serializeJsonPretty(doc, response);
  serializeJsonPretty(doc, Serial);
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

void connectToWifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  wifi_station_set_hostname(HOSTNAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("");
  Serial.println("WiFi connected");
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
}

void httpJsonResponseHeader(WiFiClient client, int code, String response) {
  client.println("HTTP/1.1 " + String(code) + " " + response);
  client.println("Content-Type: text/json");
  client.println("");                             //  do not forget this one
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
