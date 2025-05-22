#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte allowedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF}; // Ganti ini dengan UID milikmu
bool lampState = false;
bool lastRead = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("Tap kartu NFC...");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    lastRead = false;  // reset status baca
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  if (lastRead) return;  // cegah pembacaan dobel saat kartu belum dilepas

  // Cek apakah UID cocok
  bool authorized = true;
  for (byte i = 0; i < 4; i++) {
    if (mfrc522.uid.uidByte[i] != allowedUID[i]) {
      authorized = false;
      break;
    }
  }

  if (authorized) {
    lampState = !lampState;  // toggle
    digitalWrite(LED_PIN, lampState ? HIGH : LOW);
    Serial.print("Akses OK. Lampu: ");
    Serial.println(lampState ? "NYALA" : "MATI");
  } else {
    Serial.println("Akses Ditolak");
  }

  lastRead = true;
  delay(500);
}