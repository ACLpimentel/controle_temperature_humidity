/* ESP32 WiFi Scanning example */

#include "WiFi.h"
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN1_1
#define DHTTYPE_1 DHT22
DHT dht(DHTPIN_1, DHTTYPE_1);

#define DHTPIN1_2
#define DHTTYPE_2 DHT22
DHT dht(DHTPIN_2, DHTTYPE_2);

LiquidCrystal_I2C lcd(0x27, 20, 4);


bool camara = 0;

void setup() {
  // ####### LCD #######
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Cabine 1");
  lcd.setCursor(0,1);
  lcd.print("Humi: xx%|Temp: xxC");
  lcd.setCursor(0,2);
  lcd.print("Cabine 2");
  lcd.setCursor(0,3);
  lcd.print("Humi: xx%|Temp: xxC");

  dht.begin();
  
}

void loop() {


// ####### CAMARA 1 ########
  float h1 = dht.readHumidity();
  float t1 = dht.readTemperature();

  lcd.setCursor(5,1);
  lcd.print(h1);
  lcd.setCursor(15,1);
  lcd.print(t1);


//###### CAMARA 2 #######
  float h2 = dht.readHumidity();
  float t2 = dht.readTemperature();

  lcd.setCursor(5,3);
  lcd.print(h2);
  lcd.setCursor(15,1);
  lcd.print(t2);
  
}

// change the camera selected to chage the temperature
void  setCamara(){



}