#include <ESP8266WiFi.h>

#include "config.h"
#include "sender.h"
#include "timer.h"

bool isLightOn = false;
bool lightWasOn = false;
Timer lightOnTimer;
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

void setup() {
    pinMode(PHOTO_PIN, INPUT);
    Serial.begin(115200);
    initWifi();
    setupSender();
    lastHttpUpdateTimer.start();
}

void readPhotoPin() {
    bool previousState = isLightOn;
    isLightOn = digitalRead(PHOTO_PIN);

    if (isLightOn) {
        if (lightOnTimer.isStarted()) {
            if (lightOnTimer.elapsedMs() >= LIGHT_TRIGGER_MS) {
                Serial.println("Light is ON");
                lightWasOn = true;
            }
        } else {
            lightOnTimer.start();
        }
    } else {
        lightOnTimer.stop();

        if (previousState) {
            Serial.println("Light is OFF");
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

        lightWasOn = false;
        lightOnTimer.stop();
    }
}
