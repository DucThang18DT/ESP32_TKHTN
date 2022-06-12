#include "TempAndHum.h"

String _today(){
    DateTime.setTimeZone("<+07>-7");
    DateTime.begin();
    while (!DateTime.isTimeValid()) {
      Serial.println("\n(DigitalClock) Failed to get time from server.");
      DateTime.setTimeZone("<+07>-7");
      DateTime.begin();
    }
    Serial.println("\n(DigitalClock) Success get time from server.");
    String _now = DateTime.toString();
    Serial.printf("\ntoday: %s\n", _now.c_str());
    if (_now.length() == 0) return "";
    char today[] = {_now[8], _now[9], '-' , _now[5], _now[6], 0};
    Serial.printf("\n_now = %s\n", today);
    return String(today);
}

void vSenderTemp(void *pvParameters)
{
const TickType_t xTicksToWait = 2000 / portTICK_PERIOD_MS;

  bool xStatus;
  xData Data; 
  for( ;; )
  {
    Serial.println("\n --- Read temp ---");
    // Serial.printf("\nread temp running on core: ");
    // Serial.println(xPortGetCoreID());
    mtx.lock();
    delay(2000);
    float temp = dht.readTemperature();
    // Serial.printf("\nTemp = %f\n", temp);
    // float temp = dht.getTemperature();
    if (!isnan(temp)){
      Data.ucValue = temp;
      Data.ucSource=1;
      Serial.printf("temp: %f", Data.ucValue);
      xStatus = xQueueSendToBack( *getQueueRef(), &Data, 0 );

      if( xStatus != true )
      {
        Serial.println( "Could not send to the queue.\r\n" );
      }
    }
    mtx.unlock();
    vTaskDelay(xTicksToWait);
    // taskYIELD();
  }
}

void vSenderHumi( void *pvParameters )
{
  const TickType_t xTicksToWait = 2000 / portTICK_PERIOD_MS;
  bool xStatus;
  xData Data;
  for( ;; )
  {
    Serial.println("\n --- Read Hum ---");
    // Serial.printf("\nRead hum running on core: ");
    // Serial.println(xPortGetCoreID());
    mtx.lock();
    delay(2000);
    float hum = dht.readHumidity();
    // Serial.printf("\nHum = %f\n", hum);
    // // float hum = dht.getHumidity();
    if (!isnan(hum)){
      Data.ucValue=hum;
      Serial.printf("\nhum: %f", Data.ucValue);
      Data.ucSource = 2;
      xStatus = xQueueSendToBack( *getQueueRef(), &Data, 0 );

      if( xStatus != true )
      {
        Serial.println( "Could not send to the queue.\r\n" );
      }
    }
    mtx.unlock();
    vTaskDelay(xTicksToWait);
    // taskYIELD(); 
  }
}

void vReceiverTask( void *pvParameters )
{
  const TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;
  bool xStatus;
  xData Data;
  for( ;; )
  {
    Serial.println("\n --- Send temp and hum ---");
    xStatus = xQueueReceive( *getQueueRef(), &Data, portMAX_DELAY );

    if( xStatus == true )
    {
      String today = _today();
      Serial.printf("\nToday: %s\n", today.c_str());
      if( (int)Data.ucSource == 1 )
      {
        Serial.print(  "\nTemperature:" );
        Serial.println(  Data.ucValue );
        int tempLength = dhtDatabase.getInt(TEMPPATH + "/Dates/" + today + "/length");
        if (tempLength == 0){
          int dateLength = dhtDatabase.getInt(TEMPPATH + "/length");
          dhtDatabase.sendData(TEMPPATH + "/length", dateLength + 1, Mode::set);
        }
        dhtDatabase.sendData(TEMPPATH + "/Dates/" + today + "/temperatures/" + String(tempLength), (int)Data.ucValue, Mode::set);
        dhtDatabase.sendData(TEMPPATH + "/Dates/" + today + "/length", tempLength + 1, Mode::set);
      }
      else
      {
        Serial.print( "\nHumidity:" );
        Serial.println( Data.ucValue );

        int humLength = dhtDatabase.getInt(HUMPATH + "/Dates/" + today + "/length");
        if (humLength == 0){
          int dateLength = dhtDatabase.getInt(HUMPATH + "/length");
          dhtDatabase.sendData(HUMPATH + "/length", dateLength + 1, Mode::set);
        }
        dhtDatabase.sendData(HUMPATH + "/Dates/" + today + "/humidities/" + String(humLength), (int)Data.ucValue, Mode::set);
        dhtDatabase.sendData(HUMPATH + "/Dates/" + today + "/length", humLength + 1, Mode::set);
      }
      
    }
    else
    {
      Serial.println( "Could not receive from the queue.\r\n" );
    }
    vTaskDelay(xTicksToWait);
  }

}

QueueHandle_t* getQueueRef(){
  return &xQueue;
}

DHT* getDhtREf(){
  return &dht;
}