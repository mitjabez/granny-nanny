from google.cloud import monitoring_v3

import base64
import json
import dateutil.parser as dp

PROJECT = '***REMOVED***'


def timestamp_to_epoch(timestamp: str):
    return int(dp.parse(timestamp).strftime('%s'))


def write_metric(device_id: str, registry_id: str, light: bool, timestamp_seconds: int):
    client = monitoring_v3.MetricServiceClient()
    project_name = client.project_path(PROJECT)

    series = monitoring_v3.types.TimeSeries()
    series.metric.type = 'custom.googleapis.com/light'
    series.resource.type = 'global'
    series.metric.labels['device_id'] = device_id
    series.metric.labels['registry_id'] = registry_id
    point = series.points.add()
    point.value.int64_value = 1 if light else 0
    point.interval.end_time.seconds = int(timestamp_seconds)
    point.interval.end_time.nanos = int(
        (timestamp_seconds - point.interval.end_time.seconds) * 10**9)
    client.create_time_series(project_name, [series])


def iot_streamer(event, context):
    device_id = event['attributes']['deviceId']
    registry_id = event['attributes']['deviceRegistryId']
    payload = base64.b64decode(event['data']).decode('utf-8')
    print(f'Writing: {payload} at {context.timestamp}');
    decoded_payload = json.loads(payload)
    light = decoded_payload['light']
    timestamp_seconds = timestamp_to_epoch(context.timestamp)
    write_metric(device_id=device_id, registry_id=registry_id, light=light, timestamp_seconds=timestamp_seconds)

