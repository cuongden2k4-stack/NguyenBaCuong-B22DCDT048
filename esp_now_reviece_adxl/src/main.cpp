
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//CẤU TRÚC DỮ LIỆU MỚI (Khớp 100% với mạch gửi)
typedef struct struct_message {
    int id;
    float accelX;
    float accelY;
    float accelZ;
} struct_message;

struct_message myData;

//Hàm callback tự động chạy với TỐC ĐỘ CỰC NHANH khi có dữ liệu bay tới
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // CHỈ COPY DỮ LIỆU.
  memcpy(&myData, incomingData, sizeof(myData));
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  delay(1000);
  
  
  Serial.begin(9600); 
  
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Serial.print("ĐỊA CHỈ MAC CỦA MẠCH NÀY LÀ: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Lỗi khởi tạo ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("MẠCH NHẬN ĐÃ SẴN SÀNG! Đang chờ dữ liệu ADXL345...");
}

void loop() {
  //VÒNG LẶP CHÍNH: in dữ liệu ra màn hình
  Serial.print("Đã nhận từ ID: ");
  Serial.println(myData.id);
  
  // In dữ liệu 3 trục với 2 chữ số thập phân
  Serial.print("➜ [Gia Tốc Kế] X: ");
  Serial.print(myData.accelX, 2);
  Serial.print(" | Y: ");
  Serial.print(myData.accelY, 2);
  Serial.print(" | Z: ");
  Serial.print(myData.accelZ, 2);
  Serial.println(" m/s^2");
  Serial.println("-------------------");

  
  
  delay(500); 
}
