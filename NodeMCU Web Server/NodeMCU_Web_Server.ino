/*
 * NodeMCU Web Server ile LED Kontrol
 * Mehmet SUTCU <info@mehmetsutcu.com> 2017 
 * Cirak Dergisi <www.cirakdergi.com>
 * Konfigurasyon: NodeMCU 1.0(ESP-12E Module), 80 MHz(tercihen 160 MHz), 921600 Upload Speed, 4M(3M SPIFFS)
 */

#include <ESP8266WiFi.h>
 
const char* ssid = "KablosuzAgAdi"; // Cihazin baglanacagi kablosuz agin adini
const char* password = "KablosuzAgSifresi"; // ve sifresini bu kisma yaziyoruz.
 
int ledPin = 5; // LED'in D1(5) pinine bagli oldugunu belirtiyoruz.
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Kablosuz aga baglan.
  Serial.println();
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
 
  // Web Serveri calistir.
  server.begin();
  Serial.println("Web Server calisiyor.");
 
  // Baglanti bilgilerini goster.
  Serial.print("Baglanmak icin su linki kopyalayin: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  Serial.print("Adresi kopyalamak icin Ctrl+C kombinasyonunu kullanin.");
  
}
 
void loop() {
  // Bir istemci baglanmis mi, kontrol et.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Bir istemcinin veri gondermesini bekle.
  Serial.println("Bir istemci baglandi!");
  while(!client.available()){
    delay(1);
  }
 
  // Istegin ilk satirini oku.
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Gelen veriyi eslestir.
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
  // Gelen veriye cevap ver.
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("LED Durumu: ");
 
  if(value == HIGH) {
    client.print("Acik");
  } else {
    client.print("Kapali");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Ac </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Kapat </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Istenen islem basariyla gerceklestirildi.");
  Serial.println("");
 
}
 
