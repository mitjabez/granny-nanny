#!/bin/bash

main() {
  url=$(grep POST_URL src/esp8266_monitor/config.h | awk '{print $3}' | tr -d "\"")
  payload="{\"light\": true,\"uptime\":1234}"

  echo "Sending $payload to $url"
  curl -v --data "$payload" --header "content-type: application/json" "$url"
}

main
