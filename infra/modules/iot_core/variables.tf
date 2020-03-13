variable "project" {
  type = string
  description = "GCP project ID"
}
variable "name" {
  type = string
  description = "Registry name"
}
variable "region" {
  type = string
  description = "GCP region"
}
variable "certificate_file" {
  type = string
  description = "Path to the PEM public key certificate file"
}
