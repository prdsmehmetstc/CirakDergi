/*
 * NodeMCU WiFi Sinyal Gucu Olcer
 * Mehmet SUTCU <info@mehmetsutcu.com> 2017 
 * Cirak Dergisi <www.cirakdergi.com>
 * Konfigurasyon: NodeMCU 1.0(ESP-12E Module), 80 MHz(tercihen 160 MHz), 921600 Upload Speed, 4M(3M SPIFFS)
 *
 * RSSI, -120db ile 0db araligindaki bir yuzde deger sistemidir.
 * Sifira(0) yakin olan deger daha iyidir.
 *
 * Baglantilar:
 *  NodeMCU        Nokia 5110   
 * (ESP8266)       PCD8544 LCD
 *
 * D2 (GPIO4)      0 RST         
 * D1 (GPIO5)      1 CE          
 * D6 (GPIO12)     2 DC          
 * D7 (GPIO13)     3 Din         
 * D5 (GPIO14)     4 Clk         
 * VU              5 Vcc        
 * D0 (GPIO16)     6 BL          
 * G               7 Gnd         
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pins
const int8_t RST_PIN = D2;
const int8_t CE_PIN = D1;
const int8_t DC_PIN = D6;
const int8_t BL_PIN = D0;

// Olcum yapilacak olan agi tanimla.
const char* myssid = "KablosuzAgAdi";
const char* mypass = "KablosuzAgParolasi";

long rssi;
int8_t graph[83];
uint8_t i, col, pos = 0;
bool scroll = false;

Adafruit_PCD8544 display = Adafruit_PCD8544(DC_PIN, CE_PIN, RST_PIN);

void setup(void) {
  Serial.begin(9600);
  Serial.println("\nNodeMCU WiFi Gucu Olcer");
  Serial.println("NodeMCU (ESP8266) ve Nokia 5110 LCD Ekrani ile yapildi.\nCirak Dergisi\n");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // LCD Backlight'ini ac
  pinMode(BL_PIN, OUTPUT);
  digitalWrite(BL_PIN, HIGH);

  // LCD Ayarlamalari
  display.begin();
  display.setContrast(15);  // Ekraniniza gore kontrasti duzenleyin.
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.clearDisplay();

  WiFi.begin(myssid, mypass);
  Serial.print("Baglaniyor");
  display.print("Baglaniyor");
  display.display();

  // Baglanana kadar bekle.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }

  Serial.print("\nSu aga baglanildi: ");
  Serial.println(myssid);
  Serial.print("IP adresi: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  display.clearDisplay();
  display.println("\nBaglanilan Ag: ");
  display.println(myssid);
  display.display();
  delay(2000);
}

void loop(void) {
  rssi = WiFi.RSSI();
  graph[pos] = (1.0 - (rssi / -100.0)) * -48.0;

  // RSSI grafigini ekrana ciz.
  display.clearDisplay();
  display.printf("%ddb\n",rssi);

  if (!scroll) {
    for (i = 0; i <= pos; i++) {
      display.drawFastVLine(i, LCDHEIGHT-1, graph[i], BLACK);
    }
  }
  else {
    for (i = 0; i < LCDWIDTH; i++) {
      col = (i + pos + 1) % LCDWIDTH;
      display.drawFastVLine(i, LCDHEIGHT-1, graph[col], BLACK);
    }
  }
  display.display();

  if (pos == 83) {
    pos = 0;
    scroll = true;
  }
  else {
    pos++;
  }

  delay(100);  // Grafik yenileme hizini degistirmek istiyorsaniz bu degeri degistirin.
}
