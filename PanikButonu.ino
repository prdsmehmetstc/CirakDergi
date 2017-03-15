/*
 * NodeMCU Panik Butonu
 * Mehmet SÜTCÜ <info@mehmetsutcu.com> 2016 
 * Çırak Dergisi
 * Konfigürasyon: NodeMCU 1.0(ESP-12E Module), 80 MHz(İsteğe bağlı 160 MHz), 921600 Upload Speed, 4M(3M SPIFFS)
 */

#include <SPI.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "KablosuzAgAdi";
const char* password = "KablosuzAgSifresi";
const char* MQTT_SERVER = "iot.eclipse.org";

WiFiClient espClient;
PubSubClient client(espClient);

// 5 numaralı pine, yani D1 pinine alarmPin adını veriyoruz.  
int alarmPin = 5;

// ALARM_KAPAT ve ALARM_AC modlarını tanımlıyoruz.
#define ALARM_KAPAT    0  
#define ALARM_AC     1  
int alarmModu = 0;

char message_buff[100];

void setup_wifi() {

  delay(10);
  // Seri porttan WiFi ağına bağlanıp bağlanmadığını okuyabilmek için bu kısmı yazıyoruz. 
  Serial.println();
  Serial.println(ssid);
  Serial.print(" ağına bağlanılıyor...");
  

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Başarıyla ağa bağlanıldı!");
  Serial.println("IP adresi: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;

  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  
 // Bu kısımda yukarıda tanımladığımız modları hangi komutla çalıştıracağımızı yazıyoruz. 
  if (msgString.equals("0")) {
    alarmModu = ALARM_KAPAT;
    client.publish("sonDurum", "Alarm Kapalı.");
    Serial.println("Alarm Kapalı.");
  } else if (msgString.equals("1")) {
    alarmModu = ALARM_AC;
    client.publish("sonDurum", "Alarm Açık!");
    Serial.println("Alarm Açık!");
  } 
}

void setup()
{
  // alarmPin'i OUTPUT, yani çıkış olarak tanımlıyoruz.
  // NodeMCU'ya alarmPin olarak tanımladığımız pine enerji vermesi gerektiğini söylüyoruz. 
  pinMode(alarmPin, OUTPUT);
  
  // Seri portun baud rate'ini ayarlıyoruz. 
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
}




void loop()
{
  if (!client.connected())
  {
      // MQTT'ye bağlandığında ne yapacağı kısmını buraya yazıyoruz.
      // "void loop()" kısmında yazdığımız kod döngüye girer.  
      client.connect("CirakDergiPanikButonu");
      client.publish("sonDurum", "Bir NodeMCU, başarıyla sunucuya bağlandı!");
      client.subscribe("butonDurum");
      Serial.println("Bir NodeMCU, başarıyla sunucuya bağlandı!");
  }
  
  switch (alarmModu) {
    case ALARM_KAPAT:
      digitalWrite(alarmPin, LOW);
      break;
    case ALARM_AC:
      digitalWrite(alarmPin, HIGH);
      delay(500);
    digitalWrite(alarmPin, LOW);
      delay(500);
      
  }
  
  // MQTT ile bağlantıyı sürekli canlı tutarak kopmayı önlüyor. 
  // Kısacası bağlantıyı loop'a, yani döngüye sokuyor.
  client.loop();
}
