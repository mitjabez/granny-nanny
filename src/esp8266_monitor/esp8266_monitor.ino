#include <ESP8266WiFi.h>

#include "config.h"
#include "sender.h"

bool lightWasOn = false;
unsigned long lastMillis = 0;

void initWifi() {
    WiFi.begin(SSID, PSK);

    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected! IP address: ");
    Serial.println(WiFi.localIP());
}

void setup() {
    pinMode(PHOTO_PIN, INPUT);
    Serial.begin(115200);
    initWifi();
    setupSender();
}

void readPhotoPin() {
    bool lightPin = digitalRead(PHOTO_PIN);
    if (!lightWasOn && lightPin) {
        Serial.println("Light is ON");
    }
    lightWasOn |= lightPin;
}

void loop() {
    // <- fixes some issues with WiFi stability
    delay(10);

    readPhotoPin();

    if (millis() - lastMillis > UPDATE_INTERVAL_MS) {
        lastMillis = millis();
        sendStatus(lightWasOn, millis());
        // Reset light state
        lightWasOn = false;
    }
}
