resource "google_pubsub_topic" "stats" {
  name = "${var.name}-stats"
  project = var.project
}
