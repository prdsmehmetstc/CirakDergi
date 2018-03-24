/*
 * NodeMCU DHT11 Sıcaklık ve Nem Ölçer
 * Mehmet SÜTCÜ <info@mehmetsutcu.com> 2018 
 * Çırak Dergisi
 * Konfigürasyon: NodeMCU 1.0(ESP-12E Module), 80 MHz(İsteğe bağlı 160 MHz), 
 * 921600 Upload Speed, 4M(3M SPIFFS), Erase Flash: Only Sketch (Kart sürümü 2.4.1)
 */

// Gerekli yazilim kutuphanelerini ekledik.
#include <ESP8266WiFi.h>
#include <DHT.h>

// DHT sensor tipini belirliyoruz.
#define DHTTYPE DHT11 

// Bu kısmı kendi kablosuz ag bilgileriniz ile degistirin!
const char* ssid = "KablosuzAgAdi";
const char* password = "KablosuzAgParolasi";

// Web server'ı 80 portunda baslatiyoruz.
WiFiServer server(80);

// DHT11'in bagli oldugu pini belirtiyoruz.
const int DHTPin = D4;
// DHT sensorunu tani.
DHT dht(DHTPin, DHTTYPE);

// Gecici degiskenler atadik.
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

void setup() {
  // Seri portun baud rate'ini belirle.
  Serial.begin(115200);
  delay(10);

  dht.begin();
  
  // Aga baglanma kismi.
  Serial.println();
  Serial.print(ssid);
  Serial.println(" agina baglaniliyor...");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Aga basariyla baglanildi!");
  
  // Web server'i calistirma.
  server.begin();
  Serial.println("Web server calisiyor. ESP'nin IP almasi icin bekleniyor...");
  delay(10000);
  
  // ESP'nin IP adresini yazdir.
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Yeni baglantilari kontrol et.
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("*Yeni baglanti*");
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            // Nemi oku.
            float h = dht.readHumidity();
            // Sicakligi Santigrat olarak oku.
            float t = dht.readTemperature();
            // Sicakligi Fahrenheit olarak oku.
            float f = dht.readTemperature(true);
            // Okuma islemini kontrol et.
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println("DHT sensoru okunamadi.");
              strcpy(celsiusTemp,"HATA!");
              strcpy(fahrenheitTemp, "HATA!");
              strcpy(humidityTemp, "HATA!");         
            }
            else{
              // Santigrat, Fahrenheit ve Nem ile hissedilen sicaklik degerini hesapla.
              float hic = dht.computeHeatIndex(t, h, false);       
              dtostrf(hic, 6, 2, celsiusTemp);             
              float hif = dht.computeHeatIndex(f, h);
              dtostrf(hif, 6, 2, fahrenheitTemp);         
              dtostrf(h, 6, 2, humidityTemp);
              // Seri porta degerleri yazdir.
              Serial.print("Nem: ");
              Serial.print(h);
              Serial.print(" %\t Sicaklik: ");
              Serial.print(t);
              Serial.print(" *C ");
              Serial.print(f);
              Serial.print(" *F\t Hissedilen: ");
              Serial.print(hic);
              Serial.print(" *C ");
              Serial.print(hif);
              Serial.print(" *F");
            }
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // Degerleri web sayfasina yazdir.
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>NodeMCU - Sicaklik ve Nem Gosterge Sayfasi</h1><h3>Sicaklik(Santigrat): ");
            client.println(celsiusTemp);
            client.println("*C</h3><h3>Sicaklik(Fahrenheit): ");
            client.println(fahrenheitTemp);
            client.println("*F</h3><h3>Nem: ");
            client.println(humidityTemp);
            client.println("%</h3><h3>");
            client.println("</body></html>");     
            break;
        }
        if (c == '\n') {
          // Yeni bir satir okumaya basladiginda
          blank_line = true;
        }
        else if (c != '\r') {
          // mevcut satirda bir karakter buldugunda
          blank_line = false;
        }
      }
    }  
    // Baglantiyi sonlandir.
    delay(1);
    client.stop();
    Serial.println("\nBaglanti sonlandi.");
  }
}   

