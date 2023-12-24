#!/bin/bash
#set -e

MY_DIR="$(dirname "$0")"
MY_NAME=$(basename "$0")

check_args() {
  if [ "$1" == "-h" ] || [ "$1" == "--help" ] || [ "$#" -ne 0 ]; then
    echo "Usage $MY_NAME"
    echo "Deploys the cloud function to GCP"
    exit 0
  fi
}

main() {
  check_args "$@"

  cd "$MY_DIR/../src/http_iot_streamer" || exit 1
  project=$(grep PROJECT .env.yaml)
  project=${project#* }
  echo "Starting deploy to project $project"

  gcloud run deploy http-iot-streamer \
    --source "." \
    --project "$project" \
    --region europe-west1 \
    --cpu 1 \
    --memory 128Mi \
    --max-instances 1 \
    --service-account "svc-http-iot-streamer@$project.iam.gserviceaccount.com" \
    --env-vars-file .env.yaml \
    --allow-unauthenticated
}

main "$@"
