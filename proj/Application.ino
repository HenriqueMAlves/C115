enum NUM_ROOM{
  SALA,
  QUARTO1,
  QUARTO2,
  MAX_ROOM_NUMBER
};

//  [0][0] - room to set
//  [1][0] - action to set
int applicationData[2][1] = { 0xFF , 0xFF };

typedef struct{
  bool active;
  int currentConsumption; // mA
  int voltageConsumption; // V
  int powerConsumption;   // mW
}ROOMS;

ROOMS rooms[MAX_ROOM_NUMBER];

char* postTopic = "event";

void application_thread()
{
  if(timer(&timerUpdateRooms, 1000))
  {
    for(int i = 0; i < MAX_ROOM_NUMBER; i++)
    {
      if(rooms[i].active)
      {
        rooms[i].currentConsumption += getRandomCurrent();
        rooms[i].voltageConsumption = getRandomVoltage();
        rooms[i].powerConsumption = rooms[i].currentConsumption * rooms[i].voltageConsumption;
        /*Serial.print("Room: ");
        Serial.println(i);
        Serial.print("Consumption - [I]: ");
        Serial.print(rooms[i].currentConsumption);
        Serial.print(" [V]: ");
        Serial.print(rooms[i].voltageConsumption);
        Serial.print(" [P]: ");
        Serial.println(rooms[i].powerConsumption);*/
      }
    } 
  }
}

void applicationHandle()
{
  if(applicationData[0][0] != 0xFF)
  {
    Serial.print("Updating room: ");
    Serial.println(applicationData[0][0]);
    rooms[applicationData[0][0]].active = applicationData[1][0];
    applicationData[0][0] = 0xFF;
    applicationData[1][0] = 0xFF;  
  }
}

void applicationSendData()
{
  int op = 0;
  static bool flagOP = 0;
  char payload[255], consumption[255];

  if(timer(&timerSendData, 2500))
  {
    //Serial.println("Send Data...");
    for(int i = 0; i < MAX_ROOM_NUMBER; i++)
    {
      if(rooms[i].active)
      {
        //Envia o valor de corrente
        strcpy(payload, "current#");
        itoa(i, &payload[8], 10);
        payload[9] = '#';
        itoa(rooms[i].currentConsumption, &payload[10], 10);

        MQTT_publish(payload, postTopic);
        
        Serial.print("Menssage to send: ");
        for(int i=0; payload[i] != 0; i++)
          Serial.print(payload[i]);
        Serial.print("\nTopic: ");
        Serial.println(postTopic);

        //Envia o valor de potencia
        strcpy(payload, "power#");
        itoa(i, &payload[6], 10);
        payload[7] = '#';
        itoa(rooms[i].powerConsumption, &payload[8], 10);

        MQTT_publish(payload, postTopic);
        
        Serial.print("Menssage to send: ");
        for(int i=0; payload[i] != 0; i++)
          Serial.print(payload[i]);
        Serial.print("\nTopic: ");
        Serial.println(postTopic);

        op += rooms[i].powerConsumption;
        flagOP = 1;
      }
    } 
    if(flagOP == 1)
    {
      //Envia o valor do consumo total
      strcpy(payload, "consumption#");
      itoa(op, &payload[12], 10);
  
      MQTT_publish(payload, postTopic);
      flagOP = 0; 
    }
  }
}

int getRandomCurrent()
{
  return 1;
}

int getRandomVoltage()
{
  return 110;
}
