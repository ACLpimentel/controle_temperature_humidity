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

// ds18b20 sensor======================================
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 13

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;

DeviceAddress tempDeviceAddress;
// DHT SENSOR===========================================
#include "DHT.h"

#define DHTPIN_1 15
#define DHTTYPE_1 DHT22

DHT dht_1(DHTPIN_1, DHTTYPE_1);

#define DHTPIN_2 2
#define DHTTYPE_2 DHT22

DHT dht_2(DHTPIN_2, DHTTYPE_2);

// INPUT PARAMETER======================================
const char* PARAM_TEMP_1 = "goaltemp1";
const char* PARAM_TEMP_2 = "goaltemp2";

String inputMessage;
String inputMessage2;

String camaraTemperatura[2];
String outTemperature[2];
String camaraHumidade[2];
String goalhumi1;
String goalhumi2;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


// Replaces placeholder with LED state value
String processor(const String& var) {
  Serial.println(var);
  if (var == "TEMP1") {
    return camaraTemperatura[0];
  } else if (var == "TEMP2") {
    return camaraTemperatura[1];
  } else if (var == "HUMI1") {
    return camaraTemperatura[0];
  } else if (var == "HUMI2") {
    return camaraTemperatura[1];
  } else if (var == "GOALHUMI1") {
    return goalhumi1;
  } else if (var == "GOALHUMI2") {
    return goalhumi2;
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  //dht====================================================================
  dht_1.begin();
  dht_2.begin();

  //ds18b20 sensor ========================================================
  sensors.begin();

  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for (int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }


  // Initialize SPIFFS=======================================================
  if (!SPIFFS.begin(true)) {
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });


  // GET VALUES
  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    // GET threshold_input value on <ESP_IP>/get?threshold_input=<inputMessage>
    if (request->hasParam(PARAM_TEMP_1)) {
      goalhumi1 = request->getParam(PARAM_TEMP_1)->value();
    }
    // GET enable_arm_input value on <ESP_IP>/get?enable_arm_input=<inputMessage2>
    if (request->hasParam(PARAM_TEMP_2)) {
      goalhumi2 = request->getParam(PARAM_TEMP_2)->value();
    } else {
    }

    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field () with value: <br><a href=\"/\">Return to Home Page</a>");

    Serial.println(goalhumi1);
    Serial.println(goalhumi2);
    //request->send(200, "text/html", "<script> function Alert(){ alert(\"Temperatura modificada com sucesso!\") </script>});
  });


  // Start server
  server.begin();
}

void loop() {
  // dht ======================================================================
  float h_1 = dht_1.readHumidity();
  // Read temperature as Celsius (the default)
  float t_1 = dht_1.readTemperature();

  camaraTemperatura[0] = String(t_1);
  camaraHumidade[0] = String(h_1);


  float h_2 = dht_2.readHumidity();
  // Read temperature as Celsius (the default)
  float t_2 = dht_2.readTemperature();

  camaraTemperatura[1] = String(t_2);
  camaraHumidade[1] = String(h_2);

  // ds18b20===================================================================
  sensors.requestTemperatures();  // Send the command to get temperatures

  // Loop through each device, print out temperature data
  for (int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i)) {
      // Output the device ID
      Serial.print("Temperature for device: ");
      Serial.println(i, DEC);
      // Print the data
      float tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print("Temp C: ");
      Serial.print(tempC);
      Serial.print(" Temp F: ");
      Serial.println(DallasTemperature::toFahrenheit(tempC));  // Converts tempC to Fahrenheit
    }
  }
  delay(5000);
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
