from google.cloud import monitoring_v3

import time
import base64
import json

PROJECT = '***REMOVED***'

def write_metric(device_id: str, registry_id: str, light: bool):
    client = monitoring_v3.MetricServiceClient()
    project_name = client.project_path(PROJECT)

    series = monitoring_v3.types.TimeSeries()
    series.metric.type = 'custom.googleapis.com/light'
    series.resource.type = 'global'
    series.metric.labels['deviceId'] = device_id
    series.metric.labels['registryId'] = registry_id
    point = series.points.add()
    point.value.int64_value = 1 if light else 0
    now = time.time()
    point.interval.end_time.seconds = int(now)
    point.interval.end_time.nanos = int(
        (now - point.interval.end_time.seconds) * 10**9)
    client.create_time_series(project_name, [series])
    print('Successfully wrote time series.')



def iot_streamer(event, context):
    device_id = event['attributes']['deviceId']
    registry_id = event['attributes']['deviceRegistryId']
    payload = base64.b64decode(event['data']).decode('utf-8')
    print(f'Got payload: {payload}');
    decoded_payload = json.loads(payload)
    light = decoded_payload['light']
    write_metric(device_id=device_id, registry_id=registry_id, light=light)

