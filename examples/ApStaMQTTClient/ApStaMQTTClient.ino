/*
  ApStaMQTTClient.ino
  The purpose of this exemple is to illustrate a simple handling of MQTT and Wifi connection with the WIFI_APSTA mode.
  Once it connects successfully to a Wifi network and a MQTT broker, it subscribe to a topic and send a message to it.
  It will also send a message delayed 5 seconds later.
*/

#include "EspMQTTClient.h"

EspMQTTClient client;

void setup()
{
  Serial.begin(115200);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
  client.setWifiCredentials("WifiSSID","WifiPassword");
  client.setMqttClientName("TestClient");
  client.setMqttServer("192.168.1.100", "MQTTUserName", "MQTTPassword", 1883); // 192.168.1.100 is the IP address of the MQTT server
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("mytopic/test", [](const String & payload) {
    Serial.println(payload);
  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
    Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
  });

  // Publish a message to "mytopic/test"
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("mytopic/wildcardtest/test123", "This is a message sent 5 seconds later");
  });

  createAccessPoint();
}

// This function creates an Access Point
void createAccessPoint()
{
  if(WiFi.softAP("APName", "APPassword"))
  {
    Serial.println("Access Point created");
  }
  else
  {
    Serial.println("Unable to create Access Point");
  }
}

void loop()
{
  client.loop(WIFI_AP_STA);
}
