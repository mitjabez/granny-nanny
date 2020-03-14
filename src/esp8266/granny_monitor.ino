#include <CloudIoTCore.h>

#include "esp8266_mqtt.h"

void setup()
{
    Serial.begin(115200);
    setupCloudIoT();
}

unsigned long lastMillis = 0;

void loop()
{
    mqtt->loop();
    // <- fixes some issues with WiFi stability
    delay(10);

    if (!mqttClient->connected())
    {
        ESP.wdtDisable();
        connect();
        ESP.wdtEnable(0);
    }

    if (millis() - lastMillis > 60000)
    {
        lastMillis = millis();
        publishTelemetry(getDefaultSensor());
    }
}
