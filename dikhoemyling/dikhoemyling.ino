#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 5     // Định nghĩa chân kết nối DHT22
#define DHTTYPE DHT22 // Loại cảm biến DHT

const char* ssid = "iPorn";       // Tên của mạng Wi-Fi
const char* password = "domapdjt";   // Mật khẩu Wi-Fi
const char* serverAddress = "172.20.10.5"; // Địa chỉ server Django
String apiPath = "192.168.2.6:8000/addData/";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed vto read from DHT sensor");
    return;
  }

  // Tạo JSON payload
  String data = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";
  Serial.println(data);
  //delay(2000);

  WiFiClient client;
  if (client.connect(serverAddress, 8000)) {
    Serial.println("Connected to server");
    // HTTPClient http;
    // http.addHeader("Content-Type", "application/raw");
    // http.begin(client, serverAddress, 8000, "/addData/");
    // int httpResponseCode = http.POST(data);
    // Serial.println(httpResponseCode);
    client.print("POST ");
    client.print("/addData/");
    client.println(" HTTP/1.1");
    client.println("Host: 172.20.10.5"); // Thay thế bằng tên miền hoặc địa chỉ IP của máy chủ
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
  }else {
    Serial.print("Fail to connect");
  }
    
    
}