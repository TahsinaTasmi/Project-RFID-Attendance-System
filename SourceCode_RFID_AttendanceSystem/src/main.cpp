#include "LCD16x2.h"
#include <Wire.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 53
#define RST_PIN 9

LCD16x2 lcd;
String UID = "33778a33"; // Student card ID that needs to be stored in the Attendance system 
int buttons;

MFRC522 mfrc522(SS_PIN, RST_PIN); // MFRC522 instance
MFRC522::MIFARE_Key key;

void setup()
{
  Serial.begin(9600); // Initialize serial communications
  while (!Serial)
    ;
  Serial.println("Initializing RFID module...");
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("RFID module initialized.");
  Wire.begin();
  lcd.lcdClear();
  lcd.lcdGoToXY(2, 1);
  lcd.lcdWrite("Hello! Please");
  lcd.lcdGoToXY(2, 2);
  lcd.lcdWrite("Scan Your Card");
}

String getUID();
bool isAuthorized(String uid);

void loop()
{
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())
  {
    Serial.println("Card detected!");
    String uid = getUID();
    Serial.print("Read UID: ");
    Serial.println(uid);

    lcd.lcdClear();
    lcd.lcdGoToXY(2, 1);

    if (isAuthorized(uid))
    {
      Serial.println("ID Recorded.");
      lcd.lcdWrite("ID Recorded.");
      lcd.lcdGoToXY(2, 2);
      lcd.lcdWrite(("Id: " + uid).c_str()); // Converting String to char* before passing to lcdWrite
    }
    else
    {
      Serial.println("Unauthorized ID");
      lcd.lcdWrite("Unauthorized ID");
    }
  }
}

String getUID()
{
  String uid = ""; // Initialization of an empty string
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  return uid;
}

bool isAuthorized(String uid)
{
  String authorizedUID = "33778a33";

  // Checking if the sacnned UID matches the authorized student UIDs

  return uid.equals(authorizedUID);

}


