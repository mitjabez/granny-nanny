#ifndef SENDER_H
#define SENDER_H

#include <ESP8266HTTPClient.h>

#include "config.h"

HTTPClient https;

String readingToJson(bool light, unsigned long uptime) {
    String lightBoolStr = light ? "true" : "false";
    return "{\"light\":" + lightBoolStr + ",\"uptime\":" + String(millis()) + "}";
}

void sendStatus(bool light, unsigned long uptime) {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    // For production, configure your certificates
    client->setInsecure();

    if (https.begin(*client, POST_URL)) {
        https.setTimeout(TIMEOUT);
        https.addHeader("Content-Type", "application/json");
        https.setUserAgent(USER_AGENT);

        int responseCode = https.POST(readingToJson(light, uptime));

        if (responseCode == HTTP_CODE_OK) {
            Serial.printf("Request sent: %d\n", responseCode);
        } else {
            Serial.printf("[HTTP] POST... failed, code: %d, error: %s\n", responseCode, https.errorToString(responseCode).c_str());
        }
    } else {
        Serial.println("Unable to connect!");
    }

    https.end();
}

#endif
