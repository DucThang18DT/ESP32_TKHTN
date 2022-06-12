#include "FBRtInteraction.h"
std::vector<DeviceItem>* getListDeviceRef(){
  return &listDevices;
}

void buildListDevices(std::vector<DeviceItem>* list){
  Serial.println("\n --- Build list device ---");
  while (list->size() > 0) list->pop_back();
  list->resize(0);

  int length = fbDatatbase.getInt(PATH + "/length");
  Serial.printf("\n(build list devices) length of list: %d\n", length);
  for (int i = 0; i < length; i++){
    String dataRcv = fbDatatbase.getData(PATH + "/" + KEY + "/" + String(i));
    if ((dataRcv != NULL) || (dataRcv != "")){
      DeviceItem::addObject(&(*list), dataRcv);
      isAlarm(list, i);
    }
  }
  // Serial.printf("\n(build list devices) size of listDevice = %d\n", list->size());
}

void splitString(std::vector<String> *listKeys,String str = "", char symbol = '/'){
  while (str.length() > 0){
    if (str.charAt(0) == '/') str = str.substring(1);
    listKeys->push_back(str.substring(0, str.indexOf('/')));
    str = str.substring(str.indexOf('/'));
  }
}

void streamCallback(StreamData data){
  // Serial.printf("\nstream path: %s\nevent path: %s\ndata type: %s\nevent type: %s\n\n",
  //               data.streamPath().c_str(),
  //               data.dataPath().c_str(),
  //               data.dataType().c_str(),
  //               data.eventType().c_str());
    Serial.printf("\nStream firebase running on core: ");
    Serial.println(xPortGetCoreID());
    Serial.println();
    buildListDevices(&listDevices);
}

void streamTimeOutCallback(bool timeOut){
  Serial.println("timeOut founction");
  if (timeOut)
    Serial.println("stream timeout, resuming...\n");
}

void checkAlarm(std::vector<DeviceItem>* list){
      Serial.println("check alarm");
      for (int i = 0; i < list->size(); i++){
        if (list->at(i).isTimeToOn() || list->at(i).isTimeToOff()) {
          String json = list->at(i).toJson();
          fbDatatbase.sendData(PATH + "/" + KEY + "/" + String(i), json, Mode::set);
          delay(50);
        }
      }
}

void isAlarm(std::vector<DeviceItem>* list, int index){
  if (list->at(index).isTimeToOn() || list->at(index).isTimeToOff()) {
    String json = list->at(index).toJson();
    fbDatatbase.sendData(PATH + "/" + KEY + "/" + String(index), json, Mode::set);
    delay(50);
  }
}