#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

String lastSide = "";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Starte MPU6050 Test...");

  Wire.begin(19, 20);
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 Verbindung erfolgreich!");
  } else {
    Serial.println("FEHLER: MPU6050 nicht gefunden!");
    while (1);
  }
}

String getSide(int ax, int ay, int az) {
  int threshold = 15000;

  if (az > threshold) return "Seite 1 (Learning)";
  if (az < -threshold) return "Seite 6 (Default)";
  if (ay > threshold) return "Seite 2 (Gardening)";
  if (ay < -threshold) return "Seite 3 (Working)";
  if (ax > threshold) return "Seite 4 (Cooking)";
  if (ax < -threshold) return "Seite 5 (House work)";

  return "Unbekannt";
}

void loop() {
  int16_t ax, ay, az;

  mpu.getAcceleration(&ax, &ay, &az);

  String currentSide = getSide(ax, ay, az);

  if (currentSide != lastSide && currentSide != "Unbekannt") {
    Serial.println("Neue Seite erkannt:");
    Serial.println(currentSide);
    Serial.println("--------------------");

    lastSide = currentSide;
  }

  delay(300);
}