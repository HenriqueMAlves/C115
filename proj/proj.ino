#include<WiFi.h>
#include <PubSubClient.h>
#include "Esp32MQTTClient.h"

/***  Timer  ***/
typedef struct {
  long time_start;
  long time_end;
  bool time_flag;
}TIMER;

TIMER timerSendData;
TIMER timerUpdateRooms;

/***  Connection  ***/
WiFiClient espClient;
PubSubClient client(espClient);

void setup()
{
  Serial.begin(115200);
  WIFI_init();
  MQTT_init();
}

void loop()
{  
  MQTT_thread();
  application_thread();
  applicationHandle();
  
  applicationSendData();
}
