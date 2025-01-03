#ifndef WIFI_SETUP_H_
#define WIFI_SETUP_H_

#include <WiFi.h>

// Function to set up Wi-Fi
void setupWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("Camera Stream Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

#endif // WIFI_SETUP_H_
