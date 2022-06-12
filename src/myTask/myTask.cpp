#include "myTask.h"

void alarm(void* parameters){
  // Serial.print("\nTask_Alarm running on core: ");
  // Serial.println(xPortGetCoreID());
  for (;;){
    checkAlarm(&listDevices);
    vTaskDelay(5000/ portTICK_PERIOD_MS);
  }
  vTaskDelete(Task2);
}

void WifiData(void * parameters){
  Serial.print("\nwifiData running on core: ");
  Serial.println(xPortGetCoreID());
  for (;;){
    // Serial.print("\nwifiData running on core: ");
    // Serial.println(xPortGetCoreID());
    WifiStream();
    vTaskDelay(5000/ portTICK_PERIOD_MS);
  }
}

void firebaseStream(void *){
  Firebase.beginStream(fbStreamData, "/testUser1");
  Serial.println("(main) begin Stream");
  Firebase.setStreamCallback(fbStreamData, streamCallback, streamTimeOutCallback);
  Serial.println("(main) set Stream callback: OK");
  vTaskDelete(Task3);
}

void buildListItem(void * parameters){
  for(;;){
    // Serial.print("\nbuild list device running on core: ");
    // Serial.println(xPortGetCoreID());
    buildListDevices(&listDevices);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}