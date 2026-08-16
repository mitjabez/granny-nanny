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

## Run locally

Create a virtual environment and install the service dependencies:

    cd src/http_iot_streamer
    python3 -m venv .venv
    source .venv/bin/activate
    python -m pip install -r requirements.txt

Set the required configuration values, then start the service:

    export API_KEY=local-api-key
    export ENVIRONMENT=local
    export PROJECT=your-gcp-project-id
    python main.py

The service listens on `http://localhost:8080`. To send it a test request:

    curl --data '{"light": true}' --header 'content-type: application/json' \
      'http://localhost:8080/light/status?apikey=local-api-key'

Valid requests write a metric to Google Cloud Monitoring, so local runs need Google
Cloud credentials that can write metrics to the selected project.

## Test

This will send a test payload to the deployed service:

    ./scripts/test.sh
