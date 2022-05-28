#ifndef _MYWIFI_H_
#define _MYWIFI_H_

#include <Arduino.h>
#include <WiFi.h>
#include <FBRtDatabase.h>
#include <ArduinoJson.h>

static String mySsid = "Kim Ngan 2";
static String myPass = "manhhung";
bool connectWifi(const char*, const char*);
void disconnect();
bool changeWifi(const char*, const char*);
String localIP();
void wifiStreamTimeOutCallback(bool);
void wifiStreamCallback(StreamData);

#endif