import os
import unittest
from unittest.mock import patch

os.environ.setdefault('API_KEY', 'test-api-key')
os.environ.setdefault('ENVIRONMENT', 'test')
os.environ.setdefault('PROJECT', 'test-project')

import main


class UpdateStatusTest(unittest.TestCase):
    def setUp(self):
        self.client = main.app.test_client()

    def test_rejects_an_invalid_api_key(self):
        response = self.client.post('/light/status?apikey=wrong', json={'light': True})

        self.assertEqual(response.status_code, 403)

    @patch('main.write_metric')
    def test_accepts_a_valid_status_update(self, write_metric):
        response = self.client.post(
            '/light/status?apikey=test-api-key', json={'light': True}
        )

        self.assertEqual(response.status_code, 204)
        write_metric.assert_called_once_with(light=True, environment='test')
