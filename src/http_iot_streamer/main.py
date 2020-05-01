from google.cloud import monitoring_v3
from flask import abort

import json
import time
import os

PROJECT = '***REMOVED***'
API_KEY = '***REMOVED***'


def write_metric(light: bool, environment: str, timestamp_seconds: int):
    client = monitoring_v3.MetricServiceClient()
    project_name = client.project_path(PROJECT)

    series = monitoring_v3.types.TimeSeries()
    series.metric.type = 'custom.googleapis.com/light'
    series.resource.type = 'global'
    series.metric.labels['environment'] = environment
    point = series.points.add()
    point.value.int64_value = 1 if light else 0
    point.interval.end_time.seconds = int(timestamp_seconds)
    point.interval.end_time.nanos = int(
        (timestamp_seconds - point.interval.end_time.seconds) * 10**9)
    client.create_time_series(project_name, [series])


def handle_request(request_json):
    if request_json and 'light' in request_json:
        light = request_json['light']
        write_metric(light=light, environment=os.environ['env'], timestamp_seconds=time.time())
    else:
        print(f'Invalid request: {request_json}')
        return abort(400)

def http_iot_streamer(request):
    print(f'Handling request {request}')

    if request.method == 'POST' and request.headers['content-type'] == 'application/json':
        if 'apikey' not in request.args or request.args['apikey'] != API_KEY:
            return abort(403)

        return handle_request(request.get_json(silent=True))
    else:
        print(f'Invalid request {request.headers}')
        return abort(404)
