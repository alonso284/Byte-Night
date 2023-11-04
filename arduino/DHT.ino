#include <DHTesp.h>

#define DHT_PIN D0   // The pin to which the DHT sensor is connected, in this case, D0
#define DHT_TYPE DHTesp::DHT11  // Specify DHT11 as the sensor type
#define RELAY_PIN D1  // The pin to which you want to control the relay or output
#define FAN_PIN A0

DHTesp dht;
bool relayState = LOW;

void setup() {
  Serial.begin(115200);  // You can adjust the baud rate as needed
  dht.setup(DHT_PIN, DHT_TYPE);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, relayState); // Initialize the relay to LOW
}

void loop() {
  delay(dht.getMinimumSamplingPeriod());

  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);~
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

  if (humidity > 80.0) {
      analogWrite(FAN_PIN, 1023);  // Maximum fan speed
    } else if (humidity > 75.0) {
      analogWrite(FAN_PIN, 506);   // Medium fan speed
    } else {
      analogWrite(FAN_PIN, 0);     // Fan off
    }


    if (humidity > 80.0) {
      relayState = HIGH;  // Turn the relay on
      digitalWrite(RELAY_PIN, relayState);
    } else {
      relayState = LOW;   // Turn the relay off
      digitalWrite(RELAY_PIN, relayState);
    }
  }
}
