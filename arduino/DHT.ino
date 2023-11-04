#include <DHT.h> // https://github.com/adafruit/DHT-sensor-library  
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN    2
#define LEDPIN    16 
#define DHTTYPE   DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

HTTPClient httpClient;
WiFiClient wClient;

String URL = "http://3.135.79.180:3100/api/logTemp";

float last_recorded_temp = 0;

void setup() {
  Serial.begin(9600); 
  Serial.println("***Inicializando conexión a My SQL***");

  // Configuracion de sensor DHT
  dht.begin(); 

  // Configuracion de WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a red WiFi \"");
  Serial.print(ssid);
  Serial.print("\"");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConectado! IP: ");
  Serial.println(WiFi.localIP());
  delay(500);

  // Configuracion de LED
  pinMode(LEDPIN, OUTPUT);
  
}

void loop() {
    // Obtener temperatura y humedad del sensor
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // Terminar en case de que haya error
    if (isnan(t)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return;
    }

    // Imprimir valores en monitor
    Serial.print("\nTemperatura: ");
    Serial.print(t);
    Serial.print(" ºC\t");
    Serial.print("\nHumidity: ");
    Serial.print(h);
    Serial.print(" %\t");

    // Si la temperatura cambia por mas de 1.2, loggear la informacion
    if(abs(last_recorded_temp - t) >= 1.2){
      logIntento(h, t);
      last_recorded_temp = t;
    }

    // Si la humedad esta por encima de 80, encender el LED
    if(h >= 80) digitalWrite(LEDPIN, HIGH);
    else digitalWrite(LEDPIN, LOW);
    
      
    delay(10000);
}


void logIntento(float h, float t){
  if(WiFi.status() == WL_CONNECTED){
    String data = URL;
    // /api/logTemp/:humidity/:temperature
    data = data + '/' + h + '/' + t;
    Serial.println(data); 

    // Hacer post request
    httpClient.begin(wClient, data.c_str()); 
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode); 
    httpClient.end(); 
  }
  return;
}
