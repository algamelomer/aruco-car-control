#include "camera_pins.h"
#include "camera_stream.h"
#include "wifi_setup.h"
#include <WebServer.h>
#include <WiFi.h>

const char* ssid = "TP-Link_C2A8";
const char* password = "07870163";

// Define static IP parameters
IPAddress local_IP(192, 168, 0, 115);       // Set your desired IP address
IPAddress gateway(192, 168, 1, 1);          // Set your network gateway
IPAddress subnet(255, 255, 255, 0);         // Set your subnet mask
IPAddress primaryDNS(8, 8, 8, 8);           // Optional: set primary DNS
IPAddress secondaryDNS(8, 8, 4, 4);         // Optional: set secondary DNS

WebServer commandServer(81);  // Web server to handle commands on port 81

// Function to handle incoming commands via HTTP GET
void handleCommand() {
  if (commandServer.hasArg("cmd")) {
    String cmd = commandServer.arg("cmd");  // Get the value of the "cmd" parameter
    Serial.println(cmd);  // Print the command to Serial
    commandServer.send(200, "text/plain", "Command received: " + cmd);  // Send response to client
  } else {
    commandServer.send(400, "text/plain", "Command not found");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println();

  // Camera initialization
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 6;
    config.fb_count = 1;
  }

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Connect to Wi-Fi with static IP
  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);  // Set static IP
  setupWiFi(ssid, password);

  // Start the camera streaming server (on port 80)
  startCameraServer();

  // Start the command handling server (on port 81)
  commandServer.on("/cmd", handleCommand);
  commandServer.begin();  // Start the command server on port 81
}

void loop() {
  commandServer.handleClient();  // Handle any incoming client requests for commands
}
