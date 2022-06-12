#ifndef _MYWIFI_H_
#define _MYWIFI_H_

#include <Arduino.h>
#include <WiFi.h>
#include <FBRtDatabase.h>
#include <ArduinoJson.h>

static String mySsid = "Thanh Tung";
static String myPass = "29032009";
bool connectWifi(const char*, const char*);
void disconnect();
bool changeWifi(const char*, const char*);
String localIP();
void wifiStreamTimeOutCallback(bool);
void wifiStreamCallback(StreamData);
void WifiStream();

#endif