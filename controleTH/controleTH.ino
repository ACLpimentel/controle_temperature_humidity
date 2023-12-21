/* ESP32 WiFi Scanning example */

#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "WiFi.h"

const char* ssid ="";
const char* password = "";
WiFiServer server(80);

#define DHTPIN1_1
#define DHTTYPE_1 DHT22
DHT dht(DHTPIN_1, DHTTYPE_1);

#define DHTPIN1_2
#define DHTTYPE_2 DHT22
DHT dht(DHTPIN_2, DHTTYPE_2);

LiquidCrystal_I2C lcd(0x27, 20, 4);

// buttons
bool selectBut;
bool upBut;
bool downBut;

int temp1;
int temp2;

bool camara = 1;

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

  WiFi.begin(ssid,password);

  while(Wifi.status() != WL_CONNECTED);
  
}

void loop() {

  refreshDisplay();

// change camara
  if (selectBut == 1){

    while(selectBut == 1){}

    if (camara == 1){ camara = 0}
    else { camara = 1}
    
    setCamaraDisplay();

    double time = millis();
    
    while (millis() - time <= 10000){ 
    
      if (selectBut == 1){
        time = millis();
        while(selectBut == 1){}
      }
      if (camara == 1){ camara = 0}
      else { camara = 1}
      
      setTemperature();
  }
  

  
  }


  
}

void  setCamaraDisplay(bool camara){
  if(camera == 0){
    lcd.setCursor(19,0);
    lcd.print("**");
    lcd.setCursor(19,2);
    lcd.print("  ");
  }
    
  } 
  else if(camera == 1){
    lcd.setCursor(19,0);
    lcd.print("  ");
    lcd.setCursor(19,2);
    lcd.print("**");
  }

}

void setTemperture(){

  swith (camara)

  case 0:

    if (upBut == 1){
      while(selectBut == 1){}
      temp1++;
    }

    if (downBut == 1){
      while(selectBut == 1){}
      temp1--;
    }
  break;

  case 1:

    if (upBut == 1){
      while(selectBut == 1){}
      temp2++;
    }

    if (downBut == 1){
      while(selectBut == 1){}
      temp2--;
    }
  break;

}

void refreshDisplay(){

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
  lcd.setCursor(15,3);
  lcd.print(t2);

}


String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP8266 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP8266 NodeMCU Weather Report</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}