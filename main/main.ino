/*
  Project: Hipster Plant
  Description: This code contains main functions for a smart plant project with sensors and actuators.

  Autor: Leandro C. Silva
  Data: 22/07/2023
*/

#include "main_env.h"

/* Constants */
#define BLYNK_PRINT Serial
#define ESP8266_BAUD 9600
#define SEND_DATA_INTERVAL_IN_SECONDS 2

#define MOISTURE_PORT A0

/* Libraries includes */
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>

BlynkTimer timer;
SoftwareSerial EspSerial(2, 3); // TX, RX
ESP8266 ESP8266_MODULE(&EspSerial);

/*
  Entry point
*/
void setup() {
  // Debug console
  Serial.begin(ESP8266_BAUD);
  setup_ports();
  setup_blynk();
}

/*
  Loop
*/
void loop() {
  Blynk.run();
  timer.run();
}

void setup_ports() {
  pinMode(MOISTURE_PORT, INPUT);
}

void setup_blynk() {
  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(BLYNK_AUTH_TOKEN, ESP8266_MODULE, WIFI_SSID, WIFI_PASS);
  delay(10);
  timer.setInterval(1000L * SEND_DATA_INTERVAL_IN_SECONDS, send_data_to_blynk);
}

void send_data_to_blynk() {
  int current_moisture = get_moisture_percentage();

  Blynk.virtualWrite(V0, current_moisture);
  Serial.println("Current moisture: " + String(current_moisture) + "%");
  Serial.println();
}

int get_moisture_percentage() {
  return parse_analog_read_to_percentage(analogRead(MOISTURE_PORT));
}

int parse_analog_read_to_percentage(int value) {
  return (100 - ((value/1023.00) * 100 ));
}