#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WebServer.h>

#define DHTPIN 5     // Định nghĩa chân kết nối DHT22
#define DHTTYPE DHT22 // Loại cảm biến DHT

const char* ssid = "KhueTorres"; // Tên WiFi
const char* password = "04021995"; // Mật khẩu WiFi
const char* serverAddress = "192.168.100.4"; // Địa chỉ server Django
IPAddress IP;

ESP8266WebServer server(80);

DHT dht(DHTPIN, DHTTYPE);

int ledPin = D0; // Chân kết nối đèn LED

void setup() {
  pinMode(ledPin, OUTPUT); // Thiết lập chân kết nối đèn LED
  Serial.begin(9600);
  dht.begin();
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println("Connecting to WiFi..");
  }

  server.on("/", handleRoot);
  server.on("/0", turnOff);
  server.on("/1", turnOn);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // In địa chỉ IP trong Serial Monitor
  IP = WiFi.localIP();

  Serial.println("Connected to WiFi");
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  String device_name = "Rèm 2";

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed vto read from DHT sensor");
    return;
  }

  // Tạo JSON payload
  String data = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + ", \"device_name\":\"" + device_name + "\", \"ipaddr\":\"" + IP.toString() + "\"}";
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
    server.handleClient();
    
}

void handleRoot() {
  server.send(200, "text/html", "Welcome to LED control");
}

void turnOn() {
  digitalWrite(ledPin, HIGH); // Bật đèn
  server.send(200, "text/html", "LED turned on");
}

void turnOff() {
  digitalWrite(ledPin, LOW); // Tắt đèn
  server.send(200, "text/html", "LED turned off");
}