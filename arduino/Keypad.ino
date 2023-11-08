#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Keypad.h>
#include <Servo.h>
#include <vector>


Servo servo0; //Crear objeto para el servo
int posicion = 0; //Inicializar variable posicion
const byte ROWS = 4; // Cantidad de filas
const byte COLS = 4; // Cantidad de columnas
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
}; //Crear la matriz
byte rowPins[ROWS] = {D0, D1, D2, D3}; //Conexion de pines a filas
byte colPins[COLS] = {D4, D5, D6, D7}; // Conexion de pines a columnas
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
std::vector<char> order;
String Orden;
int orderChar = 0;

const char* ssid = "Deco_X_Guest";
const char* password = "CRHouse1!";

HTTPClient httpClient;
WiFiClient wClient;

String URL = "http://192.168.68.102:3100/api/logKeys/";

void setup() {
  Serial.begin(9600);
  
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
  //Comentado porque se dañó mi servo
  //servo0.attach(8); //Servo 0 para representar bebida 0
}
//Abrir la valvula n dependiendo del valor que le hayas dado
void dispense(String orden){
  int pos;
  //Ir abriendo poco a poco la valvula/servo para dispensar
  for(pos = 0; pos <= 180; pos++){
    servo0.write(pos); //Definir angulo para el servo
    delay(15); //Tiempo de espera antes del siguiente paso de 15 ms
  }
  //Cerrar igual poco a poco la valvula/servo para dispensar
  for(pos = 180; pos >= 0; pos--){
    servo0.write(pos); //Definir el angulo para el servo
    delay(15); //Tiempo espera
  }
}
//Imprime los valores de la orden actual, como si fuera una maquinita dispensadora
String get_order(){
  String itemOrden = "";
  for(int i = 0; i < order.size(); i ++){
    itemOrden += String(order[i]);
  }
  Serial.println(itemOrden);
  logIntento(itemOrden);
  return itemOrden;
}
void loop() {
  //Leer key presionada
  char key = keypad.getKey();

  //si se presiona
  if (key){
    //Se verifica que si esta vacio, el primer input solo puede ser una letra
    if (order.size() == 0 && int(key) > 64){
      Serial.println("Letra: " + String(key)); 
      order.push_back(key); //Se agrega al vector la letra
    //Se verifica que si ya tiene items, solo se pueden aceptar numeros
    } else if(order.size() > 0 && (48 <= int(key)  && int(key) <= 57)){
      Serial.println("Numero: " + String(key));
      order.push_back(key); //Se arregla al vector el numero
    } 
    //Si se completo la combinacion 
    if (order.size() >= 2){
      String orden_a_dispensar = get_order();
      //dispense(orden_a_dispensar);
      order.clear();
    }
  }
}

void logIntento(String t){
  if(WiFi.status() == WL_CONNECTED){
    String data = URL;
    data = data + "%22" +t + "%22";
    Serial.println(data); 
    
    httpClient.begin(wClient, data.c_str()); 
    //httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode); 
    httpClient.end(); 
  }
  return;
}
