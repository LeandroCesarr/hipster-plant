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
#define DHT_PORT 7

/* Libraries includes */
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include <DHT11.h>

BlynkTimer timer;
SoftwareSerial EspSerial(2, 3); // TX, RX
ESP8266 ESP8266_MODULE(&EspSerial);
DHT11 dht(DHT_PORT);

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
  pinMode(DHT_PORT, INPUT);
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
  int current_temperature = get_temperature();

  Blynk.virtualWrite(V0, current_moisture);
  Blynk.virtualWrite(V1, current_temperature);
  Serial.println("Current moisture: " + String(current_moisture) + "%");
  Serial.println("Current temperature: " + String(current_temperature) + " C");
  Serial.println();
}

int get_moisture_percentage() {
  return parse_analog_read_to_percentage(analogRead(MOISTURE_PORT));
}

int get_temperature() {
  return dht.readTemperature();
}

int parse_analog_read_to_percentage(int value) {
  return (100 - ((value/1023.00) * 100 ));
}