#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"


const char* server1 = "https://ecse-three-led-api-v2.onrender.com/api/state";
const char* apiKey = "Edith#3905";

// Digital output pins for the LEDs
const int led1Pin = 2;
const int led2Pin = 3;
const int led3Pin = 4;

// Two-dimensional array of LED states
bool ledStates[][3] = {
  {false, false, false},
  {false, false, true},
  {false, true, false},
  {false, true, true},
  {true, false, false},
  {true, false, true},
  {true, true, false},
  {true, true, true}
};
const int numStates = sizeof(ledStates) / sizeof(ledStates[0]);

void setup() {
  // Connect to WiFi network
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi.");
  }
  Serial.println("Connected to WiFi");

  // Initialize digital output pins
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
}

void loop() {
  for (int i = 0; i < numStates; i++) {
    // Set the state of each LED
    digitalWrite(led1Pin, ledStates[i][0]);
    digitalWrite(led2Pin, ledStates[i][1]);
    digitalWrite(led3Pin, ledStates[i][2]);
    delay(1000); // wait for 1 second before changing state

    // Send HTTP PUT request to update the state on the server
    WiFiClient client;
    HTTPClient http;

    // Build the JSON object with the current state of the LEDs
    StaticJsonDocument<1024> Doc;

    Doc["light_switch_1"] = ledStates[i][0];
    Doc["light_switch_2"] = ledStates[i][1];
    Doc["light_switch_3"] = ledStates[i][2];

    String jsonStr;
    serializeJson(Doc, jsonStr);

    // Set up the HTTP request with the API key and JSON body
    http.begin(client, server1);
    http.addHeader("X-API-Key", apiKey);  
    http.addHeader("Content-Type", "application/json");
  }
}
 