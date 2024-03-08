/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "Automacao";
const char* password = "127.0.0.1...";


// INPUT PARAMETER
const char* PARAM_TEMP_1 = "goaltemp1";
const char* PARAM_TEMP_2 = "goaltemp2";

String inputMessage;
String inputMessage2;

String lastTemperature1 = "31";
String lastTemperature2 = "32";
String lastHumidity1 = "91%";
String lastHumidity2 = "92%";
String goalhumi1;
String goalhumi2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if(var == "TEMP1"){
    return lastTemperature1;
  }
  else if(var == "TEMP2"){
    return lastTemperature2;
  }
  else if(var == "HUMI1"){
    return lastHumidity1;
  }
  else if(var == "HUMI2"){
    return lastHumidity2;
  }
  else if(var == "GOALHUMI1"){
    return goalhumi1;
  }
  else if(var == "GOALHUMI2"){
    return goalhumi2;
  }
  return String();
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });


// GET VALUES
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage>
    if (request->hasParam(PARAM_TEMP_1)) {
      goalhumi1 = request->getParam(PARAM_TEMP_1)->value();

    }
      // GET enable_arm_input value on <ESP_IP>/get?enable_arm_input=<inputMessage2>
    if (request->hasParam(PARAM_TEMP_2)) {
        goalhumi2 = request->getParam(PARAM_TEMP_2)->value();
      }
     else {
      }
      
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field () with value: <br><a href=\"/\">Return to Home Page</a>");

    Serial.println(goalhumi1);
    Serial.println(goalhumi2);
    //request->send(200, "text/html", "<script> function Alert(){ alert(\"Temperatura modificada com sucesso!\") </script>});
  });


  // Start server
  server.begin();
}
 
void loop(){
  
}
