#ifndef _TEMPANDHUM_H_
#define _TEMPANDHUM_H_

    #include <FBRtDatabase.h>
    #include <ESPDateTime.h>
    #include <DHT.h>

    #include <mutex>

    static std::mutex mtx;

    const int DHTPIN = 15;
    const int DHTTYPE = DHT11;
    static DHT dht(DHTPIN, DHTTYPE);

    static String TEMPPATH = "/Temperature";
    static String HUMPATH = "/Humidity";
    static String TEMPKEY = "/temperatures";
    static String HUMKEY = "/humidities";

    typedef struct
    {
    float ucValue;
    float ucSource;
    } xData;
    
    // return date: "dd-mm"
    String _today();
    DHT* getDhtREf();

    void vSenderTemp( void *pvParameters );
    void vSenderHumi( void *pvParameters );
    void vReceiverTask( void *pvParameters );

    static FBRtDatabase dhtDatabase = FBRtDatabase(HOST, AUTH, "/", "");
    static QueueHandle_t xQueue = xQueueCreate( 5, sizeof( xData ) );

    QueueHandle_t* getQueueRef();


#endif