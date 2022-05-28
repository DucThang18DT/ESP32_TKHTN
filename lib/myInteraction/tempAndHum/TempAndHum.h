#ifndef _TEMPANDHUM_H_
#define _TEMPANDHUM_H_

    #include <FBRtDatabase.h>
    #include <sensors/tempAndHumSensor/TempAndHumSensor.h>
    #include <digitalClock/DigitalClock.h>
    #include <sensors/tempAndHum/DHT.h>

    #define DHTPIN 22
    static String TEMPPATH = "/Temperature";
    static String HUMPATH = "/Humidity";
    static String TEMPKEY = "/temperatures";
    static String HUMKEY = "/humidities";

    static TempAndHumSensor dhtSensor = TempAndHumSensor(DHTPIN, DHTType::DHT11);
    static FBRtDatabase dhtDatabase = FBRtDatabase(HOST, AUTH, "/", "");

    typedef struct
    {
    float ucValue;
    float ucSource;
    } xData;
    
    // return date: "dd-mm"
    String _today();
    void sendDhtData();


    void vSenderTemp( void *pvParameters );
    void vSenderHumi( void *pvParameters );
    void vReceiverTask( void *pvParameters );

    static QueueHandle_t xQueue = xQueueCreate( 2, sizeof( xData ) );

#endif