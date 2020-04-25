#include <ESP8266WiFi.h>

#include "config.h"
#include "sender.h"
#include "timer.h"

// Is light currently ON
bool isLightOn = false;
// If light was on during the last minute
bool lightWasOn = false;
Timer stateChangeTimer;
Timer lastHttpUpdateTimer;

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

void restartState() {
}

void setup() {
    pinMode(PHOTO_PIN, INPUT);
    Serial.begin(115200);
    initWifi();
    lastHttpUpdateTimer.start();
}

void toggleLight() {
    if (isLightOn) {
        Serial.println("Light is ON");
        lightWasOn = true;
    } else {
        Serial.println("Light is OFF");
    }
}

void readPhotoPin() {
    bool previousState = isLightOn;
    isLightOn = digitalRead(PHOTO_PIN);

    if (previousState != isLightOn) {
        stateChangeTimer.restart();
    } else {
        if (stateChangeTimer.isStarted() && stateChangeTimer.elapsedMs() >= LIGHT_TRIGGER_MS) {
            toggleLight();
            stateChangeTimer.stop();
        }
    }
}

void loop() {
    // Fix some issues with WiFi stability
    delay(10);

    readPhotoPin();

    if (lastHttpUpdateTimer.elapsedMs() > UPDATE_INTERVAL_MS) {
        lastHttpUpdateTimer.restart();

        sendStatus(lightWasOn, millis());

        lightWasOn = isLightOn;
    }
}
