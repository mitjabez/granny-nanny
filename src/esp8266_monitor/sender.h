#ifndef SENDER_H
#define SENDER_H

#include <ESP8266HTTPClient.h>

#include "config.h"


HTTPClient https;

void setupSender() {
    // I know this is bad practice, but we're not sending top secret stuff here
}

String readingToJson(bool light, unsigned long uptime) {
    String lightBoolStr = light ? "true" : "false";
    return "{\"light\":" + lightBoolStr + ",\"uptime\":" + String(millis()) + "}";
}

void sendStatus(bool light, unsigned long uptime) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();

    if (https.begin(*client, "https://www.google.com")) {
//        https.setTimeout(TIMEOUT_SECONDS);
//        https.addHeader("Content-Type", "application/json");
//        https.setUserAgent(USER_AGENT);

        int responseCode = https.GET(); //https.POST(readingToJson(light, uptime));

        if (responseCode == HTTP_CODE_OK) {
            Serial.println("Request sent!");
        } else {
            Serial.printf("[HTTP] POST... failed, code: %d, error: %s\n", responseCode, https.errorToString(responseCode).c_str());
        }
    } else {
        Serial.println("Unable to connect!");
    }

    https.end();
}

#endif
