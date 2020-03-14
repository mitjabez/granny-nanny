#include <CloudIoTCore.h>

#include "mqtt.h"

#define UPDATE_INTERVAL_MS 60000
#define PHOTO_PIN 2

bool lightWasOn = false;

void setup() {
    pinMode(PHOTO_PIN, INPUT);
    Serial.begin(115200);
    setupCloudIoT();
}

unsigned long lastMillis = 0;

void readPhotoPin() {
    bool lightPin = digitalRead(PHOTO_PIN);
    if (!lightWasOn && lightPin) {
        Serial.println("Light is ON");
    }
    lightWasOn |= lightPin;
}

String readingToJson() {
    String lightState = lightWasOn ? "true" : "false";
    return "{\"light\":" + lightState + ",\"uptime\":" + String(millis()) + "}";
}

void loop() {
    mqtt->loop();
    // <- fixes some issues with WiFi stability
    delay(10);

    if (!mqttClient->connected()) {
        ESP.wdtDisable();
        connect();
        ESP.wdtEnable(0);
    }

    readPhotoPin();

    if (millis() - lastMillis > UPDATE_INTERVAL_MS) {
        lastMillis = millis();
        String json = readingToJson();
        Serial.println("Sending: " + json);
        publishTelemetry(json);
        // Reset light state
        lightWasOn = false;
    }
}
