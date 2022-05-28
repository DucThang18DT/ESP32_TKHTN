#include "TempAndHum.h"

String _today(){
    DigitalClock clock = DigitalClock();
    String date ="", month = "";
    date += clock.getDate() <10 ? "0" + String(clock.getDate()): String(clock.getDate());
    month += clock.getMonth() <10 ? "0" + String(clock.getMonth()): String(clock.getMonth());
    return date + "-" + month;
}

void sendDhtData(){
    int tempLength = dhtDatabase.getInt(TEMPPATH + "/length");
    dhtDatabase.sendData(TEMPPATH + "/Dates/" + _today() + "/" + String(tempLength), String(dhtSensor.tempNow()), Mode::set);
    dhtDatabase.sendData(TEMPPATH + "/length", tempLength + 1, Mode::update);
    int humLength = dhtDatabase.getInt(HUMPATH + "/length");
    dhtDatabase.sendData(HUMPATH + "/Dates/" + _today() + "/" + String(humLength), String(dhtSensor.humNow()), Mode::set);
    dhtDatabase.sendData(HUMPATH + "/length", humLength + 1, Mode::update);
}

void vSenderTemp( void *pvParameters )
{
const TickType_t xTicksToWait = 500 / portTICK_PERIOD_MS;

  for( ;; )
  {
    bool xStatus;
    xData Data;
    Data.ucValue=dht.readTemperature();
    Data.ucSource=1;
    xStatus = xQueueSend( xQueue, &Data, portMAX_DELAY );

    if( xStatus != true )
    {
      Serial.println( "Could not send to the queue.\r\n" );
    }

    taskYIELD();
  }
}

void vSenderHumi( void *pvParameters )
{
const TickType_t xTicksToWait = 500 / portTICK_PERIOD_MS;

  for( ;; )
  {
    bool xStatus;
    xData Data;
    Data.ucValue=dht.readHumidity();
    Data.ucSource=2;
    xStatus = xQueueSend( xQueue, &Data, portMAX_DELAY );

    if( xStatus != true )
    {
      Serial.println( "Could not send to the queue.\r\n" );
    }
    taskYIELD();
  }
}

void vReceiverTask( void *pvParameters )
{
  for( ;; )
  {
    xData Data;
    bool xStatus;
    xStatus = xQueueReceive( xQueue, &Data, portMAX_DELAY );

    if( xStatus == true )
    {
      if( Data.ucSource == 1 )
      {
        Serial.print(  "Temperature:" );
        Serial.println(  Data.ucValue );

        int tempLength = dhtDatabase.getInt(TEMPPATH + "/length");
        dhtDatabase.sendData(TEMPPATH + "/Dates/" + _today() + "/" + String(tempLength), Data.ucValue, Mode::set);
        dhtDatabase.sendData(TEMPPATH + "/length", tempLength + 1, Mode::update);
      }
      else
      {
        Serial.print( "Humidity:" );
        Serial.println( Data.ucValue );

        int humLength = dhtDatabase.getInt(HUMPATH + "/length");
        dhtDatabase.sendData(HUMPATH + "/Dates/" + _today() + "/" + String(humLength), Data.ucValue, Mode::set);
        dhtDatabase.sendData(HUMPATH + "/length", humLength + 1, Mode::update);
      }
      
    }
    else
    {
      Serial.println( "Could not receive from the queue.\r\n" );
    }
  }
}