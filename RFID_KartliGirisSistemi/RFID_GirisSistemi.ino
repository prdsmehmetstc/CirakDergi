/*
 * NodeMCU RC522 RFID Kartlı Geçiş Sistemi
 * Mehmet SÜTCÜ <info@mehmetsutcu.com> 2018 
 * Çırak Dergisi
 * Konfigürasyon: NodeMCU 1.0(ESP-12E Module), 80 MHz(İsteğe bağlı 160 MHz), 
 * 921600 Upload Speed, Flash, 4M(3M SPIFFS), Erase Flash: Only Sketch (Kart sürümü 2.4.2)
 */

#define SS_PIN D4 //SDA Pini
#define RST_PIN D3

int alarmPin = D8;
int yesilLed = D1;
int kirmiziLed = D2;

#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   
int statuss = 0;
int out = 0;

void setup() 
{
  pinMode(alarmPin, OUTPUT);
  pinMode(yesilLed, OUTPUT);
  pinMode(kirmiziLed, OUTPUT);
  
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init();   
}

void loop() 
{
  // Yeni kartlari tara
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Kart okundu ise
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Kartin UID'sini oku ve seri porta yaz
  Serial.println();
  Serial.print(" UID etiketi :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "2B 0F 20 A3") // Bu UID degerini tanimlamak istediginiz kartin degeri ile degistirin.
  {
    Serial.println(" Giris Basarili ! ");
    Serial.println(" Hos Geldiniz, Sn. Cirak Dergi Okuyucusu. ");
     digitalWrite(alarmPin, HIGH);
     digitalWrite(yesilLed, HIGH);
      delay(500);
    digitalWrite(alarmPin, LOW);
    digitalWrite(yesilLed, LOW);
    delay(1000);
    Serial.println(" Eglencenize bakin :) ");
    Serial.println();
    statuss = 1;
   
  }
  
  else   {
    Serial.println(" Kart Tanimlanmadi ! ");
    Serial.println(" Giris Basarisiz. ");
    digitalWrite(alarmPin, HIGH);
    digitalWrite(kirmiziLed, HIGH);
      delay(100);
    digitalWrite(alarmPin, LOW);
    digitalWrite(kirmiziLed, LOW);
      delay(100);
    digitalWrite(alarmPin, HIGH);
    digitalWrite(kirmiziLed, HIGH);
      delay(100);
    digitalWrite(alarmPin, LOW);
    digitalWrite(kirmiziLed, LOW);
      delay(100);
    digitalWrite(alarmPin, HIGH);
    digitalWrite(kirmiziLed, HIGH);
      delay(100);
    digitalWrite(alarmPin, LOW);
    digitalWrite(kirmiziLed, LOW);
      delay(100);
    
    delay(3000);
  }
} 
