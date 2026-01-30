#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// ===== CẤU HÌNH =====
#define DHTPIN   4      // GPIO nối DATA DHT11
#define DHTTYPE  DHT11

// Địa chỉ LCD I2C thường là 0x27 hoặc 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("ESP32 + DHT11");
  lcd.setCursor(0,1);
  lcd.print("Khoi dong...");
  delay(2000);
  lcd.clear();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Doc DHT11 that bai!");
    lcd.setCursor(0,0);
    lcd.print("Loi doc DHT11 ");
    lcd.setCursor(0,1);
    lcd.print("Kiem tra day ");
    delay(2000);
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print(" C | Do am: ");
  Serial.print(h);
  Serial.println(" %");

  lcd.setCursor(0,0);
  lcd.print("Nhiet do: ");
  lcd.print(t,1);
  lcd.print("C   ");

  lcd.setCursor(0,1);
  lcd.print("Do am: ");
  lcd.print(h,1);
  lcd.print("%    ");

  delay(2000);
}

