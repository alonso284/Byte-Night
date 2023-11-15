//Debanhi Monserrath Medina Elizondo
//A00835550
//Código para sensor rfid y actuador servo

//este código corre cada que alguien pase una tarjeta entonces no es necesario
//incluir cada cuando se guardaran los datos ya que es indefinido

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <dummy.h>
#include <DHT.h> // https://github.com/adafruit/DHT-sensor-library  
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#define RST_PIN         5           // (D1)
#define SS_PIN          4          // (D2) 
#define LOCK_PIN        0          //D3
#define ACTIVATION_TIME 100
#define DHTPIN    2
#define DHTTYPE   DHT11

//pines y variables

Servo myservo; //instancia de servo 
MFRC522 mfrc522(SS_PIN, RST_PIN);   // instancia de MFRC522
DHT dht(DHTPIN, DHTTYPE);

bool dataLock = false;//indicador 
unsigned long Auth = 3158988764; //uid con el que se acepta la tarjeta

//datos de mi internet
const char* ssid = "Tec-IoT"; 
const char* password = "spotless.magnetic.bridge";
int acce = 0; //indicador de acceso

HTTPClient httpClient;
WiFiClient wClient;

String URL = "http://3.135.79.180:3100/api/logUid/";
String URL_verify = "http://3.135.79.180:3100/api/verifyUid/";

//funcion de abrir puerta (servo)

void OpenDoor()
{
  myservo.write(180); //gira el servo 180 grados simulando el abrir la puerta
  delay(5000); //espera 5 segundos o 5000 milisegundos, simulando que la puerta se queda abierta
  myservo.write(0); //gira el servo 180 grados, regresando al 0, simulando cerrar la puerta
  digitalWrite(LOCK_PIN, LOW); //activa el pasador de la cerradura
  delay(ACTIVATION_TIME); //mantiene activada la cerradura
  digitalWrite(LOCK_PIN, HIGH); //desactiva el pasador de la cerradura
}

//funcion que regresa el link en donde se incluye el uid y el acceso que serán mandados
//a la base de datos, uniendo el URL ya establecido con los nuevos datos
void logIntento(unsigned long t, int ac){
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

bool verifyIntento(unsigned long t, int ac) {
  if (WiFi.status() == WL_CONNECTED) {
    String data = URL_verify;
    data += t;
    Serial.println(data);

    httpClient.begin(wClient, data.c_str());

    httpClient.addHeader("Content-Type", "application/json");  // Remove "Content-Type: " from the value

    int httpResponseCode = httpClient.GET();
    Serial.println("HTTP Response Code: " + String(httpResponseCode));

    if (httpResponseCode > 0) {
      String payload = httpClient.getString();
      Serial.println("Response payload: " + payload);
      if(String("{\"data\":[]}") == httpClient.getString()) return false;
      else return true;
    } else {
      Serial.println("Failed to get response");
    }

    httpClient.end();
  }
  return false;
}

//funcion inicial
void setup() {
  Serial.begin(9600); 
  //seccion de rfid
  myservo.attach(2); // conecta el servo al pin 2
  myservo.write(0);// inicia la posicion en 0 grados
  SPI.begin(); // inicializa el bus SPI
  pinMode(LOCK_PIN, OUTPUT); //lock_Pin es salida
  digitalWrite(LOCK_PIN, HIGH); //establece el lock_pin en alto (bloquea la cerradura)
  mfrc522.PCD_Init(); //inicializa el lector de tarjetas
  
  Serial.println("***Inicializando conexión a My SQL***");
  
  dht.begin(); 

  //conexión al wifi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.begin(ssid, password);
  Serial.print("Conectando a red WiFi \"");
  Serial.print(ssid);
  Serial.print("\"");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  Serial.print("\nConectado! IP: ");
  Serial.println(WiFi.localIP());
  //conexión realizada
}

//ciclo 
void loop() {
  // Prepara la llave de acceso - por defecto viene seteadas con FFFFFFFFFFFFh directamente desde la fabrica.
  MFRC522::MIFARE_Key key;
  
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //variables
  byte block;
  byte len;
  MFRC522::StatusCode status;


  // Reincia el Loop en caso de no encontrar una tarjeta presente.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();
    return;
  }

  // Selecciona una de las tarjetas
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();
    return;
  }

  //mensaje de tarjeta detectada
  Serial.println(F("**Tarjeta Detectada:**"));

  //muestra los detalles de la tarjeta 
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); 
 
  //variables
  byte buffer2[18];
  block = 1;
  byte size = sizeof(buffer2);

  //satuts verifican si leyó bien el PICC type
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); 
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();
    return;
  }
  dataLock=false;


  
  //obtiene el codigo uid de la tarjeta

    unsigned long uid = (mfrc522.uid.uidByte[0]<<24UL) | 
    (mfrc522.uid.uidByte[1]<<16UL) | 
    (mfrc522.uid.uidByte[2]<<8UL) | 
    mfrc522.uid.uidByte[3];
//    dataLock=false;
    dataLock = verifyIntento(uid, acce);
      

    
//si las uid son iguales, deja pasar y registra en base de datos 0 en acceso no concedido o 1 en acceso concedido
//    for (uint8_t i = 1; i < 2; i++)
//  {
//    Serial.print(uid);
//    Serial.print("--");
//
//    Serial.println(Auth);
//    if(Auth==uid)
//    {
//      dataLock=true;
//      break;
//    }
//  }



if (!dataLock)
  {
    Serial.println("Acceso Denegado");
    acce = 0;
  }
  else
  {
    Serial.println("Acceso Concedido");
    acce = 1;
    OpenDoor();
  }

  //manda a funcion logIntento los valores correspondientes
  logIntento(uid, acce);
  

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
