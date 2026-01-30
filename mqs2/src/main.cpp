#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 4       // GPIO bạn nối DATA của DHT11
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Khoi dong DHT11...");
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Khong doc duoc DHT11!");
  } else {
    Serial.print("Nhiet do: ");
    Serial.print(t);
    Serial.print(" °C  |  Do am: ");
    Serial.print(h);
    Serial.println(" %");
  }

  delay(2000);
}
