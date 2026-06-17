#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <HTTPClient.h>

MPU6050 mpu;

// WLAN Daten
const char* ssid = "ssid einsetzten";
const char* password = "passwort einsetzten";

// Server URL (Laptop IP!)
const char* serverName = "http://laptopIp:port/update";

// Variablen
String lastSide = "";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP startet...");
  // I2C starten
  Wire.begin(19, 20);

  // MPU starten
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 Verbindung fehlgeschlagen!");
    while (1);
  }

  Serial.println("MPU6050 bereit!");

  // WLAN verbinden
  WiFi.begin(ssid, password);

Serial.print("Verbinde mit WLAN");

int tries = 0;

while (WiFi.status() != WL_CONNECTED && tries < 30) {
  delay(1000);
  Serial.print(".");
  tries++;
}

Serial.println();

if (WiFi.status() == WL_CONNECTED) {
  Serial.println("WLAN verbunden!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
} else {
  Serial.print("Fehlercode: ");
  Serial.println(WiFi.status());
}
}

// Funktion zur Seitenbestimmung
String getSide(int ax, int ay, int az) {

  // Schwellenwert (anpassen!)
  int threshold = 15000;

  if (az > threshold) return "study";
  if (az < -threshold) return "code";
  if (ay > threshold) return "reading";
  if (ay < -threshold) return "work";
  if (ax > threshold) return "creative";
  if (ax < -threshold) return "exercise";

  return "Unknown";
}

// Daten senden
void sendData(String side) {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"side\": \"" + side + "\"}";

    int httpResponseCode = http.POST(json);

    Serial.print("Gesendet: ");
    Serial.print(side);
    Serial.print(" | Code: ");
    Serial.println(httpResponseCode);

    http.end();
  }
}

void loop() {

  int16_t ax, ay, az;

  // Sensor auslesen
  mpu.getAcceleration(&ax, &ay, &az);

  // Seite bestimmen
  String currentSide = getSide(ax, ay, az);

  // Nur senden wenn Seite sich ändert
  if (currentSide != lastSide && currentSide != "Unknown") {

      Serial.print("Neue Seite erkannt: ");
      Serial.println(currentSide);

      sendData(currentSide);

      lastSide = currentSide;
  }

  delay(500);
}