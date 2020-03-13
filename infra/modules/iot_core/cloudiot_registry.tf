resource "google_cloudiot_registry" "registry" {
  name = var.name
  project = var.project
  region = var.region

  event_notification_configs {
    pubsub_topic_name = google_pubsub_topic.stats.id
  }

  http_config = {
    http_enabled_state = "HTTP_DISABLED"
  }

  mqtt_config = {
    mqtt_enabled_state = "MQTT_ENABLED"
  }

  credentials {
    public_key_certificate = {
      format      = "X509_CERTIFICATE_PEM"
      certificate = file("${var.certificate_file}")
    }
  }
}

