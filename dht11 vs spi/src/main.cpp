#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// ===== DHT11 =====
#define DHTPIN   4
#define DHTTYPE  DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== TFT SPI =====
#define TFT_CS   5
#define TFT_DC   16   // A0
#define TFT_RST  17

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Init TFT
  tft.initR(INITR_BLACKTAB);   // ST7735
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("ESP32 + DHT11");

  delay(1500);
  tft.fillScreen(ST77XX_BLACK);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 20);
  tft.setTextSize(2);

  if (isnan(h) || isnan(t)) {
    tft.setTextColor(ST77XX_RED);
    tft.println("DHT ERROR");
    Serial.println("Doc DHT11 that bai!");
  } else {
    tft.setTextColor(ST77XX_GREEN);
    tft.print("Temp: ");
    tft.print(t,1);
    tft.println(" C");

    tft.print("Hum:  ");
    tft.print(h,1);
    tft.println(" %");

    Serial.printf("T=%.1fC  H=%.1f%%\n", t, h);
  }

  delay(2000);
}
