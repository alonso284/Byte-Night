#include <Adafruit_Sensor.h>

#include <DHT.h> // https://github.com/adafruit/DHT-sensor-library  
#include <DHT_U.h>
#include <ESP8266WiFi.h> // Biblioteca para manejar la conexión WiFi del ESP8266
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h> // Biblioteca para realizar peticiones HTTP

#define DHTPIN    2 // Define el pin al que está conectado el sensor DHT
#define DHTTYPE   DHT11 // Define el tipo de sensor DHT que se utiliza
DHT dht(DHTPIN, DHTTYPE); // Crea una instancia del sensor DHT

#define D5 14  // Define el pin D5 como GPIO14 en NodeMCU.
#define D4 4   // Define el pin D4 como GPIO2 en NodeMCU.

int sound_analog = A0;  // Pin analógico para el sensor de sonido.

const char* ssid = "Tec-IoT"; // SSID de la red WiFi
const char* password = "spotless.magnetic.bridge"; // Contraseña de la red WiFi


HTTPClient httpClient; // Crea un cliente HTTP
WiFiClient wClient; // Crea un cliente WiFi

//String URL = "http://";
String URL = "http://3.135.79.180:3100/api/logVolume/"; // URL del servidor al que se enviarán los datos

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps
  pinMode(D4, OUTPUT); // Configura el pin D4 como salida (para el buzzer).
  pinMode(D5, INPUT);  // Configura el pin D5 como entrada (lectura digital del sensor).
  Serial.println("***Inicializando conexión a My SQL***");

  dht.begin(); // Inicializa el sensor DHT

  WiFi.mode(WIFI_STA); // Configura el módulo como estación WiFi
  WiFi.disconnect(); // Desconecta de cualquier red WiFi previa
  delay(100);

  WiFi.begin(ssid, password); // Inicia la conexión a la red WiFi con las credenciales proporcionadas
  Serial.print("Conectando a red WiFi \"");
  Serial.print(ssid);
  Serial.print("\"");
  while (WiFi.status() != WL_CONNECTED) { // Espera hasta que la conexión sea exitosa
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConectado! IP: ");
  Serial.println(WiFi.localIP()); // Muestra la dirección IP asignada al módulo
  delay(500);
}



void loop() {
  int val_digital = digitalRead(D5);       // Lee el valor digital del sensor en D5.
  int val_analog = analogRead(sound_analog);  // Lee el valor analógico del sensor en A0.

  // Envía los valores del sensor a la consola serial.
  Serial.print("Analog: ");
  Serial.print(val_analog);  
  Serial.print("\tDigital: ");         
  Serial.println(val_digital); 

  // Si el valor analógico es >100, activa el buzzer.
  if (val_analog > 55) {
    digitalWrite(D4, HIGH); // Activa el buzzer.
    delay(3000);            // Espera 3 segundos con el buzzer activado.
    digitalWrite(D4, LOW);  // Desactiva el buzzer.
  }

  // Si el WiFi está conectado, envía los datos al servidor
  if(WiFi.status() == WL_CONNECTED) {
    String fullPath = URL + String(val_analog); // Construye la URL completa con el valor analógico


    httpClient.begin(wClient, fullPath); // Inicia la conexión al servidor
    int httpCode = httpClient.POST(""); // Realiza la petición POST sin cuerpo

    // Si la petición fue exitosa, imprime la respuesta
    if (httpCode > 0) {
      // La solicitud ha sido enviada exitosamente
      String response = httpClient.getString();
      Serial.println(response);
    } else {
      // Error al enviar la solicitud
      Serial.println("Error en la solicitud HTTP: " + String(httpCode));
    }

    httpClient.end(); // Cierra la conexión
  } else {
    // Si no hay conexión WiFi, intenta reconectar
    Serial.println("WiFi no conectado. Reintentando...");
    WiFi.reconnect();
  }

  delay(1000); // Espera 10 segundos antes de enviar los siguientes datos
}
