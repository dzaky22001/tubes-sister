#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>  // Library untuk RTC (Real Time Clock)

// Ganti dengan SSID dan Password WiFi Anda
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// Alamat server Node.js (Backend)
const char* serverName = "http://192.168.1.100:3000/data";

// Pin DHT sensor
#define DHTPIN 4
#define DHTTYPE DHT22  // Ganti dengan DHT11 jika Anda menggunakan sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// Setup RTC
RTC_DS3231 rtc;

// Setup WiFi
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // Tunggu koneksi WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Setup DHT sensor
  dht.begin();

  // Setup RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Kirim data ke server backend
  sendDataToServer();
}

void loop() {
  // Kirim data setiap 10 detik
  sendDataToServer();
  delay(10000);
}

// Fungsi untuk mengirim data ke server backend
void sendDataToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);  // Menghubungkan ke server
    http.addHeader("Content-Type", "application/json");  // Set header untuk JSON

    // Membaca data suhu dan kelembapan dari sensor DHT
    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();

    // Membaca waktu dari RTC
    DateTime now = rtc.now();
    String waktu = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " +
                   String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

    // Membuat payload JSON
    String payload = "{\"temperature\": \"" + String(suhu) + "\", \"humidity\": \"" + String(kelembapan) + "\", \"time\": \"" + waktu + "\"}";

    // Mengirimkan data menggunakan POST
    int httpResponseCode = http.POST(payload);

    // Mengecek response dari server
    if (httpResponseCode > 0) {
      Serial.println("Data successfully sent to server");
    } else {
      Serial.println("Error sending data");
    }

    http.end();  // Menutup koneksi HTTP
  } else {
    Serial.println("WiFi not connected");
  }
}


//Test Push GitHub