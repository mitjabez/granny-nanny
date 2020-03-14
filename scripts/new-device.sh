#!/bin/bash

me=$(basename "$0")
my_dir=$(dirname "$0")

die() {
  echo "[EXIT] $@" 1>&2
  exit 1
}

extract_variable_from_tf() {
  variable_name="$1"
  file_path="$2"
  grep "^\s\s${variable_name}\s*=" "${file_path}" | grep -o "\".*\"" | sed "s/\"//g"
}

printusage() {
  cat <<EOF
Usage: $me environment devicename

Add a new iot device to specific environment.


OPTIONS:
  environment        dev or prod
  devicename         name of the device

  -h --help          This help message.

EOF
}

if [ "$1" == "-h" ] || [ "$1" == "--help" ]; then
  printusage
  exit 0
fi

device_env="$1"
device_name="$2"

# Base path
cd "${my_dir}/../"

# Validate
test -z "$device_env" && die "specify device environment"
test -z "$device_name" && die "specify device name"

infra_path="./infra/environments/${device_env}"
device_path="./infra/environments/${device_env}/devices"
test -d "$infra_path" || die "infrastructure $device_env does not exist on $infra_path"
test -d "$device_path" || die "device path does not exist on $device_path"

# Extract variables
registry_name=$(extract_variable_from_tf name "${infra_path}/main.tf")
region_name=$(extract_variable_from_tf region "${infra_path}/main.tf")
project=$(extract_variable_from_tf project "${infra_path}/main.tf")

# Check if device already exists
found_device=$(gcloud --project "$project" iot devices list --registry "$registry_name" --region "$region_name" | grep "$device_name")
test -n "$found_device" && die "device $device_name already exists"

# Generate certificates
echo "Generating certificates:"
public_cert_path="${device_path}/${device_name}_public.pem"
private_cert_path="${device_path}/${device_name}_private.pem"

openssl ecparam -genkey -name prime256v1 -noout -out "$private_cert_path"
openssl ec -in "$private_cert_path" -pubout -out "$public_cert_path"

# Create devices
echo
echo "Creating device:"
gcloud iot --project "$project" devices create $device_name \
  --region=$region_name \
  --registry=$registry_name \
  --public-key=path=$public_cert_path,type=es256-pem
