#include <myTask/myTask.h>
#include "soc/rtc_wdt.h"

unsigned long referenceTime;

void setup() {
  Serial.begin(9600);
  // turn off watchdog timer
  rtc_wdt_protect_off();
  rtc_wdt_disable();

  connectWifi(mySsid.c_str(), myPass.c_str());
  fbDatatbase.connect();
  
  buildListDevices(&listDevices);
  delay(1000);

  getDhtREf()->begin();
  if( *(getQueueRef()) != NULL )
  {
    Serial.println("\nDHT task");
    xTaskCreatePinnedToCore( vSenderTemp, "Sender1", 5000, NULL, 2, NULL ,0);
    xTaskCreatePinnedToCore( vSenderHumi, "Sender2", 5000, NULL , 2, NULL ,0);
    xTaskCreatePinnedToCore( vReceiverTask, "Receiver", 10000, NULL, 1, NULL,0); 
    // vTaskStartScheduler();
  }

  xTaskCreatePinnedToCore(buildListItem,"Build list item",10000, NULL, 1, &Task3, 1);
  xTaskCreatePinnedToCore(alarm, "Check Alarm", 5000, NULL, 1, &Task2, 1);
  xTaskCreatePinnedToCore(WifiData, "Wifi Data", 20000, NULL, 0, &Task4, 1);

  Serial.printf("\nRunning on core: ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  // if (abs(millis() - referenceTime >= 10000)){
  //   referenceTime = millis();
  //   Serial.println("\n-------List Devices-------");
  //   Serial.printf("size = %d\n", listDevices.size());
  //   for (int i = 0; i< listDevices.size(); i++){
  //     Serial.println(listDevices.at(i).name());
  //   }
  // }
  // mtx.lock();
  // // disconnect();
  // delay(2000);
  // float hum = getDhtREf()->readHumidity();
  // Serial.printf("\nHum = %f\n", hum);
  // float temp = getDhtREf()->readTemperature();
  // Serial.printf("\ntemp = %f\n", temp);
  // delay(2000);
  // // connectWifi(mySsid.c_str(), myPass.c_str());
  // mtx.unlock();
}