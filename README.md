# About

IoT light sensor app. Light state is sent to a rest endpoint which saves it to Google Cloud
Monitoring.

I use this to monitor the light at my mom's house so I know if she's ok.

The app consists of 2 parts located in `src/`:
- esp8266_monitor. This is the sender built on ESP8266.
- http_iot_streamer. HTTP REST web service storing the data in Google Cloud Monitoring.


# esp8266_monitor

## Prerequisites:

- Copy `config.h.example` to `config.h` and edit all values marked with `CHANGEME`.
- https://github.com/plerup/makeEspArduino

## Build

    make

    make flash


# http_iot_streamer

## Prerequisites

- Service account named `svc-http-iot-streamer`.
- Copy `.env.yaml.example` to `.env.yaml` and edit the corresponding values. File is located in
`src/http_iot_streamer/`.

## Deploy

    ./scripts/deploy.sh

## Test

This will send a test payload to the deployed service:

    ./scripts/test.sh

