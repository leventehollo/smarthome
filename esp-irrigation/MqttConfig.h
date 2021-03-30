#include <PubSubClient.h>

#define MQTT_SERVER "192.168.31.201"
#define MQTT_PORT 1883
#define MQTT_USERNAME "homeassistant"
#define MQTT_PASSWORD "eor3taefiowoomai0ag2heig3ahngaewoo8wiYah1zaLashaiTheeyegieyangai"

const int RECONNECT_DELAY = 5000;

WiFiClient wifiClient;
PubSubClient mqtt(MQTT_SERVER, MQTT_PORT, wifiClient);

void publishConnected() {
  const String topic = TOPIC_DEVICE_ROOT + TOPIC_CONNECTION;
  const char* value = "connected";
  mqtt.publish(topic.c_str() ,value);
  Serial.println("Publish "+topic+": "+value);
}

void publishStatus(const char* status) {
  mqtt.publish((TOPIC_DEVICE_ROOT + TOPIC_STATUS).c_str() ,status);
}

long mqttLastReconnectAttempt = 0;

boolean mqttReconnect() {
  Serial.println("Attempting MQTT connection to ..."+String(MQTT_SERVER));
  if (mqtt.connect("arduinoClient")) {
    Serial.println("Re-connected to MQTT "+String(MQTT_SERVER));
    publishConnected();
    //client.subscribe("inTopic");  //resubscribe
  }
  else {
    Serial.println("Failed to connect to MQTT "+String(MQTT_SERVER)+", try again in "+RECONNECT_DELAY+" seconds: ");
    Serial.print(mqtt.state());
  }
  return mqtt.connected();
}

void mqttCheckConnection() {
  if (!mqtt.connected()) {
    long now = millis();
    if (now - mqttLastReconnectAttempt > 5000) {
      mqttLastReconnectAttempt = now;
      if (mqttReconnect()) {      // Attempt to reconnect
        mqttLastReconnectAttempt = 0;
      }
    }
  } else {
    mqtt.loop();
  }
}
