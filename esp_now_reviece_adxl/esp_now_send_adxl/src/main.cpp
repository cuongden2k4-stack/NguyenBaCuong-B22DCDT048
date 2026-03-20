#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// địa chỉ MAC của mạch NHẬN 
uint8_t broadcastAddress[] = {0xF4, 0x2D, 0xC9, 0x6F, 0xB7, 0x70};

// Khởi tạo đối tượng cảm biến 
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

//CẤU TRÚC DỮ LIỆU MỚI (Khớp với mạch nhận)
typedef struct struct_message {
    int id;
    float accelX; // Gia tốc trục X (m/s^2)
    float accelY; // Gia tốc trục Y (m/s^2)
    float accelZ; // Gia tốc trục Z (m/s^2)
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// Hàm callback báo cáo trạng thái gửi
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // ADXL345 gửi nhanh nên chỉ in log nếu gửi THẤT BẠI để đỡ rối màn hình
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println(">>> Gửi THẤT BẠI");
  }
}

void setup() {
  Serial.begin(9600); 
  
  // Khởi động I2C và ADXL345
  Wire.begin(); 
  if(!accel.begin()) {
    Serial.println("Lỗi: Không tìm thấy ADXL345. Kiểm tra dây I2C hoặc chân CS!");
    while(1); 
  }
  
  // Bật dải đo +-2G để có độ nhạy tốt nhất cho các chuyển động nhẹ
  accel.setRange(ADXL345_RANGE_2_G);
  Serial.println("Cảm biến ADXL345 đã sẵn sàng!");

  // Khởi tạo ESP-NOW 
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Lỗi khởi tạo ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Lỗi không thêm được Peer");
    return;
  }
}

void loop() {
  // Đọc dữ liệu từ cảm biến
  sensors_event_t event; 
  accel.getEvent(&event);

  // Nhồi dữ liệu thực tế (m/s^2) vào cấu trúc
  myData.id = 1;
  myData.accelX = event.acceleration.x; 
  myData.accelY = event.acceleration.y;         
  myData.accelZ = event.acceleration.z;         

  // Lệnh bắn dữ liệu qua sóng ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result != ESP_OK) {
    Serial.println("Lỗi khi đẩy dữ liệu vào hàng đợi");
  }
  
  
  delay(100); 
}