#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define bitSerial1  D0
#define bitSerial2  D1
#define RCLK        D2
#define SRCLK       D3
#define SRCLR       D4

const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

const long utcOffsetInSeconds = -21600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

unsigned int mskDisplays[] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB};
unsigned int mskDigitos[] = {0x7E, 0x17, 0xB6, 0xBC, 0xD8, 0xEC, 0xEE, 0x38, 0xFE, 0xF8};

int hRlj = 0;
int mRlj = 0;
int sRlj = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  pinMode(bitSerial1, OUTPUT);
  pinMode(bitSerial2, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLR, OUTPUT);
  digitalWrite(SRCLR, LOW);
  digitalWrite(SRCLR, HIGH);
}

void loop() {
  timeClient.update();
  hRlj = timeClient.getHours();
  mRlj = timeClient.getMinutes();
  sRlj = timeClient.getSeconds();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(hRlj);
  Serial.print(":");
  Serial.print(mRlj);
  Serial.print(":");
  Serial.print(sRlj);
  Serial.print("\n");
  updateDisplays();
  delay(1000);
}

void updateDisplays() {
  int disp;
  int dato;

  for (int i = 0; i < 8; i++) {
    disp = mskDisplays[i];
    switch(i) {
      case 0: dato = mskDigitos[sRlj%10];
        break;
      case 1: dato = mskDigitos[sRlj/10];
        break;
      case 2: dato = mskDigitos[mRlj%10];
        break;
      case 3: dato = mskDigitos[mRlj/10];
        break;
      case 4: dato = mskDigitos[hRlj%10];
        break;
      case 5: dato = mskDigitos[hRlj/10];
        break;
    }

    for (int j = 0; j < 8; j++) {
      if (disp&0x01) {
        digitalWrite(bitSerial1, HIGH);
      } else {
        digitalWrite(bitSerial1, LOW);
      }

      if (dato&0x01) {
        digitalWrite(bitSerial2, HIGH);
      } else {
        digitalWrite(bitSerial2, LOW);
      }
      digitalWrite(SRCLK, LOW);
      digitalWrite(SRCLK, HIGH);
      disp = disp >> 1;
      dato = dato >> 1;
    }
    digitalWrite(RCLK, LOW);
    digitalWrite(RCLK, HIGH);
  }
}
