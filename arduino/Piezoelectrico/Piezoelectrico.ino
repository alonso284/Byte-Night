#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

HTTPClient httpClient;
WiFiClient wClient;

String URL = "http://3.135.79.180:3100/api/logPressure/";

void setup() {
  Serial.begin(9600); 
  Serial.println("**Inicializando conexión a My SQL**");
  
  dht.begin(); 

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
  
  // Se incializan los pines de entrada y salida (LED y sensor)
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
  pinMode(A0, INPUT);
}

void loop() {
    int t = analogRead(A0); // Se almacena el valor de lectura del sensor

    if (isnan(t)) {
      Serial.println("Error obteniendo los datos del sensor DHT11");
      return;
    }

    Serial.print("\nPresión: ");
    Serial.println(t);

    /* Comprobamos que el vaso esté vacío.
    Si es así, enviamos ese registro a la base de datos. Si el vaso sigue lleno
    entonces únicamente desplegamos el valor de la presión en pantalla. */
    if(t < 13)
    {
      logIntento(t);
      // Se enciende el led rojo, indicando que el vaso está vacío.
      digitalWrite(D1, HIGH);
      digitalWrite(D0, LOW);
      /* Si el vaso está vacío esperamos otros 30 segundos antes de volver
      a evaluar su condición */
      delay(10000); 
    }
    // Si el vaso sigue lleno entonces esperamos 10 segundos antes de evaluar de nuevo.
    else
    { 
      // Se enciende el led verde
      digitalWrite(D0, HIGH);
      digitalWrite(D1, LOW);
      delay(10000);
    }
}

// El resto del código se mantiene, solo había que cambiar la parte del loop.
void logIntento(int t){
  if(WiFi.status() == WL_CONNECTED){
    String data = URL;
    data = data + t;
    Serial.println(data); 
    
    httpClient.begin(wClient, data.c_str()); 
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode); 
    httpClient.end(); 
  }
  return;
}