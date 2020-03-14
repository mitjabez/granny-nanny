#ifndef __ESP8266_MQTT_H__
#define __ESP8266_MQTT_H__
#include <ESP8266WiFi.h>
#include "FS.h"

// You need to set certificates to All SSL cyphers and you may need to
// increase memory settings in Arduino/cores/esp8266/StackThunk.cpp:
//   https://github.com/esp8266/Arduino/issues/6811
#include "WiFiClientSecureBearSSL.h"
#include <time.h>

#include <MQTT.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "ciotc_config.h" // Wifi configuration here

// !!REPLACEME!!
// The MQTT callback function for commands and configuration updates
// Place your message handler code here.
void messageReceived(String &topic, String &payload)
{
    Serial.println("incoming: " + topic + " - " + payload);
}
///////////////////////////////

// Initialize WiFi and MQTT for this board
MQTTClient *mqttClient;
BearSSL::WiFiClientSecure *netClient;
BearSSL::X509List certList;
CloudIoTCoreDevice *device;
CloudIoTCoreMqtt *mqtt;
unsigned long iat = 0;
String jwt;

///////////////////////////////
// Helpers specific to this board
///////////////////////////////
String getDefaultSensor()
{
    return "Wifi: " + String(WiFi.RSSI()) + "db";
}

String getJwt()
{
    // Disable software watchdog as these operations can take a while.
    ESP.wdtDisable();
    iat = time(nullptr);
    Serial.println("Refreshing JWT");
    jwt = device->createJWT(iat, jwt_exp_secs);
    ESP.wdtEnable(0);
    return jwt;
}

void setupCert()
{
    // Set CA cert on wifi client
    // If using a static (pem) cert, uncomment in ciotc_config.h:
    certList.append(primary_ca);
    certList.append(backup_ca);
    netClient->setTrustAnchors(&certList);

    return;
}

void setupWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
    }

    configTime(0, 0, ntp_primary, ntp_secondary);
    Serial.println("Waiting on time sync...");
    while (time(nullptr) < 1510644967)
    {
        delay(10);
    }
}

void connectWifi()
{
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
}

bool publishTelemetry(String data)
{
    return mqtt->publishTelemetry(data);
}

void connect()
{
    mqtt->mqttConnect();
}

void setupCloudIoT()
{
    // Create the device
    device = new CloudIoTCoreDevice(
            project_id, location, registry_id, device_id,
            private_key_str);

    // ESP8266 WiFi setup
    netClient = new WiFiClientSecure();
    setupWifi();

    // ESP8266 WiFi secure initialization
    setupCert();

    mqttClient = new MQTTClient(512);
    mqttClient->setOptions(180, true, 1000); // keepAlive, cleanSession, timeout
    mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
    mqtt->setUseLts(true);
    mqtt->startMQTT();
}

#endif
