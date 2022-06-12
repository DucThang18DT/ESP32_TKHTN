#ifndef _FBRTINTERACTION_H_
#define _FBRTINTERACTION_H_

#include <Arduino.h>
#include <DeviceItem.h>
#include <FBRtDatabase.h>
#include <myWifi/myWifi.h>

static String PATH = "/testUser1";
static String KEY = "users";
static FBRtDatabase fbDatatbase = FBRtDatabase(HOST, AUTH, PATH, KEY);
static FirebaseData(fbStreamData);
static FirebaseData(wifiData);
static std::vector<DeviceItem> listDevices = {};

std::vector<DeviceItem>* getListDeviceRef();
void buildListDevices(std::vector<DeviceItem>*);
void splitString(std::vector<String>*, String, char);
void streamCallback(StreamData);
void streamTimeOutCallback(bool);
void fbRtStream(FirebaseData*, String);
void checkAlarm(std::vector<DeviceItem>*);
void isAlarm(std::vector<DeviceItem>* list, int index);

#endif