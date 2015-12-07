#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h> 
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send (404, "text/plain", message);
}

void setup() {
  // Start serial.
  Serial.begin(115200);

  // Start wifi manager.
  WiFiManager wifi;

  // Autoconnecto or use esp8266 network
  wifi.autoConnect("esp8266");

  // Visit esp8266.local to manage this device.
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Handle not found.
  server.onNotFound(handleNotFound);

  // Add updater.
  httpUpdater.setup(&server);

  // Start server.
  server.begin();

  // Notify.
  Serial.println("HTTP server started");
  
}

void loop() {
  // Wait for request.
  server.handleClient();
}
