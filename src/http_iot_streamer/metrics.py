from google.cloud import monitoring_v3

import time
import os

PROJECT = f"projects/{os.environ['PROJECT']}"
METRIC_TYPE = 'custom.googleapis.com/light'

client = monitoring_v3.MetricServiceClient()


def write_metric(light: bool, environment: str) -> None:
    series = monitoring_v3.TimeSeries()
    series.metric.type = METRIC_TYPE
    series.resource.type = 'global'
    series.metric.labels['environment'] = environment

    now = time.time()
    seconds = int(now)
    nanos = int((now - seconds) * 10**9)
    interval = monitoring_v3.TimeInterval(
        {'end_time': {'seconds': seconds, 'nanos': nanos}}
    )
    point = monitoring_v3.Point(
        {'interval': interval, 'value': {'int64_value': 1 if light else 0}}
    )

    series.points = [point]
    client.create_time_series(request={'name': PROJECT, 'time_series': [series]})
