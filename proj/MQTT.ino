const char* mqtt_server = "192.168.0.106";
const int mqtt_port = 1883;
const char* listeningTopic = "esp32";

void MQTT_init()
{
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void MQTT_thread()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

//payload must maintain the standard: "int#int"
void callback(char* topic, byte* payload, unsigned int length)
{
  int dataType = 0;
  
  Serial.print("Menssagem recebida[");
  Serial.print(topic);
  Serial.println("] ");
  
  if(!strcmp(topic, listeningTopic))
  {
    for (int i = 0; i < length; i++)
    {
      Serial.print((char)payload[i]);
      if((char)payload[i] != '#')
      {
        applicationData[dataType][0] = (char)payload[i] - 0x30;
      }
      else
      {
        dataType = 1;
      }
    }
    Serial.println(); 
  }
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Conectando ao MQTT...");
    
    if (client.connect("ESP32"))
    {
      Serial.println("Conectado");      
      client.publish(listeningTopic, "Análise");
      
      client.subscribe(listeningTopic);
    }
    else
    {
      Serial.print("Erro:");
      Serial.print(client.state());
      Serial.println(" reconectando em 5 segundos");
      
      delay(5000);
    }
  }
}

void MQTT_publish(char* message, char* topic)
{
  Serial.print("Menssage to send: ");
  Serial.println(message);
  Serial.print("topic: ");
  Serial.println(topic);
  
  client.publish(topic, message);
}
