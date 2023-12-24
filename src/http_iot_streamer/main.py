import logging
from flask import Flask, abort, app, json, request
from metrics import write_metric

import os

API_KEY = os.environ['API_KEY']
ENVIRONMENT = os.environ['ENVIRONMENT']

app = Flask(__name__)

logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s %(levelname)s %(message)s',
)


@app.route('/light/status', methods=['POST'])
def update_status():
    user_api_key = request.args.get('apikey', None)
    if user_api_key != API_KEY:
        abort(403)

    light_info = json.loads(request.data)
    write_metric(light=light_info['light'], environment=ENVIRONMENT)
    return '', 204


if __name__ == '__main__':
    app.run(debug=False, host='0.0.0.0', port=int(os.environ.get('PORT', 8080)))
