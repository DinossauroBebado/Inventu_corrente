/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
*/
#include <PZEM004Tv30.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
  #if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
  #define PZEM_RX_PIN 16
  #define PZEM_TX_PIN 17
#endif

#define PZEM_SERIAL Serial2
#define CONSOLE_SERIAL Serial
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
//add
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  /*************************
 *  ESP8266 initialization
 * ---------------------
 * 
 * esp8266 can connect with PZEM only via Serial0
 * For console output we use Serial1, which is gpio2 by default
 */
  #define PZEM_SERIAL Serial
  #define CONSOLE_SERIAL Serial1
  PZEM004Tv30 pzem(PZEM_SERIAL);
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Dinossauro_Conectado"
#define WIFI_PASSWORD "Dinossauro_Conectado"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCFO8lVXfwcWUJCMvHiW9DCZDkDTppbbdI"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://inventu-test-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  //inputs 
   float voltage = pzem.voltage();
   float current = pzem.current();
   float power = pzem.power();
   float energy = pzem.energy();
   float frequency = pzem.frequency();
   float pf = pzem.pf();

  //tensão
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
        sendDataPrevMillis = millis();
        // Write an Int number on the database path test/int
        if (Firebase.RTDB.setFloat(&fbdo, "WM0/voltage", voltage)){
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }
        count++;
        
        //potencia
        if (Firebase.RTDB.setFloat(&fbdo, "WM0/current", current)){
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }

        //corrente
        if (Firebase.RTDB.setFloat(&fbdo, "WM0/power", power)){
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }
        //energy
        if (Firebase.RTDB.setFloat(&fbdo, "WM0/energy", energy)){
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
          }
        //FREQUENCy
        if (Firebase.RTDB.setFloat(&fbdo, "WM0/freq", frequency)){
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
      }
  } 
}