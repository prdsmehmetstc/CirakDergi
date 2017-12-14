/*
 * NodeMCU Ultrasonik Park Sensoru ve Mesafe Olcer
 * Mehmet SUTCU <info@mehmetsutcu.com> 2017 
 * Cirak Dergisi <www.cirakdergi.com>
 * Konfigurasyon: NodeMCU 1.0(ESP-12E Module), 80 MHz(tercihen 160 MHz), 921600 Upload Speed, 4M(3M SPIFFS)
 *
 * Baglantilar:
 *  NodeMCU    Ultrasonik Sensor   
 * (ESP8266)       (HC-SR04)
 *
 * D1              Trig         
 * D2              Echo          
 * -------------------------------------
 * D4              Buzzer (+) Pin          
 * D5              aled (+) pin         
 * D6              bled (+) pin         
 * D7              cled (+) pin        
 * D8              dled (+) pin          
 * -------------------------------------
 * G               GND (-) Pinler         
 */

const int trigger_pin = D1; //D1 pinini sensorun trigger pini olarak tanimladik.
const int echo_pin = D2; //D2 pinini sensorun echo pini olarak tanimladik.
const int aled = D5; //D5 pinini aled olarak tanimladik.
const int bled = D6; //D6 pinini bled olarak tanimladik. 
const int cled = D7; //D7 pinini cled olarak tanimladik.
const int dled = D8; //D8 pinini dled olarak tanimladik.
const int buzzer = D4; //D4 pinini buzzer olarak tanimladik.
int sure ; //sure adli bir degisken tanimladik.
int mesafe ; //mesafe adli bir degisken tanimladik.
 
void setup() {

Serial.begin(9600); //Seri portu 9600 baud değerine ayarladik.
pinMode(aled , OUTPUT); //aled'i cikis olarak tanimladik.
pinMode(bled , OUTPUT); //bled'i cikis olarak tanimladik.
pinMode(cled , OUTPUT); //cled'i cikis olarak tanimladik.
pinMode(dled , OUTPUT); //dled'i cikis olarak tanimladik.
pinMode(buzzer , OUTPUT); //buzzer'i cikis olarak tanimladik.
pinMode(trigger_pin , OUTPUT); //trigger pin'i cikis olarak tanimladik.
pinMode(echo_pin , INPUT); //echo pin'i giris olarak tanimladik.

}
 
void loop() {
 
digitalWrite(trigger_pin , HIGH); //trigger pin'e enerji verdik.
delayMicroseconds(1000);
digitalWrite(trigger_pin , LOW); //trigger pin'in enerjisini kestik.

sure = pulseIn(echo_pin , HIGH); //sure degiskeninin echo pin'in degerini degistirmesini sagladik.
mesafe = (sure / 2) / 29.1;
Serial.print(mesafe); //Olculen mesafeyi seri porta yazdirdik.
Serial.println("Cm");

if (mesafe <= 10) //mesafe 10 cm'den kisa ise:
 
{
 
digitalWrite(aled , HIGH); //aled'e enerji verdik.
digitalWrite(bled , HIGH); //bled'e enerji verdik.
digitalWrite(cled , HIGH); //cled'e enerji verdik.
digitalWrite(dled , HIGH); //dled'e enerji verdik.
digitalWrite(buzzer , HIGH); //buzzer'e enerji verdik.
delay(250); //0,25 saniyede bir tekrarlanmasini sagladik.
digitalWrite(buzzer , LOW); //buzzer'in enerjisini kestik.
 
}
 
else if (mesafe <= 25) //mesafe 25 cm'den kisa ise:
 
{
 
digitalWrite(aled , LOW); //aled'in enerjisini kestik.
digitalWrite(bled , HIGH); //bled'e enerji verdik.
digitalWrite(cled , HIGH); //cled'e enerji verdik.
digitalWrite(dled , HIGH); //dled'e enerji verdik.
digitalWrite(buzzer , HIGH); //buzzer'e enerji verdik.
delay(500); //0,50 saniyede bir tekrarlanmasini sagladik
digitalWrite(buzzer , LOW); //buzzer'in enerjisini kestik.
 
}
 
else if ( mesafe <= 50) //mesafe 50 cm den kisa ise:
 
{
 
digitalWrite(aled , LOW); //aled'in enerjisini kestik.
digitalWrite(bled , LOW); //bled'in enerjisini kestik.
digitalWrite(cled , HIGH); //cled'e enerji verdik.
digitalWrite(dled , HIGH); //dled'e enerji verdik.
digitalWrite(buzzer , HIGH); //buzzer'e enerji verdik.
delay(750); //0,75 saniyede bir tekrarlanmasini sagladik.
digitalWrite(buzzer , LOW); //buzzer'in enerjisini kestik.
 
}
 
else //yukaridaki sartlar karsilanmiyorsa:
 
{
 
digitalWrite(aled , LOW); //aled'in enerjisini kestik.
digitalWrite(bled , LOW); //bled'in enerjisini kestik.
digitalWrite(cled , LOW); //cled'in enerjisini kestik.
digitalWrite(dled , HIGH); //dled'e enerji verdik.
 
  }
}
