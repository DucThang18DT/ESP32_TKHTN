#ifndef _MYTASK_H_
#define _MYTASK_H_

    #include <DeviceItem.h>
    #include <FBRtDatabase.h>
    #include <fbInter/FBRtInteraction.h>
    #include <myWifi/myWifi.h>
    #include <tempAndHum/TempAndHum.h>
    
    static TaskHandle_t Task2;
    static TaskHandle_t Task3;

    void alarm(void *);
    void WifiData(void *);
    void firebaseStream(void *);
    void buildListItem(void*);
#endif