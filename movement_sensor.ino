/*
 * PIR sensor tester
 * 
 * Breadboard
 * 
 * Purple wire: ground
 * Blue wire: D7
 * Grey wire: power
 */

#include <WiFiManager.h>
#include <Servo.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "settings.h"
#include "WS2812_util.h"

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
 
int inputPin = D8;              // input pin for the motion sensor
bool state = false;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
String url = "http://remcobravenboer.nl/IOT/index.php?email="; // string for data
String email = "YOUR_EMAIL"; // email of your times account
String auth = "&key=1K14UR37C7!9anfo1!@";
String finalUrl = url + email + auth;

Servo servo;
int analogPin = A0;
int ledPin = D1;

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT);     // declare sensor as input
  Serial.print(finalUrl);
  connectToWifi();
  servo.attach(D6);
  strip.begin();
  strip.setBrightness(255);
  setAllPixels(0, 255, 0, 1.0);
 }
 
void loop(){
  //check if internet connection is still available
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to Wifi...");
    connectToWifi();
  }
  
  val = digitalRead(inputPin);
  //Serial.println(val);
  delay(1000);
  
  if (val == HIGH) {            // check if the input is HIGH
    if(state == false) {
      Serial.println("Motion detected!");
      state = true;
      setAllPixels(255, 0, 0, 1.0);
      servo.write(90);
      Serial.println(displayTime(finalUrl));
      setAllPixels(0, 255, 0, 1.0);
      servo.write(45);
    }
      
  } else {
      if(state == true) {
        Serial.println("Motion ended!");
        state = false;
      }
    }
  }

  void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);

  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

String displayTime(String url) {
  HTTPClient http; //Declare an object of class HTTPClient
    http.begin(url); //Specify request destination
    int httpCode = http.GET(); //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString(); //Get the request response payload
      Serial.println(payload); //Print the response payload
    } else {
      Serial.println("code incorrect");
    }
    http.end(); //Close connection
    delay(1500);
}
