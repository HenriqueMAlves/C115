const char* ssid = "Apto 03";
const char* password = "apto03123";

void WIFI_init()
{
  delay(10);
  Serial.println();
  Serial.print("Conectado a rede: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}
