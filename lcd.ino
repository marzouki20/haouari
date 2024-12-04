#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define LED_VERTE 3
#define LED_ROUGE 2

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte chambre1UID[] = {0x3C, 0x56, 0x79, 0x33};
byte chambre2UID[] = {0x5C, 0x7C, 0x7A, 0x33};

void setup() {
    Serial.begin(115200);
    Wire.begin();
    lcd.init();
    lcd.backlight();
    lcd.print("Scanner une");
    lcd.setCursor(0, 1);
    lcd.print("carte RFID...");
    SPI.begin();
    mfrc522.PCD_Init();
    delay(1000);
    lcd.clear();

    pinMode(LED_VERTE, OUTPUT);
    pinMode(LED_ROUGE, OUTPUT);
    digitalWrite(LED_VERTE, LOW);
    digitalWrite(LED_ROUGE, LOW);
}

void loop() {
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    if (compareUID(mfrc522.uid.uidByte, chambre1UID)) {
        displayMessage("Bienvenue a", "la chambre 1");
        Serial.println("1999");
        activateLED(LED_VERTE);
    } else if (compareUID(mfrc522.uid.uidByte, chambre2UID)) {
        displayMessage("Bienvenue a", "la chambre 2");
        Serial.println("2004");
        activateLED(LED_VERTE);
    } else {
        displayMessage("Acces", "refuse");
        Serial.println("1");
        activateLED(LED_ROUGE);
    }

    mfrc522.PICC_HaltA();
}

bool compareUID(byte *readUID, byte *knownUID) {
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (readUID[i] != knownUID[i]) {
            return false;
        }
    }
    return true;
}

void displayMessage(const char *line1, const char *line2) {
    lcd.clear();
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
    delay(1000);
    lcd.clear();
    lcd.print("Scanner une");
    lcd.setCursor(0, 1);
    lcd.print("carte RFID...");
}

void activateLED(int ledPin) {
    digitalWrite(LED_VERTE, LOW);
    digitalWrite(LED_ROUGE, LOW);
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
}

void printUID(byte *uid, byte size) {
    for (byte i = 0; i < size; i++) {
        Serial.print(uid[i] < 0x10 ? "0" : "");
        Serial.print(uid[i], HEX);
        if (i < size - 1) Serial.print(":");
    }
    Serial.println();
}
