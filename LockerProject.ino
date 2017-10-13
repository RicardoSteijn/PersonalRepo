#include <MFRC522.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define SS_PIN 10
#define RST_PIN 9
#define schuifMagneet 8
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Scan PICC to see UID and type...");
  pinMode(schuifMagneet, OUTPUT);
  
  lcd.init();
  lcd.backlight();
}

void loop()
{
  // standaard wordt dit op het display getoont.
  lcd.setCursor(0,0);
  lcd.print("Houd tag voor");
  lcd.setCursor(0,1);
  lcd.print("scanner");
  
  // Deze code is alleen om de RFID te testen in de Serial monitor
  //##########################################################################################################################################
  
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()){
    return;
  }
  
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  // ###########################################################################################################################################

  //Als de UID's overeen komen met wat er op de tags staat, dan open.
  if (content.substring(1) == "F5 B5 F0 C2" || content.substring(1) == "40 F1 07 85" || content.substring(1) == "01 02 03 04")
  {
    digitalWrite(schuifMagneet, HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Authorized");
    lcd.setCursor(0,1);
    lcd.print("Access");
    delay(3000);
  }
 
 else   {
    digitalWrite(schuifMagneet, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Access denied");
    delay(3000);
  }
}


